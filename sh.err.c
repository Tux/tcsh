/* $Header$ */
/*
 * sh.err.c: Error printing routines. 
 */
/*-
 * Copyright (c) 1980, 1991 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#define _h_tc_err		/* Don't redefine the errors	 */
#include "sh.h"

RCSID("$Id$")

/*
 * C Shell
 */

char   *seterr = NULL;	/* Holds last error if there was one */

#define ERR_FLAGS	0xf0000000
#define ERR_NAME	0x10000000
#define ERR_SILENT	0x20000000
#define ERR_OLD		0x40000000

static char *errorlist[] = 
{
#define ERR_SYNTAX	0
    "Syntax Error",
#define ERR_NOTALLOWED	1
    "%s is not allowed",
#define ERR_WTOOLONG	2
    "Word too long",
#define ERR_LTOOLONG	3
    "$< line too long",
#define ERR_DOLZERO	4
    "No file for $0",
#define ERR_DOLQUEST	5
    "$? not allowed here",
#define ERR_INCBR	6
    "Incomplete [] modifier",
#define ERR_EXPORD	7
    "$ expansion must end before ]",
#define ERR_BADMOD	8
    "Bad : modifier in $ (%c)",
#define ERR_SUBSCRIPT	9
    "Subscript error",
#define ERR_BADNUM	10
    "Badly formed number",
#define ERR_NOMORE	11
    "No more words",
#define ERR_FILENAME	12
    "Missing file name",
#define ERR_GLOB	13
    "Internal glob error",
#define ERR_COMMAND	14
    "Command not found",
#define ERR_TOOFEW	15
    "Too few arguments",
#define ERR_TOOMANY	16
    "Too many arguments",
#define ERR_DANGER	17
    "Too dangerous to alias that",
#define ERR_EMPTYIF	18
    "Empty if",
#define ERR_IMPRTHEN	19
    "Improper then",
#define ERR_NOPAREN	20
    "Words not parenthesized",
#define ERR_NOTFOUND	21
    "%s not found",
#define ERR_MASK	22
    "Improper mask",
#define ERR_LIMIT	23
    "No such limit",
#define ERR_TOOLARGE	24
    "Argument too large",
#define ERR_SCALEF	25
    "Improper or unknown scale factor",
#define ERR_UNDVAR	26
    "Undefined variable",
#define ERR_DEEP	27
    "Directory stack not that deep",
#define ERR_BADSIG	28
    "Bad signal number",
#define ERR_UNKSIG	29
    "Unknown signal; kill -l lists signals",
#define ERR_VARBEGIN	30
    "Variable name must begin with a letter",
#define ERR_VARTOOLONG	31
    "Variable name too long",
#define ERR_VARALNUM	32
    "Variable name must contain alphanumeric characters",
#define ERR_JOBCONTROL	33
    "No job control in this shell",
#define ERR_EXPRESSION	34
    "Expression Syntax",
#define ERR_NOHOMEDIR	35
    "No home directory",
#define ERR_CANTCHANGE	36
    "Can't change to home directory",
#define ERR_NULLCOM	37
    "Invalid null command",
#define ERR_ASSIGN	38
    "Assignment missing expression",
#define ERR_UNKNOWNOP	39
    "Unknown operator",
#define ERR_AMBIG	40
    "Ambiguous",
#define ERR_EXISTS	41
    "%s: File exists",
#define ERR_ARGC	42
    "Argument for -c ends in backslash",
#define ERR_INTR	43
    "Interrupted",
#define ERR_RANGE	44
    "Subscript out of range",
#define ERR_OVERFLOW	45
    "Line overflow",
#define ERR_NOSUCHJOB	46
    "No such job",
#define ERR_TERMINAL	47
    "Can't from terminal",
#define ERR_NOTWHILE	48
    "Not in while/foreach",
#define ERR_NOPROC	49
    "No more processes",
#define ERR_NOMATCH	50
    "No match",
#define ERR_MISSING	51
    "Missing %c",
#define ERR_UNMATCHED	52
    "Unmatched %c",
#define ERR_NOMEM	53
    "Out of memory",
#define ERR_PIPE	54
    "Can't make pipe",
#define ERR_SYSTEM	55
    "%s: %s",
#define ERR_STRING	56
    "%s",
#define ERR_JOBS	57
    "Usage: jobs [ -l ]",
#define ERR_JOBARGS	58
    "Arguments should be jobs or process id's",
#define ERR_JOBCUR	59
    "No current job",
#define ERR_JOBPREV	60
    "No previous job",
#define ERR_JOBPAT	61
    "No job matches pattern",
#define ERR_NESTING	62
    "Fork nesting > %d; maybe `...` loop",
#define ERR_JOBCTRLSUB	63
    "No job control in subshells",
#define ERR_SYNC	64
    "Sunc fault: Process %d not found",
#define ERR_STOPPED	65
#ifdef SUSPENDED
    "%sThere are suspended jobs",
#else
    "%sThere are stopped jobs",
#endif				/* SUSPENDED */
#define ERR_NODIR	66
    "No other directory",
#define ERR_EMPTY	67
    "Directory stack empty",
#define ERR_BADDIR	68
    "Bad directory",
#define ERR_DIRUS	69
    "Usage: %s [-lvn]%s",
#define ERR_HFLAG	70
    "No operand for -h flag",
#define ERR_NOTLOGIN	71
    "Not a login shell",
#define ERR_DIV0	72
    "Division by 0",
#define ERR_MOD0	73
    "Mod by 0",
#define ERR_BADSCALE	74
    "Bad scaling; did you mean \"%s\"?",
#define ERR_SUSPLOG	75
    "Can't suspend a login shell (yet)",
#define ERR_UNKUSER	76
    "Unknown user: %s",
#define ERR_NOHOME	77
    "No $home variable set",
#define ERR_HISTUS	78
    "Usage: history [-rht] [# number of events]",
#define ERR_SPDOLLT	79
    "$, ! or < not allowed with $# or $?",
#define ERR_NEWLINE	80
    "Newline in variable name",
#define ERR_SPSTAR	81
    "* not allowed with $# or $?",
#define ERR_DIGIT	82
    "$?<digit> or $#<digit> not allowed",
#define ERR_VARILL	83
    "Illegal variable name",
#define ERR_NLINDEX	84
    "Newline in variable index",
#define ERR_EXPOVFL	85
    "Expansion buffer overflow",
#define ERR_VARSYN	86
    "Variable syntax",
#define ERR_BADBANG	87
    "Bad ! form",
#define ERR_NOSUBST	88
    "No previous substitute",
#define ERR_BADSUBST	89
    "Bad substitute",
#define ERR_LHS		90
    "No previous left hand side",
#define ERR_RHSLONG	91
    "Right hand side too long",
#define ERR_BADBANGMOD	92
    "Bad ! modifier: %c",
#define ERR_MODFAIL	93
    "Modifier failed",
#define ERR_SUBOVFL	94
    "Substitution buffer overflow",
#define ERR_BADBANGARG	95
    "Bad ! arg selector",
#define ERR_NOSEARCH	96
    "No prev search",
#define ERR_NOEVENT	97
    "%s: Event not found",
#define ERR_TOOMANYRP	98
    "Too many )'s",
#define ERR_TOOMANYLP	99
    "Too many ('s",
#define ERR_BADPLP	100
    "Badly placed (",
#define ERR_MISRED	101
    "Missing name for redirect",
#define ERR_OUTRED	102
    "Ambiguous output redirect",
#define ERR_REDPAR	103
    "Can't << within ()'s",
#define ERR_INRED	104
    "Ambiguous input redirect",
#define ERR_BADPLPS	105
    "Badly placed ()'s",
#define ERR_VARMOD	106
    "Unknown variable modifier",
#define ERR_ALIASLOOP	107
    "Alias loop",
#define ERR_NOWATCH	108
    "No $watch variable set",
#define ERR_NOSCHED	109
    "No scheduled events",
#define ERR_SCHEDUSAGE	110
    "Usage: sched -<item#>.\nUsage: sched [+]hh:mm <command>",
#define ERR_SCHEDEV	111
    "Not that many scheduled events",
#define ERR_SCHEDCOM	112
    "No command to run",
#define ERR_SCHEDTIME	113
    "Invalid time for event",
#define ERR_SCHEDREL	114
    "Relative time inconsistent with am/pm",
#define ERR_TCNOSTR	115
    "Out of termcap string space",
#define ERR_SETTCUS	116
    "Usage: settc %s [yes|no]",
#define ERR_TCCAP	117
    "Unknown capability `%s'",
#define ERR_TCPARM	118
    "Unknown termcap parameter `%%%c'",
#define ERR_TCARGS	119
    "Too many arguments for `%s' (%d)",
#define ERR_TCNARGS	120
    "`%s' requires %d arguments",
#define ERR_TCUSAGE	121
    "Usage: echotc [-v|-s] [<capability> [<args>]]",
#define ERR_ARCH	122
    "%s: %s. Wrong Architecture",
#define ERR_HISTLOOP	123
    "!# History loop",
#define ERR_FILEINQ	124
    "Malformed file inquiry",
#define ERR_SELOVFL	125
    "Selector overflow",
#define ERR_INVALID	126
    "Invalid Error"
};

