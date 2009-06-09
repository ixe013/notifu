#include "stdafx.h"
#include "CommandLineParser.h"

#include <shellapi.h>

// Unicode console output
#ifndef tcout
#ifdef _UNICODE
std::wostream& tcout = std::wcout;
#else
std::ostream& tcout = std::cout;
#endif
#endif


template <>
bool ReadFromString(const tstring& s, tstring* pValue)
{
    // We need this specialization so that we the
    // entire string, including spaces. Otherwise,
    // operator>>(istream, s) will just take up to
    // the first space.
    *pValue = s;
    return true;
}


	Argument::Argument(const tstring& sName, const tstring& sDescription)
    :   m_bFound(false),
        m_bCaseSensitive(false),
        m_bDocumented(true),
        m_sDescription(sDescription)
    {
        _ASSERTE(!sName.empty());
        AddAlternate(sName);
    }

    Argument::~Argument()
    {
    }

    void Argument::AddAlternate(const tstring& sName)
    {
        _ASSERTE(!Matches(sName));
        m_rgNames.push_back(sName);
    }

    tstring Argument::Name() const
    {
        _ASSERTE(!m_rgNames.empty());
        return m_rgNames[0];
    }

    tstring Argument::sDescription() const
    {
        return m_sDescription;
    }

    bool Argument::Found() const
    {
        return m_bFound;
    }

    void Argument::SetFound(bool bFound)
    {
        m_bFound = bFound;
    }

    void Argument::SetDocumented(bool bDocumented)
    {
        m_bDocumented = bDocumented;
    }

    bool Argument::Matches(const tstring& sName) const
    {
        // Choose comparitor based on case sensitivity needs
        int (*cmp)(LPCTSTR, LPCTSTR) = (m_bCaseSensitive ? _tcscmp : _tcsicmp);

        for( std::vector<tstring>::const_iterator it = m_rgNames.begin(); it != m_rgNames.end(); ++it )
        {
            if( cmp(it->c_str(), sName.c_str()) == 0 ) return true;
        }

        return false;
    }

    void Argument::SetCaseSensitive(bool bCaseSensitive)
    {
        m_bCaseSensitive = bCaseSensitive;
    }

    tstring Argument::UsageHelper(const tstring& sPrefix, bool bOptional, const tstring& sValueName) const
    {
        tstring sUsage;
        if( bOptional ) sUsage += __T("[");
        sUsage += sPrefix;

        // If there's no prefix, assume a param instead of a flag,
        // and wrap the name (great for names with spaces)
        bool    bWrapName = sPrefix.size() == 0;
        if( bWrapName && !bOptional ) sUsage += __T("<");

        tstring sPipe;
        for( std::vector<tstring>::const_iterator it = m_rgNames.begin(); it != m_rgNames.end(); ++it )
        {
            sUsage += sPipe + *it;
            sPipe = __T("|");
        }

        if( !sValueName.empty() ) sUsage += __T(" <") + sValueName + __T(">");
        if( bWrapName && !bOptional ) sUsage += __T(">");
        if( bOptional ) sUsage += __T("]");
        return sUsage;
    }

    tstring Argument::LongUsageHelper(const tstring& sPrefix, const tstring& sValueName) const
    {
        if( m_sDescription.empty() ) return __T("");

        bool    bFlag = !sPrefix.empty();
        tstring sUsage;
        sUsage += sPrefix;
        sUsage += Name();
        if( bFlag && !sValueName.empty() ) sUsage += __T(" <") + sValueName + __T(">");

        // Line all descriptions up on two tabs
        if( sUsage.size() < 8 ) sUsage += __T('\t');
        sUsage += __T('\t');
        sUsage += m_sDescription;

        return sUsage;
    }

	FlagArg::FlagArg(const tstring& sName, const tstring& sDescription)
    :   Argument(sName, sDescription)
    {
    }

    bool FlagArg::IsValid() const
    {
        return true;
    }

    bool FlagArg::ExpectsValues() const
    {
        return false;
    }

    bool FlagArg::WantsAValue() const
    {
        _ASSERTE(false && "FlagArgs don't consume values");
        return false;
    }

    bool FlagArg::ConsumeValue(const tstring& sValue)
    {
        _ASSERTE(false && "FlagArgs don't consume values");
        return false;
    }

    tstring FlagArg::Usage(const tstring sPrefix) const
    {
        return UsageHelper(sPrefix, true, __T(""));
    }

    tstring FlagArg::LongUsage(const tstring sPrefix) const
    {
        return LongUsageHelper(sPrefix);
    }

    FlagArg::operator bool() const
    {
        return Found();
    }

