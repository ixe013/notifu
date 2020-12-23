// Copyright (c) 2019, Solutions Paralint inc.                                     
// All rights reserved.                                                            
//                                                                                 
// Redistribution and use in source and binary forms, with or without              
// modification, are permitted provided that the following conditions are met:     
//     * Redistributions of source code must retain the above copyright            
//       notice, this list of conditions and the following disclaimer.             
//     * Redistributions in binary form must reproduce the above copyright         
//       notice, this list of conditions and the following disclaimer in the       
//       documentation and/or other materials provided with the distribution.      
//     * Neither the name of the <organization> nor the                            
//       names of its contributors may be used to endorse or promote products      
//       derived from this software without specific prior written permission.     
//                                                                                 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED   
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY              
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES      
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;    
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND     
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT      
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS   
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                    
//                                                                                 
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
	eGracefullyKilled = 9, /// A new instance of Notifu killed all running instaces
	eUnknown = 255
};

#endif
