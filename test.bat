@echo off

setlocal 

set BUILD_CONFIGURATION=release
if not "%1" == "" set BUILD_CONFIGURATION=%1

echo Testing the %BUILD_CONFIGURATION% build.

rem ----------------------------------------------------------------------------
rem Notifu displays a message. So in it's simplest form, the message is the only 
rem required arguments. Use the /m switch
rem ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /m "This is a simple Notifu message."
echo Return value is %ERRORLEVEL%

rem ----------------------------------------------------------------------------
rem Most users will want their own prompt (or title also). Use the /p switch
rem ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /m "This is a simple Notifu message." /p "Simple prompt"
echo Return value is %ERRORLEVEL%

rem ----------------------------------------------------------------------------
rem You probably don't want it to stay up there forever. Use the /d switch (the 
rem delay is in milliseconds, with a 250ms resolution).
rem ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /m "This is a simple Notifu message with a 3 second delay." /d 3000
echo Return value is %ERRORLEVEL%

rem ----------------------------------------------------------------------------
rem If your message is important, you can change the icon Windows will use inside 
rem the balloon with the /t switch, followed by info, warn or error.
rem ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /m "This is a simple Notifu warning message." /t warn
echo Return value is %ERRORLEVEL%

rem ----------------------------------------------------------------------------
rem You might want to use a different icon in the system notification area. 
rem The /i switch tries it's best to extract an icon from the path you give it. 
rem It supports using environment variables and an icon number.
rem ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /m "This is a simple Notifu message with a custom icon." /i %SYSTEMROOT%\system32\shell32.dll,43
echo Return value is %ERRORLEVEL%

rem ----------------------------------------------------------------------------
rem Embeeded quotes are properly escaped
rem ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /m "\"Theo Est\" <test@sender.example.com> has embeeded quotes." 
echo Return value is %ERRORLEVEL%

rem ----------------------------------------------------------------------------
rem Concatenation of multiple M and P parameters
rem ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /p "Version " /p 1.3 /m "This is " /m a /m " test of " /m concatenation
echo Return value is %ERRORLEVEL%


rem ----------------------------------------------------------------------------
rem Introducing carriage return and line feed for each /n encountered
rem ----------------------------------------------------------------------------
%BUILD_CONFIGURATION%\notifu /p "Version " /p 1.4 /m "This is " /m a /m " test of\n" /m returns /m "\n" /m "and other line feeds"
echo Return value is %ERRORLEVEL%

endlocal