/*
	template <typename T>
	ValueArg<T>::ValueArg(const tstring& sName, const tstring& sDescription, const tstring& sValueName)
    :   Argument(sName, sDescription), m_sValueName(sValueName), m_bHasDefault(false)
    {
    }

	template <typename T>
    void ValueArg<T>::SetDefaultValue(const T& value)
    {
        m_value = value;
        m_valueDefault = value;
        m_bHasDefault = true;
    }

	template <typename T>
    bool ValueArg<T>::IsValid() const
    {
        return m_bHasDefault || Found();
    }

	template <typename T>
    bool ValueArg<T>::ExpectsValues() const
    {
        return true;
    }

	template <typename T>
    bool ValueArg<T>::WantsAValue() const
    {
        return !Found();
    }

	template <typename T>
    bool ValueArg<T>::ConsumeValue(const tstring& s)
    {
        return ReadFromString(s, &m_value);
    }

	template <typename T>
    tstring ValueArg<T>::Usage(const tstring sPrefix) const
    {
        bool    bFlag = sPrefix.size() != 0;
        return UsageHelper(sPrefix, m_bHasDefault, bFlag ? m_sValueName : __T(""));
    }

	template <typename T>
    tstring ValueArg<T>::LongUsage(const tstring sPrefix) const
    {
        return LongUsageHelper(sPrefix, m_sValueName);
    }

	template <typename T>
    ValueArg<T>::operator const T&() const
    {
        return m_value;
    }

	template <typename T>
    const T& ValueArg<T>::Value() const
    {
        return m_value;
    }


	template <typename T>
	RestrictedValueArg<T>::RestrictedValueArg(const tstring& sName, const tstring& sDescription, const tstring& sValueName)
    :   ValueArg<T>(sName, sDescription, sValueName),
        m_bRestrictedValueDescriptions(false)
    {
    }

	template <typename T>
    bool RestrictedValueArg<T>::IsValid() const
    {
        RestrictedValues::const_iterator it;
        const RestrictedValues::const_iterator last = m_rgValues.end();
        for( it = m_rgValues.begin(); it != last; ++it )
        {
            if( it->first == m_value ) return true;
        }

        return false;
    }

	template <typename T>
    tstring RestrictedValueArg<T>::Usage(const tstring& sPrefix) const
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

	template <typename T>
    tstring RestrictedValueArg<T>::LongUsage(const tstring sPrefix) const
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

	template <typename T>
    void RestrictedValueArg<T>::AddValue(const T& value, const tstring& sDescription)
    {
        if( sDescription.empty() ) m_bRestrictedValueDescriptions = true;
        m_rgValues.push_back(RestrictedValue(value, sDescription));
    }


	template <typename T, size_t nMinValues, size_t nMaxValues>
	MultiValueArg<T,nMinValues,nMaxValues>::MultiValueArg(const tstring& sName, const tstring& sDescription, const tstring& sValueName)
    :   Argument(sName, sDescription), m_sValueName(sValueName)
    {
    }

	template <typename T, size_t nMinValues, size_t nMaxValues>
    bool MultiValueArg<T,nMinValues,nMaxValues>::IsValid() const
    {
        return m_rgValues.size() >= nMinValues;
    }

	template <typename T, size_t nMinValues, size_t nMaxValues>
    bool MultiValueArg<T,nMinValues,nMaxValues>::ExpectsValues() const
    {
        return true;
    }

	template <typename T, size_t nMinValues, size_t nMaxValues>
    bool MultiValueArg<T,nMinValues,nMaxValues>::WantsAValue() const
    {
        return m_rgValues.size() < nMaxValues;
    }

	template <typename T, size_t nMinValues, size_t nMaxValues>
    bool MultiValueArg<T,nMinValues,nMaxValues>::ConsumeValue(const tstring& s)
    {
        T       value;
        bool    b = ReadFromString(s, &value);
        if( !b ) return b;

        m_rgValues.push_back(value);
        return true;
    }

	template <typename T, size_t nMinValues, size_t nMaxValues>
    tstring MultiValueArg<T,nMinValues,nMaxValues>::Usage(const tstring sPrefix) const
    {
        bool    bFlag = sPrefix.size() != 0;
        return UsageHelper(sPrefix, true, bFlag ? __T("value") : __T("")) + __T("...");
    }

	template <typename T, size_t nMinValues, size_t nMaxValues>
    tstring MultiValueArg<T,nMinValues,nMaxValues>::LongUsage(const tstring sPrefix) const
    {
        return LongUsageHelper(sPrefix, m_sValueName);
    }

	template <typename T, size_t nMinValues, size_t nMaxValues>
    size_t MultiValueArg<T,nMinValues,nMaxValues>::size() const
    {
        return m_rgValues.size();
    }

	template <typename T, size_t nMinValues, size_t nMaxValues>
    MultiValueArg<T,nMinValues,nMaxValues>::operator const std::vector<T>&() const
    {
        return m_rgValues;
    }

	template <typename T, size_t nMinValues, size_t nMaxValues>
    const std::vector<T>& MultiValueArg<T,nMinValues,nMaxValues>::Values() const
    {
        return m_rgValues;
    }

	template <typename T, size_t nMinValues, size_t nMaxValues>
    const T& MultiValueArg<T,nMinValues,nMaxValues>::operator[](size_t i) const
    {
        _ASSERTE(i < m_rgValues.size());
        return m_rgValues[i];
    }

	template <typename T, size_t nMinValues, size_t nMaxValues>
    const T& MultiValueArg<T,nMinValues,nMaxValues>::Value(size_t i) const
    {
        _ASSERTE(i < m_rgValues.size());
        return m_rgValues[i];
    }
//*/

	FileNameValue::FileNameValue() 
	{
	}

	FileNameValue::FileNameValue(const tstring& sFileName) 
	: tstring(sFileName) 
	{
	}

	FileNameValue::FileNameValue(LPCTSTR pszFileName) 
	: tstring(pszFileName) 
	{
	}

    bool FileNameValue::Exists() const
    {
        DWORD dwAttrib = GetFileAttributes(c_str());
        return (dwAttrib != -1) && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
    }


