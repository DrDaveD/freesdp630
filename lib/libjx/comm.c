/*       Copyright (c) 1987 AT&T   */
/*       All Rights Reserved       */

/*       THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T   */
/*       The copyright notice above does not evidence any      */
/*       actual or intended publication of such source code.   */


static char _2Vsccsid[]="@(#)comm.c	1.1.1.3	(5/10/87)";

#include <dmd.h>
#include <dmdio.h>
#define _SYSTAG		0375
#define _SYSINIT	0374
#define _SYSSYNC	0373

typedef union {
	short s;
	char c[2];
} schar;

typedef union {
	long l;
	char c[4];
} lchar;

#define JXBUFSIZ 50
/*static*/ char cmdbuf[JXBUFSIZ];	/* command information only */
static char *cmdptr = cmdbuf;	/* command information only */


jflush()
{
	sendnchars((int)(cmdptr-cmdbuf),cmdbuf);
	cmdptr = cmdbuf;
}

jputchar(c)
char c;
{
	*cmdptr++ = c;
	if (cmdptr > &(cmdbuf[JXBUFSIZ - 2]))
		jflush();
}

jputnchars(n,p)
register n;
register char *p;
{
	do {
		*cmdptr++ = *p++;
		if (cmdptr > &(cmdbuf[JXBUFSIZ - 2]))
			jflush();
	} while (--n > 0);
}

jputshort(s)
short s;
{
	jputnchars(2,&s);	/* a feeble improvement */
}

jputlong(l)
lchar l;
{
	jputnchars(4,l.c);	/* but an improvement */
}

jputbuf(p,n)
register char *p;
register short n;
{
	jputshort(n);
	jflush();
	sendnchars(n,p);	/* not always feeble */
}

jputstr(s)
register char *s;
{
	register char c;
	do {
		jputchar(c = *s++);
	} while (c != '\0');
}

#undef jgetchar
jgetchar()
{
	register c;
	while ((c = rcvchar()) == -1)
		wait(RCV);
	return(c);
}

short
jgetshort()
{
	schar s;
	register char *p;
	p = s.c;
	*p++ = jgetchar();
	*p++ = jgetchar();
	return(s.s);
}

long
jgetlong()
{
	lchar l;
	register char *p;
	p = l.c;
	*p++ = jgetchar();
	*p++ = jgetchar();
	*p++ = jgetchar();
	*p++ = jgetchar();
	return(l.l);
}

jgetbuf(p,n)
register char *p;
register short n;
{
	register short i;
	jputshort(n);
	jflush();
	n = jgetshort();
	for (i = 0; i < n; i++)
		*p++ = jgetchar();
	return(n);
}

jgetstr(s)
register char *s;
{
	while (*s++ = jgetchar());
}

/*static*/ int synced;

tag(c)
char c;
{
	if (synced == 0) {
		sync();
		synced = 1;
	}
	jputchar(_SYSTAG);
	jputchar(c);
}

sync()
{
	jputchar(_SYSTAG);
	jputchar(_SYSSYNC);
	jflush();
	while (jgetchar() != _SYSSYNC)
		;
}
