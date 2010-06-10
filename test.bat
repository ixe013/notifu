@echo off

setlocal 

set BUILD_CONFIGURATION=release
if not "%1" == "" set BUILD_CONFIGURATION=%1

echo Testing the %BUILD_CONFIGURATION% build.

:: ----------------------------------------------------------------------------
:: Notifu displays a message. So in it's simplest form, the message is the only 
:: required arguments. Use the /m switch
:: ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /m "This is a simple Notifu message."
call:printErrorLevel

:: ----------------------------------------------------------------------------
:: Most users will want their own prompt (or title also). Use the /p switch
:: ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /m "This is a simple Notifu message." /p "Simple prompt"
call:printErrorLevel

:: ----------------------------------------------------------------------------
:: You probably don't want it to stay up there forever. Use the /d switch (the 
:: delay is in milliseconds, with a 250ms resolution).
:: ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /m "This is a simple Notifu message with a 3 second delay." /d 3000
call:printErrorLevel

:: ----------------------------------------------------------------------------
:: If your message is important, you can change the icon Windows will use inside 
:: the balloon with the /t switch, followed by info, warn or error.
:: ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /m "This is a simple Notifu warning message." /t warn
call:printErrorLevel

:: ----------------------------------------------------------------------------
:: You might want to use a different icon in the system notification area. 
:: The /i switch tries it's best to extract an icon from the path you give it. 
:: It supports using environment variables and an icon number.
:: ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /m "This is a simple Notifu message with a custom icon." /i %SYSTEMROOT%\system32\shell32.dll,43
call:printErrorLevel

:: ----------------------------------------------------------------------------
:: Embeeded quotes are properly escaped
:: ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /m "\"Theo Est\" <test@sender.example.com> has embeeded quotes." 
call:printErrorLevel

:: ----------------------------------------------------------------------------
:: Concatenation of multiple M and P parameters
:: ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /p "Version " /p 1.3 /m "This is " /m a /m " test of " /m concatenation
call:printErrorLevel


:: ----------------------------------------------------------------------------
:: Introducing carriage return and line feed for each /n encountered
:: ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /p "Version " /p 1.4 /m "This is " /m a /m " test of\n" /m returns /m "\n" /m "and other line feeds"
call:printErrorLevel

:: ----------------------------------------------------------------------------
:: 
:: ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /p "Version " /p 1.4 /m "This is " /m a /m " test of\n" /m returns /m "\n" /m "and other line feeds"
call:printErrorLevel

endlocal

goto:eof


:printErrorLevel    - here starts my function identified by it`s label

goto error%ERRORLEVEL%

echo Unknown error %ERRORLEVEL%
goto:eof

:error0    
echo Returned eSuccess 
goto:eof

:error1    
echo Returned eBadArgs
goto:eof

:error2    
echo Returned eTimedOut
goto:eof

:error3    
echo Returned eClickedBallon
goto:eof

:error4    
echo Returned eClosedBallon
goto:eof

:error5    
echo Returned eNotSupported
goto:eof

:error6    
echo Returned eContextMenu
goto:eof

:error7    
echo Returned eSystemNotificationArea
goto:eof

:error8    
echo Returned eReplaced
goto:eof

:error255  
echo Returned eUnknown
goto:eof

goto:eof
