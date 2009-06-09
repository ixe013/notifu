// CommandLineParser.h: StandardCommandLineParser et al
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2000-2001, DevelopMentor.
// All rights reserved. No warranties extended.
// You may use them without royalty (and without support) in your compiled
// code. You may not redistribute the source code.
// Inspired by Josh Gray.
/////////////////////////////////////////////////////////////////////////////
// Features:
// -Parsing command line args from a single string or argv/argc
// -Simple flags and flag/arg pairs
// -Typed args, both params and flags
// -Building usages on the fly
// -Command line args can be read via @argfile.
// -Automatically generates the banner logo from the VERSIONINFO resource.
// -Case insensitive flag comparisions by default, case sensitive as an option.
/////////////////////////////////////////////////////////////////////////////
// Limitations:
// -Requires flags to be space separated, e.g.
//  /efg is a single flag, even if e, f and g are defined and efg isn't
// -Requires flag and value to be space separated, e.g.
//  /fconfig.sys is treated as a single flag, even if f is defined to take
//  a value and fconfig.sys isn't defined
// -Need a good way for FileNameValue to communicate a file not found error
//  if we give it a MustExist property. No need to make the calling program
//  handle that usage error when we handle all the others. Specializing
//  ValueArg would be a slippery slope (what about MultiValueArg?) *plus*
//  Argument::ConsuleValue doesn't get to participate in the error reporting
//  anyway. Exceptions are probably the way to go, since we're relying on
//  the standard library so much anyway...
// -Need better formatting to pad flag/param names that are larger than
//  16 characters and to support multi-line descriptions.
/////////////////////////////////////////////////////////////////////////////
// History:
//  8/17/01:
//  -Tim Tabor [tltabor@earthlink.net] contributed the ability to be able to
//   set a default value for a FileNameValue. He also pointed out a const bug.
//   Thanks, Tim!
//
//  5/2/01:
//  -Paul Westcott [paul_westcott@hotmail.com] added a class 
//   RestrictedValueArg class which allows a list of values to be specified.
//
//  4/4/01:
//  -Chris Tavares [ctavares@develop.com] fixed a nasty memory leak that the
//   compiler mistakenly reported as a warning. Thanks, Chris!
//
//  3/29/01:
//  -Added support for case sensitivity by default, case insensitive as an option.
//
//  3/25/01:
//  -Unicode support from Johan Nilsson (johan.nilsson@esrange.ssc.se).
//  -Unicode arg file support, also from Johan.
//  -Bug fix (you guess it, Johan...) to avoid pushing the leading exe name if
//   there are no command line args.
//
//  12/22/00:
//  -Bug fixes
//
//  10/16/00:
//  -Tim Tabor [tltabor@earthlink.net] pointed out IMAGE_NT_SIGNATURE to me.
//  -Changed SimpleFlag to FlagArg.
//  -Fixed some usage formatting problems.
//  -Added some typedefs.
//  -Removed /nologo from StandardCommandLineParser (it's not used)
//
//  10/15/00:
//  -Initial release
/////////////////////////////////////////////////////////////////////////////
// Usage:
/*
#include "CommandLineParser.h"

struct TestCommandLine : public StandardCommandLineParser
{
public:
    // Create flags and params
    FlagArg                         debug;
    ValueArg<int>                   minX;
    ValueArg<double>                intensity;
    ValueArg<tstring>               foo;
    MultiValueArg<tstring>          i;
    MultiValueArg<FileNameValue>    files;

public:
    TestCommandLine()
        // Set names and descriptions for usage
    :   debug(__T("debug"), __T("Turn on debugger.")),
        minX(__T("MinX"), __T("Minimum x value.")),
        intensity(__T("Intensity"), __T("Intensity value.")),
        foo(__T("foo"), __T("Not the bar.")),
        i(__T("i"), __T("Include directories.")),
        files(__T("files"), __T("File names."))
    {
        // Add flags matched by name, e.g. /foo
        AddFlag(debug);

        AddFlag(minX);
        minX.SetDefaultValue(10);

        AddFlag(intensity);
        //intensity.SetDefaultValue(1.1);   // No default means this arg is required

        AddFlag(i);
        AddFlag(foo);
        foo.SetDefaultValue(__T("bar"));

        // Add params matched by position, e.g. foo
        AddParam(files);
    }
};

void _tmain(int argc, LPCTSTR* argv)
{
    // Parse the command line
    TestCommandLine cl;
    cl.Parse(argc, argv);

    // Will show usage or version if there's an error, /?|h|help or /v|version
    if( !cl.Continue() ) return;

    // Harvest values
    std::cout << "debug: " << cl.debug << std::endl;
    std::cout << "minX: " << cl.minX << std::endl;
    std::cout << "intensity: " << cl.intensity << std::endl;
    std::cout << "foo: " << cl.foo.Value() << std::endl;

    const std::vector<tstring>& includes = cl.i;
    std::cout << "includes:" << std::endl;
    for(std::vector<tstring>::const_iterator incl = includes.begin(); incl != includes.end(); ++incl)
        std::cout << "    '" << *incl << "'" << std::endl;

    const std::vector<FileNameValue>& params = cl.files.Values();
    std::cout << "nameValuePairs:" << std::endl;
    for(std::vector<FileNameValue>::const_iterator p = params.begin(); p != params.end(); ++p)
        std::cout << "    '" << *p << "'" << (p->Exists() ? " (exists)" : " (doesn't exist)") << std::endl;
}
*/