bool ReadFromString(const tstring& s, FileNameValue* pValue)
{
    _ASSERTE(pValue);

    // Resolve to full path name at current working directory
    TCHAR   szPath[MAX_PATH+1];
    GetFullPathName(s.c_str(), MAX_PATH, szPath, 0);

    pValue->assign(szPath);
    return true;
}

    tstring PairValue::Left() const
    {
        return m_sLeft;
    }

    tstring PairValue::Right() const
    {
        return m_sRight;
    }


bool ReadFromString(const tstring& s, PairValue* pValue)
{
    _ASSERTE(pValue);

    // Split from left=right to left and right
    size_t  equals = s.find(__T('='));
    pValue->m_sLeft = s.substr(0, equals);
    if( equals != tstring::npos ) pValue->m_sRight = s.substr(equals + 1);
    else pValue->m_sRight.erase();

    return true;
}

	CommandLineParser::CommandLineParser(bool bAllowArgFile)
        : m_bAllowArgFile(bAllowArgFile)
    {
    }

    void CommandLineParser::AddFlag(Argument& arg)
    {
        _ASSERTE(!m_rgArgInfos.FindFlag(arg.Name()) && "Can't add two flags with the same name");
        m_rgArgInfos.push_back(ArgInfo(ARG_FLAG, &arg));
    }

    void CommandLineParser::AddParam(Argument& arg)
    {
        m_rgArgInfos.push_back(ArgInfo(ARG_PARAM, &arg));
    }

    tstring CommandLineParser::Usage(const tstring& sErr) const
    {
        tstringstream   ssUsage;

        // Logo
        tstring sLogo = Logo();
        if( sLogo.size() )
        {
            ssUsage << Logo() << std::endl;
        }

        // Error string
        if( !sErr.empty() )
        {
            ssUsage << sErr << std::endl << std::endl;
        }
        else if( HasErrors() )
        {
            ssUsage << m_sError << std::endl << std::endl;
        }

        // Short (name and default value only)
        ssUsage << __T("Usage: ") << ModuleName();

        if( m_bAllowArgFile ) ssUsage << __T(" ") << __T("[@argfile]");

		ArgInfoContainer::const_iterator it;

        for(it = m_rgArgInfos.begin(); it != m_rgArgInfos.end(); ++it )
        {
            _ASSERTE((it->type == ARG_FLAG) || (it->type == ARG_PARAM));
            _ASSERTE(it->pArg);
            ssUsage << __T(" ") << it->pArg->Usage(it->type == ARG_FLAG ? __T("/") : __T(""));
        }

        ssUsage << std::endl;
        ssUsage << std::endl;

        // Long (name and sDescription)
        if( m_bAllowArgFile ) ssUsage << __T("@argfile\tRead arguments from a file.") << std::endl;

        for( it = m_rgArgInfos.begin(); it != m_rgArgInfos.end(); ++it )
        {
            _ASSERTE((it->type == ARG_FLAG) || (it->type == ARG_PARAM));
            _ASSERTE(it->pArg);
            tstring sLongUsage = it->pArg->LongUsage(it->type == ARG_FLAG ? __T("/") : __T(""));
            if( sLongUsage.size() ) ssUsage << sLongUsage << std::endl;
        }

        return ssUsage.str();
    }

    bool CommandLineParser::Parse(int argc, const TCHAR** argv, bool bValidate, bool bLeadingExe)
    {
        std::vector<tstring> Params;
        for(int i = (bLeadingExe ? 1 : 0); i < argc; ++i) Params.push_back(argv[i]);
        return Parse(Params, bValidate);
    }

    bool CommandLineParser::Parse(const tstring & CommandLine, bool bValidate, bool bLeadingExe)
    {
        std::vector<tstring> Params;
        tstring CurParam;

        bool bInQuotes = false;

        for(tstring::const_iterator i = CommandLine.begin(); i != CommandLine.end(); ++i)
        {
            if (*i == __T('"'))
            {
				bInQuotes = !bInQuotes;
				continue;
            }
			else if (*i == __T('\\'))
            {
				if(bInQuotes && (*(i+1) == __T('"')))
					++i;
            }

            if (IsWhiteSpace(*i) && !bInQuotes)
            {
                if (!CurParam.empty())
                {
                    if( bLeadingExe ) bLeadingExe = false;
                    else Params.push_back(CurParam);
                    CurParam = __T("");
                }
                continue;
            }

            CurParam += *i;
        }

        // Check for bLeadingExe to avoid pushing module file
        // name if no parameters were supplied.
        if( !CurParam.empty() && !bLeadingExe )
        {
            Params.push_back(CurParam);
        }

        return Parse(Params, bValidate);
    }

    // Assumes exe name *not* in the first in the list
    bool CommandLineParser::Parse(const std::vector<tstring>& rgsArgs, bool bValidate)
    {
        ClearError();

        for(std::vector<tstring>::const_iterator psArg = rgsArgs.begin(); psArg != rgsArgs.end(); ++psArg)
        {
            Argument*   pArg = 0;
            bool        bIsFlag = false;

            // It's a flag
            if( (psArg->size() > 1) && (psArg->at(0) == __T('/') || psArg->at(0) == __T('-')) )
            {
                // Find the argument by name
                pArg = m_rgArgInfos.FindFlag(psArg->substr(1));
                bIsFlag = true;
            }
            // It's a file name to process parameters from
            else if( psArg->size() > 1 && psArg->at(0) == __T('@') && m_bAllowArgFile )
            {
                if( !ParseFromFile(psArg->substr(1)) ) return false;
                continue;
            }
            // It's a parameter
            else
            {
                // Find the argument by offset
                pArg = m_rgArgInfos.GetNextParam();
            }
        
            if( !pArg )
            {
                m_sError = __T("Unrecognized argument '") + *psArg + __T("'.");
                return false;
            }

            _ASSERTE(pArg);

            // Argument with a value, e.g. /foo bar
            if( pArg->ExpectsValues() )
            {
                if( bIsFlag && (++psArg == rgsArgs.end()) )
                {
                    m_sError = __T("Argument '") + pArg->Name() + __T("' expects a parameter.");
                    return false;
                }
                
                if( !pArg->WantsAValue() )
                {
                    m_sError = __T("Argument '") + pArg->Name() + __T("' cannot accept another parameter: '") + *psArg + __T("'.");
                    return false;
                }
                
                if( !pArg->ConsumeValue(*psArg) )
                {
                    m_sError = __T("Argument '") + pArg->Name() + __T("' cannot accept parameter: '") + *psArg + __T("'.");
                    return false;
                }
            }
            // Argument w/o a value, e.g. /foo
            else
            {
                if( pArg->Found() )
                {
                    m_sError = __T("Argument '") + pArg->Name() + __T("' already present.");
                    return false;
                }
            }
            
            pArg->SetFound(true);
        }
        
        // Check for missing required arguments
        for( ArgInfoContainer::iterator it = m_rgArgInfos.begin(); it != m_rgArgInfos.end(); ++it )
        {
            _ASSERTE(it->pArg);

            if( !it->pArg->IsValid() )
            {
                m_sError = __T("Expected argument '") + it->pArg->Name() + __T("'.");
                return false;
            }
        }

        return true;
    }

    bool CommandLineParser::ParseFromFile(const tstring& sFileName)
    {
        _ASSERT(sFileName.size());

        // Check if file exists
        DWORD   dwAttrib = GetFileAttributes(sFileName.c_str());
        bool    bFileExists = (dwAttrib != -1) && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);

        if( !bFileExists )
        {
            m_sError = __T("");
            m_sError += __T("File not found: ");
            m_sError += sFileName;
            return false;
        }

        // Point current directory at the input file
        TCHAR   szFileDir[MAX_PATH+1];
        LPTSTR  pszFilePart = 0;
        GetFullPathName(sFileName.c_str(), MAX_PATH, szFileDir, &pszFilePart);
        tstring sFullPath = szFileDir;
        _ASSERTE(pszFilePart);
        *pszFilePart = 0;

        // Set (and auto-reset) current working directory
        // We do this so that any file names read out of the file
        // can be relative to the file, not where we're running
        // the app from. We're using the system managed working directory
        // as a "context" for the individual values, i.e. FileNameValue,
        // to be able to compute a complete file name. This is potentially
        // dangerous as the cwd is set per process, not per thread, but
        // since command lines are typically processed before threads are
        // fired off, we should be safe. It saves us from having to pass
        // a virtual cwd to all values as they're parsed.
        class CurrentDir
        {
        public:
            CurrentDir(LPCTSTR pszNewDir)
            {
                GetCurrentDirectory(MAX_PATH, m_szOldDir);
                _ASSERT(pszNewDir);
                BOOL    b = SetCurrentDirectory(pszNewDir);
                _ASSERTE(b);
            }

            ~CurrentDir()
            {
                SetCurrentDirectory(m_szOldDir);
            }

        private:
            TCHAR   m_szOldDir[MAX_PATH+1];
        };

        CurrentDir  cd(szFileDir);

        // Read in the contents of the file
        tstring tContents;
        if (!ReadFile(sFullPath, tContents)) return false;

        // Parse the contents of the file like a string
        return Parse(tContents, false, false);
    }

    bool CommandLineParser::HasErrors() const
    {
        return m_sError.size() > 0;
    }

    void CommandLineParser::ClearError()
    {
        m_sError.erase();
    }

    tstring CommandLineParser::Logo() const
    {
        CVersionInfo        vi;
        if( FAILED(vi.LoadVersion()) ) return __T("");

        LPCTSTR pszDesc = vi.QueryValueString(__T("FileDescription"));
        LPCTSTR pszVersion = vi.QueryValueString(__T("FileVersion"));
        LPCTSTR pszCopyright = vi.QueryValueString(__T("LegalCopyright"));

        tstring         sDesc = (pszDesc ? pszDesc : __T(""));
        tstring         sVersion = (pszVersion ? pszVersion : __T(""));
        tstring         sCopyright = (pszCopyright ? pszCopyright : __T(""));
        tstringstream   ss;

        sCopyright = SearchAndReplace(sCopyright, __T("\x0a9"), __T("(c)"));
        sCopyright = SearchAndReplace(sCopyright, __T("\x0ae"), __T("(r)"));

        if( sCopyright[sCopyright.size() - 1] != __T('.') ) sCopyright += __T('.');

        // e.g.
        // Microsoft MIDL Compiler Version 5.01.0164
        // Copyright (c) 1991-1997, Microsoft, Inc. All rights reserved.
        ss  << sDesc << __T(" version ") << sVersion << std::endl
            << sCopyright << __T(" BSD Licensed.") << std::endl;

        return ss.str();
    }

    bool CommandLineParser::FileExists(const tstring& sFileName) 
    { 
        DWORD dwAttrib = GetFileAttributes(sFileName.c_str());
        return (dwAttrib != -1) && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
    }

    tstring CommandLineParser::ModuleName()
    {
        TCHAR   szPath[MAX_PATH+1];
        GetModuleFileName(0, szPath, MAX_PATH);
        TCHAR   szFile[_MAX_FNAME+1];
        _tsplitpath(szPath, 0, 0, szFile, 0);
        return szFile;
    }

    bool CommandLineParser::IsWhiteSpace(TCHAR ch)
    {
        return ch == __T(' ') || ch == __T('\t') || ch == __T('\n') || ch == __T('\r');
    }

    tstring CommandLineParser::SearchAndReplace(
        tstring         s,
        const tstring&  sFind,
        const tstring&  sReplace,
        bool            bGlobal)
    {
        _ASSERTE(sFind.size());
        const size_t    cchFind = sFind.size();

        size_t pos = 0;
        while( (pos = s.find(sFind, pos)) != tstring::npos )
        {
            s.replace(pos, cchFind, sReplace);
            if( !bGlobal ) break;
            pos += cchFind;
        }

        return s;
    }
