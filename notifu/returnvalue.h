#ifndef __RETURNVALUE_H__
#define __RETURNVALUE_H__

enum
{
	//I know the compiler numbers enums, but I like source that
	//is its own documentation...
	eSuccess = 0, /// The command line was understood and executed properly
	eBadArgs = 1, /// Some parameters were bad or missing.
	eTimedOut = 2, /// Implies eSuccess, but a timeout was requested and the ballon was dismissed because of that
	eClickedBallon = 3, /// Implies eSuccess, but the user closed the ballon to close it, ending the program
	eClosedBallon = 4, /// Implies eSuccess, but the user closed the upper right corner of the ballon to close it, ending the program
	eNotSupported = 5, /// IUserNotification is not supported on this system
	eContextMenu = 6, /// IUserNotification is not supported on this system
	eSystemNotificationArea = 7, /// IUserNotification is not supported on this system
	eReplaced = 8, /// A new instance of Notifu dismissed a running instace
	eUnknown = 255
};

#endif