#pragma once
#ifndef INC_COMMANDLINEPARSER
#define INC_COMMANDLINEPARSER

#pragma warning(disable:4786) // identifier was truncated to '255' characters in the debug information
#pragma warning(disable:4267) // conversion from 'size_t' to 'int', possible loss of data

#include <windows.h>
#include <crtdbg.h> // _ASSERTE
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>	// replace_if
#include <functional>	// bind2nd, equal_to
#include <shellapi.h>   // SHGetFileInfo
#include "versionInfo.h"
#pragma comment(lib, "shell32.lib")

#ifndef __TSTRING_H__
#include <tchar.h>
#include <string>
#include <sstream>
typedef std::basic_string<TCHAR> tstring;
typedef std::basic_stringstream<TCHAR> tstringstream;
#endif

//
// NOTE: We are using this because I couldn't figure out how to
//       overload operator>> for FileNameValue.
template <typename T>
bool ReadFromString(const tstring& s, T* pValue)
{
    tstringstream   ss(s);
    ss >> *pValue;
    return true;
}

template <>
bool ReadFromString(const tstring& s, tstring* pValue);
/*
{
    // We need this specialization so that we the
    // entire string, including spaces. Otherwise,
    // operator>>(istream, s) will just take up to
    // the first space.
    *pValue = s;
    return true;
}
//*/

class Argument
{
public:
    Argument(const tstring& sName, const tstring& sDescription = __T(""));
    void AddAlternate(const tstring& sName);

    tstring Name() const;

    tstring sDescription() const;

    virtual ~Argument();

    bool Found() const;

    void SetFound(bool bFound = true);
    void SetDocumented(bool bDocumented = true);

    bool Matches(const tstring& sName) const;

    void SetCaseSensitive(bool bCaseSensitive = true);

public:
    virtual bool IsValid() const =0;
    virtual bool ExpectsValues() const =0;
    virtual bool WantsAValue() const =0;
    virtual bool ConsumeValue(const tstring& s) =0;
    virtual tstring Usage(const tstring sPrefix) const =0;
    virtual tstring LongUsage(const tstring sPrefix) const =0;

protected:
    tstring UsageHelper(const tstring& sPrefix = __T("/"), bool bOptional = true, const tstring& sValueName = __T("")) const;

    tstring LongUsageHelper(const tstring& sPrefix, const tstring& sValueName = __T("")) const;

protected:
    bool                    m_bFound:1;
    bool                    m_bCaseSensitive:1;
    std::vector<tstring>    m_rgNames;
    tstring                 m_sDescription;
    bool                    m_bDocumented;
};

class FlagArg : public Argument
{
public:
    FlagArg(const tstring& sName, const tstring& sDescription = __T(""));

