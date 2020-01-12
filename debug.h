#if 0
# /*
# run make -f debug.h 


check: 
		gcc -Wall -Wextra -DFULLDEBUG -DDEBUG_INCLUDESRC -std=c9x -fsyntax-only -Werror debug.h 

debug.h.txt: debug.h.3

README.rst: debug.h
		$(file > README.rst,$(README))

manpage: debug.h.3

debug.h.3: README.rst
		rst2man README.rst > debug.h.3

html: README.rst
	pandoc -f rst -t html -s README.rst > debug.html


define README =

=========
 debug.h
=========
 
debugging functions


SYNOPSIS
========
 
 #define DEBUG 
 
 #define FULLDEBUG

 #define DEBUG_FILELEVEL [0..5]

 #define DEBUG_INCLUDESCR
 
 #include "debug.h"

**setdebugtarget(debugtarget,filename)**
	set the debug target to one of:

	- STDOUT
	- STDERR
	- TOFILE (filename)

**dbg[1..5]("message [fmt]",arg1,arg2,..)**:
  debug, when [1..5] < debug_filelevel

**dbgif[1..5]( if , "message [fmt]",arg1,arg2,..)**:
  debug, when "if" evaluates to true

**dbgf[1..5]("message [fmt]",arg1,arg2,..)**:
	debug with additional info: sourcefile, line

**warn("message [fmt]",arg1,arg2,..)**:
  Dump a warning, independent of debug switches

**warnif( if, "message [fmt]",arg1,arg2,..)**:
  Dump a warning, when "if" is true, independent of debug switches

**error("message [fmt]",arg1,arg2,..)**:
  Dump an error, independent of debug switches

**errorif( if, "message [fmt]",arg1,arg2,..)**:
  Dump an error, when "if" is true, independent of debug switches

**fatal("message [fmt]",arg1,arg2,..)**:
  Dump an error, independent of debug switches, and quit


OVERVIEW
========

Include debug.h into all sources, where you need debug macros.
In one (and only one) sourcefile define DEBUG_INCLUDESRC,
before the include.

debug (and dbg) macros can be enabled and disabled by #define DEBUG
globally.

The debugtarget can be set by the function setdebugtarget at runtime.
possible targets: STDERR, STDOUT, TOFILE

error and warning macros are not affected by the DEBUG switch.

dbg(..) calls will dump the message, 
		when debug_filelevel is not set, or set to 1..5

dbg[x] will log only, when debug_filelevel is set to any value > x


DESCRIPTION
===========
 

Hopefully uploading this to github prevents me of doing this for the X'th time,
again and again. (this file might date back to ~2003,2004.
And I was about to do this work today, again.)

The intention of this file is to have several debug macros, dbg, and dbg[2..5]
dbg messages will always be logged, except DEBUG is not defined or
debug_filelevel is set to 0
dbg2 and dbg messages will be logged, when debug is set to a value >= 2
dbg3, dbg2 and dbg messages will be logged, when debug is set to a value >= 3
.... until dbg5

When FULLDEBUG is defined, the filename and line is also dumped.

