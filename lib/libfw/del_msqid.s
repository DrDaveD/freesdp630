#       Copyright (c) 1987 AT&T   
#       All Rights Reserved       

#       THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T   
#       The copyright notice above does not evidence any     
#       actual or intended publication of such source code.  

# @(#)del_msqid.s	1.1.1.3	(5/12/87)

	text
	def	del_msqid;	val	del_msqid;	scl	2;	type	040;	endef

	global	del_msqid
del_msqid:
	mov.l	_addrSys,%a0
	mov.l	4*829(%a0),%a0
	jmp	(%a0)
	def	del_msqid;	val	~;	scl	-1;	endef
