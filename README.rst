
=========
 debug.h
=========
 
debugging functions, coloured, file logging, single file header only 


SYNOPSIS
========
 
 #define DEBUG 
 
 #define FULLDEBUG

 #define DEBUG_FILELEVEL [0..5]

 #define DEBUG_INCLUDESRC
 
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
and/or once by '#define DEBUG_FILEVEL', before the includement of debug.h
It's set to 5 per default, meaning, all dbg[1..5] calls will debug output.


Generating documentation
========================


make -f debug.h [target]


LICENSE
=======

BSD 3-clause


AUTHOR
======

Michael (misc) Myer misc.myer at zoho.com