So spread dbg instructions wildly around, and simply disable them when you
are going to do a release. When bugs arise, you are able to ask for a recompile
with DEBUG enabled, and for the resulting log file.
This also means sort of transparency.
When releasing with the source code, 
the debug functions give much insight into the development process.
(I do not believe, revealing, where and why you did have to look for bugs
would show bad skills. Whoever has the skills to understand the debug
messages and debug functions spreading patterns, will most possibly
also be able to read your code. If not, it doesn't matter anyways)


My workflow is mostly related with debugging: separate a problem into as many different parts as possible,
and implement them part for part.
After every implementation of a part, check for the wished functionality, via debugging functions,
also with unexpected input.
Sometimes this can involve changing only a single source line,
and check after that.
This might seem like a slowly approach. 
But it isn't, in my experience. 
When fully in flow, I put out up to thousands of (tested) source lines
per day. The big advantage of this approach might also be the continuing
success feelings. Which keeps me flowing, in turn. Anyways.



Include this header into all sources, where you need debug macros.
In one (and only one) sourcefile define DEBUG_INCLUDESRC,
before the include.


debug (and dbg) macros can be enabled and disabled by #define DEBUG
globally.

The debugtarget can be set by the function setdebugtarget at runtime.
possible targets: STDERR, STDOUT, TOFILE

error and warning macros are not affected by the DEBUG switch.

The debuglevel (-1..5) can be set differently for every source file, 
either at runtime via dbg_filelevel = level,
and/or once by '#define DBG_FILEVEL', before the includement of debug.h
It's set to 5 per default, meaning, all dbg[1..5] calls will debug output.


LICENSE
=======

BSD License


AUTHOR
======

Michael (misc) Myer misc.myer at zoho.com


endef
ifdef UNDEF
*/
#endif


#ifndef debug_h
#define debug_h


#ifdef __cplusplus
extern "C" {
#endif

#ifndef bool
#define bool int
#define true 1
#define false 0
#endif


		// where the debug should go
		enum _debugtarget { STDERR, STDOUT, TOFILE };

		bool _setdebugtarget( enum _debugtarget t, const char *file );

		// Set a target for debug (defaults to stderr)
		// returns false, if the stream could not be opened
#define setdebugtarget(debugtarget,filename) _setdebugtarget( debugtarget, filename )



		// Error and warning severity
		enum _severity { DEBUG, MINOR, UNUSUAL, WARNING, SEVERE, FATAL};
		//enum _severity { FATAL, SEVERE, WARNING, UNUSUAL, MINOR, DEBUG };
#ifndef DBG_FILELEVEL
		// local file debug level (everything by default );
		static int dbg_filelevel = DEBUG;
#else 
		static int dbg_filelevel = DBG_FILELEVEL;
#endif


		void _warning(enum _severity sev, const char* file, const int line, const char* function, const char* fmt, ... );
		void _error( enum _severity sev, const char* file, const int line, const char* function, const char* fmt, ... );
		void _dbg_full( int level, int filelevel, const char* file, const int line, const char* function, const char* fmt, ... );
		void _dbg( int level, int filelevel, const char* fmt, ... );

#define warning(severity,...) _warning( severity, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define warn(...) _warning( WARNING, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define warnif(when,...) {if ( when ) warning(__VA_ARGS__)}

#define error(...) _error( SEVERE, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define fatal(...) _error( FATAL, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define errorif(when,...) {if ( when ) warning(__VA_ARGS__)}
#define fatalif(when,...) {if ( when ) fatal(__VA_ARGS__)}


#ifdef FULLDEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif

#ifdef DEBUG

#define dbgf(...) _dbg_full( 0, dbg_filelevel, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define dbgf1(...) _dbg_full( 1, dbg_filelevel, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define dbgf2(...) _dbg_full( 2, dbg_filelevel, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define dbgf3(...) _dbg_full( 3, dbg_filelevel, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define dbgf4(...) _dbg_full( 4, dbg_filelevel, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define dbgf5(...) _dbg_full( 5, dbg_filelevel, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )

#define dbgi(arg) _dbg(1, dbg_filelevel, "DBG: L.%d, %s, int %s:   %d", __LINE__, __FILE__, #arg , arg )

#ifdef FULLDEBUG
#define dbg(...) _dbg_full( 0, dbg_filelevel, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define dbgif(when,...) {if ( when ) dbg(__VA_ARGS__)}
#define dbg1(...) _dbg_full( 1, dbg_filelevel, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define dbg2(...) _dbg_full( 2, dbg_filelevel, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define dbg3(...) _dbg_full( 3, dbg_filelevel, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define dbg4(...) _dbg_full( 4, dbg_filelevel, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#define dbg5(...) _dbg_full( 5, dbg_filelevel, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#else
#define dbg(...) _dbg( 0, dbg_filelevel,  __VA_ARGS__ )
#define dbgif(when,...) {if ( when ) dbg(__VA_ARGS__)}
#define dbg1(...) _dbg( 1, dbg_filelevel, __VA_ARGS__ )
#define dbg2(...) _dbg( 2, dbg_filelevel, __VA_ARGS__ )
#define dbg3(...) _dbg( 3, dbg_filelevel, __VA_ARGS__ )
#define dbg4(...) _dbg( 4, dbg_filelevel, __VA_ARGS__ )
#define dbg5(...) _dbg( 5, dbg_filelevel, __VA_ARGS__ )
#endif //FULLDEBUG

#else //DEBUG not defined
#define dbg(...) {}
#define dbgif(...) {}
#define dbg1(...) {}
#define dbg2(...) {}
#define dbg3(...) {}
#define dbg4(...) {}
#define dbg5(...) {}
#define dbgf(...) {}
#define dbgf1(...) {}
#define dbgf2(...) {}
#define dbgf3(...) {}
#define dbgf4(...) {}
#define dbgf5(...) {}

#define dbgi(_int) {}

#endif //DEBUG




#ifdef __cplusplus
}
#endif


#ifdef DEBUG_INCLUDESRC

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

// Implementation

static FILE* target;

static void init(){
		if ( !target )
				target = stderr;
}

bool _setdebugtarget( enum _debugtarget t , const char * file)
{
		switch(t){
				case STDOUT: target = stdout;
										 break;

				case TOFILE: target = fopen(file,"w");
										 if ( ferror( target ) ){
												 target = stderr;
												 warn("Couldn't open debug target: %s", file);
												 return(false);
										 }
										 break;
				default: 
										 target = stderr;
										 break;

		}

		return(true);
}



void _warning( enum _severity sev, const char* file, const int line, const char* function, const char*fmt, ... ){
		init();
		fprintf(target, "WARNING: %s   %d, in %s\n   ", file, line, function);
		va_list ap;
		va_start(ap,fmt);
		vfprintf(target, fmt, ap );
		fprintf(target,"\n");
		va_end(ap);
		fflush(target);
}
void _error( enum _severity sev, const char* file, const int line, const char* function, const char*fmt, ... ){
		init();
		fprintf(target, "ERROR: %s   %d, in %s    XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n", file, line, function);
		va_list ap;
		va_start(ap,fmt);
		vfprintf(target, fmt, ap );
		fprintf(target,"\n");
		va_end(ap);
		fflush(target);
		if (sev==FATAL){
				exit(1);
		}
}
void _dbg_full( int level, int filelevel, const char* file, const int line, const char* function, const char*fmt, ... ){
		if ( level > filelevel )
				return;
		init();
		fprintf(target, "   %s   %d, in %s\n", file, line, function );
		va_list ap;
		va_start(ap,fmt);
		vfprintf(target, fmt,  ap );
		fprintf(target,"\n");
		va_end(ap);
		fflush(target);
}
void _dbg( int level, int filelevel, const char*fmt, ... ){
		if ( level > filelevel )
				return;
		init();
		va_list ap;
		va_start(ap,fmt);
		vfprintf(target, fmt,  ap );
		fprintf(target,"\n");
		va_end(ap);
		fflush(target);
}




#endif //DEBUG_INCLUDESRC

#endif //IFNDEF debug_h

#if 0
endif
#endif
