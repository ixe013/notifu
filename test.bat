@echo off
rem ----------------------------------------------------------------------------
rem Notifu displays a message. So in it's simplest form, the message is the only 
rem required arguments. Use the /m switch
rem ----------------------------------------------------------------------------
release\notifu /m "This is a simple Notifu message."

rem ----------------------------------------------------------------------------
rem Most users will want their own prompt (or title also). Use the /p switch
rem ----------------------------------------------------------------------------
release\notifu /m "This is a simple Notifu message." /p "Simple prompt"

rem ----------------------------------------------------------------------------
rem You probably don't want it to stay up there forever. Use the /d switch (the 
rem delay is in milliseconds, with a 250ms resolution).
rem ----------------------------------------------------------------------------
release\notifu /m "This is a simple Notifu message." /d 3000

rem ----------------------------------------------------------------------------
rem If your message is important, you can change the icon Windows will use inside 
rem the balloon with the /t switch, followed by info, warn or error.
rem ----------------------------------------------------------------------------
release\notifu /m "This is a simple Notifu message." /t warn

rem ----------------------------------------------------------------------------
rem You might want to use a different icon in the system notification area. 
rem The /i switch tries it's best to extract an icon from the path you give it. 
rem It supports using environment variables and an icon number.
rem ----------------------------------------------------------------------------
release\notifu /m "This is a simple Notifu message." /i %SYSTEMROOT%\system32\shell32.dll,43