/*
    template<typename invalid_t, invalid_t invalid_value>
		CommandLineParser::SimpleHandleT<invalid_t,invalid_value>::SimpleHandleT(const HANDLE& h) : m_h(h) {}

	template<typename invalid_t, invalid_t invalid_value>
		CommandLineParser::SimpleHandleT<invalid_t,invalid_value>::~SimpleHandleT()
        {
            if( invalid_value != reinterpret_cast<invalid_t>(m_h) )
            {
                ::CloseHandle(m_h);
            }
        }
    
	template<typename invalid_t, invalid_t invalid_value>
        bool CommandLineParser::SimpleHandleT<invalid_t,invalid_value>::operator!() const
        {
            return (invalid_value == reinterpret_cast<invalid_t>(m_h));
        }
    
	template<typename invalid_t, invalid_t invalid_value>
        CommandLineParser::SimpleHandleT<invalid_t,invalid_value>::operator HANDLE() const
        { return m_h; }

    //template<class From, class To>
    //void CommandLineParser::ConvertStringData(const From* pFrom, size_t nSize, std::basic_string<To>& sTo) //throw (std::exception)
    //{ sTo.assign(pFrom, nSize); }

    template<>
    void CommandLineParser::ConvertStringData(const char* pFrom, size_t nSize, std::basic_string<wchar_t>& sTo) //throw (std::exception)
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
    void CommandLineParser::ConvertStringData(const wchar_t* pFrom, size_t nSize, std::basic_string<char>& sTo) //throw (std::exception)
    {
        sTo.empty();
        
        int nLen = WideCharToMultiByte(GetACP(), 0, pFrom, nSize, 0, 0, 0, 0);
        std::vector< char > buf( nLen );
        char *pBuf = &buf[ 0 ];
        
        int nCvt = WideCharToMultiByte(GetACP(), 0, pFrom, nSize, pBuf, nLen, 0, 0);
        _ASSERTE(nCvt == nLen);
        
        sTo.assign(pBuf, nCvt);
    }
//*/    
    bool CommandLineParser::ReadFile(const tstring& sFullPath, tstring& sContents)
    {
        // Clean up
        ClearError();
        sContents = L"";
    
        FileHandle hFile = CreateFile(sFullPath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    
        if( !hFile )
        {
            _ASSERTE(!"Failed to open parameter file");
            m_sError = L"Failed to open file ";
            m_sError += sFullPath;
        
            return false;
        }
    
        BY_HANDLE_FILE_INFORMATION bhfi;
        ::ZeroMemory(&bhfi, sizeof(bhfi));
    
        if( GetFileInformationByHandle(hFile, &bhfi) == 0 )
        {
            _ASSERTE(!"Failed to get file information");
            m_sError = L"Failed to get file information for ";
            m_sError += sFullPath;
        
            return false;
        }
    
        // A few special cases
        if( bhfi.nFileSizeHigh != 0 )
        {
            _ASSERTE(!"Ridiculously large parameter file");
            m_sError = L"File ";
            m_sError += sFullPath;
            m_sError += L" is too large";
        
            return false;
        }
        else if( bhfi.nFileSizeLow == 0 )
        {
            // Empty file (but should be accepted anyway)
            return true;
        }
    
        size_t nDataSize = bhfi.nFileSizeLow;
    
        Handle hFileMapping = CreateFileMapping(hFile, 0, PAGE_WRITECOPY, 0, nDataSize, 0);
        if( !hFileMapping )
        {
            _ASSERTE(!"Failed to create file mapping");
            m_sError = L"Failed to create file mapping on file ";
            m_sError += sFullPath;
        
            return false;
        }
    
        LPVOID lpView = MapViewOfFile(hFileMapping, FILE_MAP_COPY, 0, 0, 0);
    
        if( !lpView )
        {
            _ASSERTE(!"Failed to map view of file");
            m_sError = L"Failed to map view of file ";
            m_sError += sFullPath;
        
            return false;
        }
    
        class MapViewCleanup
        {
        public:
            explicit MapViewCleanup(LPVOID lpv)
                : m_lpView(lpv)
            {}

			~MapViewCleanup()
            { ::UnmapViewOfFile(m_lpView); }
        
        private:
            LPVOID m_lpView;
        } mapCleanup(lpView);
    
        // Try to determine if unicode
        LPVOID  lpData = lpView;
        bool    bUnicode = false;
    
        if( nDataSize > sizeof(WORD) )
        {
            WORD wMaybeUnicodeMarker = *(reinterpret_cast<LPWORD>(lpView));
        
            if( wMaybeUnicodeMarker == 0xFEFF )
            {
                _ASSERTE(0 == (nDataSize%sizeof(wchar_t)));
                lpData = &(reinterpret_cast<LPWORD>(lpView)[1]);
                bUnicode = true;
            }
            else if( wMaybeUnicodeMarker == 0xFFFE )
            {
                _ASSERTE(!"Big-endian unicode not supported");
                m_sError = L"File ";
                m_sError += sFullPath;
                m_sError += L" is in big-endian unicode format";
            
                return false;
            }
        }
    
        // Convert as necessary
        try
        {
            if( bUnicode ) 
				ConvertStringData(reinterpret_cast<LPCWSTR>(lpData), (nDataSize/sizeof(wchar_t))-1, sContents);
            else 
				ConvertStringData(reinterpret_cast<LPCSTR>(lpData), nDataSize/sizeof(char), sContents);
        }
        catch( std::exception& e )
        {
            _ASSERTE(e.what());
            e;  // Avoid "unreferenced local variable"
        
            m_sError = L"Failed to convert character data from file ";
            m_sError += sFullPath;
            m_sError += L".";
        
            return false;
        }
    
        // Replace CRs & LFs
        std::replace_if(sContents.begin(),
                        sContents.end(),
                        std::bind2nd(std::equal_to<TCHAR>(), '\r'),
                        ' ');
        std::replace_if(sContents.begin(),
                        sContents.end(),
                        std::bind2nd(std::equal_to<TCHAR>(), L'\n'),
                        L' ');
    
        return true;
    }

	CommandLineParser::ArgInfo::ArgInfo(ARG_TYPE _type, Argument* _pArg) : type(_type), pArg(_pArg) {}

		Argument* CommandLineParser::ArgInfoContainer::FindFlag(const tstring& sName) const
        {
            for( const_iterator it = begin(); it != end(); ++it )
            {
                _ASSERTE(it->pArg);
                if( (it->type == ARG_FLAG) &&
                    (it->pArg->Matches(sName)) )
                {
                    return it->pArg;
                }
            }

            return 0;
        }

        Argument* CommandLineParser::ArgInfoContainer::GetNextParam() const
        {
            // Give each argument all the values it wants
            // before moving onto the next one
            for( const_iterator it = begin(); it != end(); ++it )
            {
                if( it->type == ARG_PARAM )
                {
                    _ASSERTE(it->pArg);
                    Argument*   pArg = it->pArg;
                    if( pArg->ExpectsValues() && pArg->WantsAValue() )
                    {
                        return pArg;
                    }
                }
            }

            return 0;
        }

	StandardCommandLineParser::StandardCommandLineParser(bool bAllowArgFile)
    :   CommandLineParser(bAllowArgFile),
        help(__T("?"), __T("Show usage.")),
        version(__T("v"), __T("Show version."))
    {
        AddFlag(help);
        help.AddAlternate(__T("h"));
        help.AddAlternate(__T("help"));

        AddFlag(version);
        version.AddAlternate(__T("version"));
    }

    bool StandardCommandLineParser::IsConsole()
    {
        TCHAR       szPath[MAX_PATH+1];
        GetModuleFileName(0, szPath, MAX_PATH);

        SHFILEINFO  sfi = { 0 };
        DWORD       dw = SHGetFileInfo(szPath, 0, &sfi, sizeof(sfi), SHGFI_EXETYPE);

        return (LOWORD(dw) == IMAGE_NT_SIGNATURE) && (HIWORD(dw) == 0);
    }

    void StandardCommandLineParser::Show(const tstring& s) const
    {
        if( IsConsole() )
        {
            // Always send usage to stdout so it's easy to capture the output
            tcout << s.c_str() << std::endl;
        }
        else
        {
            MessageBox(0, s.c_str(), ModuleName().c_str(), MB_SETFOREGROUND);
        }
    }

    bool StandardCommandLineParser::Continue()
    {
        if( version )
        {
            Show(Logo());
            return false;
        }

        if( HasErrors() || help )
        {
            if( help ) ClearError();
            Show(Usage());
            return false;
        }

        return true;
    }

    bool StandardCommandLineParser::ParseAndContinue(int argc, const TCHAR* argv[])
    {
        Parse(argc, argv);
        return Continue();
    }

    bool StandardCommandLineParser::ParseAndContinue(int argc, TCHAR* argv[])
    {
        // To avoid C2664
        Parse(argc, (const TCHAR**)argv);
        return Continue();
    }

    bool StandardCommandLineParser::ParseAndContinue(LPCTSTR psz)
    {
        Parse(psz);
        return Continue();
    }