/*
 * The parser and scanner set up errors for later by calling seterr,
 * which sets the variable err as a side effect; later to be tested,
 * e.g. in process.
 */
void
/*VARARGS1*/
#if __STDC__
seterror(unsigned int id, ...)
#else
seterror(va_alist)
    va_dcl
#endif
{

    if (seterr == 0) {
	va_list va;
	char    berr[BUFSIZ];
#if __STDC__
	va_start(va, id);
#else
	unsigned int id;
	va_start(va);
	id = va_arg(va, unsigned int);
#endif

	if (id >= sizeof(errorlist) / sizeof(errorlist[0]))
	    id = ERR_INVALID;
	xvsprintf(berr, errorlist[id], va);
	va_end(va);

	seterr = strsave(berr);
    }
}

/*
 * Print the error with the given id.
 *
 * Special ids:
 *	ERR_SILENT: Print nothing.
 *	ERR_OLD: Print the previously set error if one was there.
 *	         otherwise return.
 *	ERR_NAME: If this bit is set, print the name of the function
 *		  in bname
 *
 * This routine always resets or exits.  The flag haderr
 * is set so the routine who catches the unwind can propogate
 * it if they want.
 *
 * Note that any open files at the point of error will eventually
 * be closed in the routine process in sh.c which is the only
 * place error unwinds are ever caught.
 */