    virtual bool IsValid() const;
    virtual bool ExpectsValues() const;
    virtual bool WantsAValue() const;
    virtual bool ConsumeValue(const tstring& sValue);
    virtual tstring Usage(const tstring sPrefix) const;
    virtual tstring LongUsage(const tstring sPrefix) const;
    operator bool() const;
};

template <typename T>
class ValueArg : public Argument
{
public:
    ValueArg(const tstring& sName, const tstring& sDescription = __T(""), const tstring& sValueName = __T("value"))
    :   Argument(sName, sDescription), m_sValueName(sValueName), m_bHasDefault(false)
    {
    }

    void SetDefaultValue(const T& value)
    {
        m_value = value;
        m_valueDefault = value;
        m_bHasDefault = true;
    }

    virtual bool IsValid() const
    {
        return m_bHasDefault || Found();
    }

    virtual bool ExpectsValues() const
    {
        return true;
    }

    virtual bool WantsAValue() const
    {
        return !Found();
    }

    virtual bool ConsumeValue(const tstring& s)
    {
        return ReadFromString(s, &m_value);
    }

    virtual tstring Usage(const tstring sPrefix) const
    {
        bool    bFlag = sPrefix.size() != 0;
        return UsageHelper(sPrefix, m_bHasDefault, bFlag ? m_sValueName : __T(""));
    }

    virtual tstring LongUsage(const tstring sPrefix) const
    {
        return LongUsageHelper(sPrefix, m_sValueName);
    }

    operator const T&() const
    {
        return m_value;
    }

    const T& Value() const
    {
        return m_value;
    }

protected:
    tstring m_sValueName;
    bool    m_bHasDefault;
    T       m_valueDefault;
    T       m_value;
};


template <typename T>
class RestrictedValueArg : public ValueArg<T>
{
protected:
    typedef std::pair<T, tstring>           RestrictedValue;
    typedef std::vector<RestrictedValue>    RestrictedValues;

public:
    RestrictedValueArg(const tstring& sName, const tstring& sDescription = __T(""), const tstring& sValueName = __T("value"))
    :   ValueArg<T>(sName, sDescription, sValueName),
        m_bRestrictedValueDescriptions(false)
    {
    }

    virtual bool IsValid() const
    {
        RestrictedValues::const_iterator it;
        const RestrictedValues::const_iterator last = m_rgValues.end();
        for( it = m_rgValues.begin(); it != last; ++it )
        {
            if( it->first == m_value ) return true;
        }

        return false;
    }

    virtual tstring Usage(const tstring& sPrefix) const
    {
        tstring sUsage = ValueArg<T>::Usage(sPrefix);

        if( !m_rgValues.empty() )
        {
            bool            bFirst = true;
            tstringstream   ssValues;

            RestrictedValues::const_iterator        it;
            const RestrictedValues::const_iterator  end = m_rgValues.end();
            for( it = m_rgValues.begin(); it != end; ++it )
            {
                if( bFirst) bFirst = false;
                else ssValues << _T('|');

                ssValues << it->first;
            }

            const tstring       sValue(_T("<value>"));
            tstring::size_type  pos = sUsage.find(sValue);
            if( tstring::npos != pos )
            {
                sUsage.replace(sUsage.begin() + pos, sUsage.begin() + pos + sValue.size(), ssValues.str());
            }
            // else something is wrong; UsageHelper() must have changed
        }

        return sUsage;
    }

    virtual tstring LongUsage(const tstring sPrefix) const
    {
        tstring sUsage = ValueArg<T>::LongUsage(sPrefix);

        if( m_bRestrictedValueDescriptions )
        {
            sUsage += _T(" values are:");

            RestrictedValues::const_iterator        it;
            const RestrictedValues::const_iterator  end = m_rgValues.end();
            for( it = m_rgValues.begin(); it != end; ++it )
            {
                sUsage += _T("\n");

                const size_t    LEFT_PAD = 3;
                const size_t    TOTAL_WITH_PADDING = 15;
                tstring         sValue(LEFT_PAD, _T(' '));
                sValue += it->first;

                if( sValue.size() < TOTAL_WITH_PADDING )
                {
                    sValue.resize(TOTAL_WITH_PADDING, _T(' ')); 
                }

                tstringstream   ssValues;
                ssValues << _T("\t\t") << sValue << _T(" ") << it->second;

                sUsage += ssValues.str();
            }
        }

        return sUsage;
    }

    void AddValue(const T& value, const tstring& sDescription = tstring())
    {
        if( !sDescription.empty() ) m_bRestrictedValueDescriptions = true;
        m_rgValues.push_back(RestrictedValue(value, sDescription));
    }

protected:
    RestrictedValues    m_rgValues;
    bool                m_bRestrictedValueDescriptions;
};

template <typename T, size_t nMinValues = 0, size_t nMaxValues = size_t(-1)>
class MultiValueArg : public Argument
{
public:
    MultiValueArg(const tstring& sName, const tstring& sDescription = __T(""), const tstring& sValueName = __T("value"))
    :   Argument(sName, sDescription), m_sValueName(sValueName)
    {
    }

    virtual bool IsValid() const
    {
        return m_rgValues.size() >= nMinValues;
    }

    virtual bool ExpectsValues() const
    {
        return true;
    }

    virtual bool WantsAValue() const
    {
        return m_rgValues.size() < nMaxValues;
    }

    virtual bool ConsumeValue(const tstring& s)
    {
        T       value;
        bool    b = ReadFromString(s, &value);
        if( !b ) return b;

        m_rgValues.push_back(value);
        return true;
    }

    virtual tstring Usage(const tstring sPrefix) const
    {
        bool    bFlag = sPrefix.size() != 0;
        return UsageHelper(sPrefix, true, bFlag ? __T("value") : __T("")) + __T("...");
    }

    virtual tstring LongUsage(const tstring sPrefix) const
    {
        return LongUsageHelper(sPrefix, m_sValueName);
    }

    size_t size() const
    {
        return m_rgValues.size();
    }

    operator const std::vector<T>&() const
    {
        return m_rgValues;
    }

    const std::vector<T>& Values() const
    {
        return m_rgValues;
    }

    const T& operator[](size_t i) const
    {
        _ASSERTE(i < m_rgValues.size());
        return m_rgValues[i];
    }

    const T& Value(size_t i) const
    {
        _ASSERTE(i < m_rgValues.size());
        return m_rgValues[i];
    }

protected:
    tstring         m_sValueName;
    std::vector<T>  m_rgValues;
};


// Resolves to full path name based on current working directory.
// For use with [Multi]ValueArg.
class FileNameValue : public tstring
{
public:
	FileNameValue();
	FileNameValue(const tstring& sFileName);
	FileNameValue(LPCTSTR pszFileName);

    bool Exists() const;
};


bool ReadFromString(const tstring& s, FileNameValue* pValue);

// Splits left=right into left and right.
// For use with [Multi]ValueArg.
class PairValue
{
public:
    tstring Left() const;
    tstring Right() const;

    friend
    bool ReadFromString(const tstring& s, PairValue* pValue);

private:
    tstring m_sLeft;
    tstring m_sRight;
};

bool ReadFromString(const tstring& s, PairValue* pValue);

class CommandLineParser
{
public:
    CommandLineParser(bool bAllowArgFile = true);

    void AddFlag(Argument& arg);
    void AddParam(Argument& arg);
    tstring Usage(const tstring& sErr = __T("")) const;
    bool Parse(int argc, const TCHAR** argv, bool bValidate = true, bool bLeadingExe = true);
    bool Parse(const tstring & CommandLine, bool bValidate = true, bool bLeadingExe = false);
    // Assumes exe name *not* in the first in the list
    bool Parse(const std::vector<tstring>& rgsArgs, bool bValidate = true);
    bool ParseFromFile(const tstring& sFileName);
    bool HasErrors() const;
    void ClearError();
    virtual tstring Logo() const;
    static bool FileExists(const tstring& sFileName);

    static tstring ModuleName();