void
/*VARARGS*/
#if __STDC__
stderror(unsigned int id, ...)
#else
stderror(va_alist)
    va_dcl
#endif
{
    va_list va;
    register Char **v;
    int     flags;

#if __STDC__
    va_start(va, id);
#else
    unsigned int id;

    va_start(va);
    id = va_arg(va, unsigned int);
#endif

    flags = id & ERR_FLAGS;
    id &= ~ERR_FLAGS;

    if ((flags & ERR_OLD) && seterr == NULL) {
	va_end(va);
	return;
    }

    if (id >= sizeof(errorlist) / sizeof(errorlist[0]))
	id = ERR_INVALID;

    /*
     * Must flush before we print as we wish output before the error to go on
     * (some form of) standard output, while output after goes on (some form
     * of) diagnostic output. If didfds then output will go to 1/2 else to
     * FSHOUT/FSHDIAG. See flush in sh.print.c.
     */
    flush();
    haderr = 1;			/* Now to diagnostic output */
    timflg = 0;			/* This isn't otherwise reset */


    if (!(flags & ERR_SILENT)) {
	if (flags & ERR_NAME)
	    xprintf("%s: ", bname);
	if ((flags & ERR_OLD))
	    /* Old error. */
	    xprintf("%s.\n", seterr);
	else {
	    xvprintf(errorlist[id], va);
	    xprintf(".\n");
	}
    }
    va_end(va);

    if (seterr) {
	xfree((ptr_t) seterr);
	seterr = NULL;
    }

    if (v = pargv)
	pargv = 0, blkfree(v);
    if (v = gargv)
	gargv = 0, blkfree(v);

    didfds = 0;			/* Forget about 0,1,2 */
    /*
     * Go away if -e or we are a child shell
     */
    if (exiterr || child)
	xexit(1);

    /*
     * Reset the state of the input. This buffered seek to end of file will
     * also clear the while/foreach stack.
     */
    btoeof();

    set(STRstatus, Strsave(STR1));
#ifdef BSDJOBS
    if (tpgrp > 0)
	(void) tcsetpgrp(FSHTTY, tpgrp);
#endif
    reset();			/* Unwind */
}