    static bool IsWhiteSpace(TCHAR ch);

    static tstring SearchAndReplace(tstring s, const tstring& sFind, const tstring& sReplace, bool bGlobal = true);

public:
    enum ARG_TYPE { ARG_FLAG, ARG_PARAM };

private:
    template<typename invalid_t, invalid_t invalid_value>
    class SimpleHandleT
    {
    private:
        HANDLE m_h;
        SimpleHandleT& operator=(const SimpleHandleT&);
    
    public:
        SimpleHandleT(const HANDLE& h) : m_h(h) {}

        ~SimpleHandleT()
        {
            if( invalid_value != reinterpret_cast<invalid_t>(m_h) )
            {
                ::CloseHandle(m_h);
            }
        }
    
        bool operator!() const
        {
            return (invalid_value == reinterpret_cast<invalid_t>(m_h));
        }
    
        operator HANDLE() const
        { return m_h; }
    };
    
    
    typedef SimpleHandleT<int, -1> FileHandle; // -1 == INVALID_HANDLE_VALUE
    typedef SimpleHandleT<HANDLE, NULL> Handle;
    
    template<class From, class To>
    void ConvertStringData(const From* pFrom, size_t nSize, std::basic_string<To>& sTo) //throw (std::exception)
    { sTo.assign(pFrom, nSize); }

    template<>
    void ConvertStringData(const char* pFrom, size_t nSize, std::basic_string<wchar_t>& sTo) //throw (std::exception)
    {
        sTo.empty();
        
        int nLen = MultiByteToWideChar(GetACP(), 0, pFrom, nSize, 0, 0);
        std::vector< wchar_t > buf( nLen );
        wchar_t *pBuf = &buf[ 0 ];
        
        int nCvt = MultiByteToWideChar(GetACP(), 0, pFrom, nSize, pBuf, nLen);
        _ASSERTE(nCvt == nLen);
        
        sTo.assign(pBuf, nCvt);
    }
    
    template<>
    void ConvertStringData(const wchar_t* pFrom, size_t nSize, std::basic_string<char>& sTo) //throw (std::exception)
    {
        sTo.empty();
        
        int nLen = WideCharToMultiByte(GetACP(), 0, pFrom, nSize, 0, 0, 0, 0);
        std::vector< char > buf( nLen );
        char *pBuf = &buf[ 0 ];
        
        int nCvt = WideCharToMultiByte(GetACP(), 0, pFrom, nSize, pBuf, nLen, 0, 0);
        _ASSERTE(nCvt == nLen);
        
        sTo.assign(pBuf, nCvt);
    }
    
protected:
    // Helper to read file in either unicode or ansi
    // NOTE: We're missing formatting of error codes to text (e.g.
    //       FormatMessage(...) and incorporating these to m_sError on failure.
    bool ReadFile(const tstring& sFullPath, tstring& sContents);

protected:
    struct ArgInfo
    {
        ARG_TYPE    type;
        Argument*   pArg;

        ArgInfo(ARG_TYPE _type, Argument* _pArg);
    };

    friend class ArgInfoContainer;

    class ArgInfoContainer : public std::vector<ArgInfo>
    {
    public:
        Argument* FindFlag(const tstring& sName) const;

        Argument* GetNextParam() const;
    };

    ArgInfoContainer    m_rgArgInfos;
    tstring             m_sError;
    bool                m_bAllowArgFile:1;
};

class StandardCommandLineParser : public CommandLineParser
{
public:
    StandardCommandLineParser(bool bAllowArgFile = true);

    static
    bool IsConsole();
    void Show(const tstring& s) const;
    bool Continue();
    bool ParseAndContinue(int argc, const TCHAR* argv[]);
    bool ParseAndContinue(int argc, TCHAR* argv[]);
    bool ParseAndContinue(LPCTSTR psz);

public:
    FlagArg  help;
    FlagArg  version;
};

// Commonly used types
typedef ValueArg<tstring> StringArg;
typedef ValueArg<long> LongArg;
typedef ValueArg<double> DoubleArg;

#endif // INC_COMMANDLINEPARSER
