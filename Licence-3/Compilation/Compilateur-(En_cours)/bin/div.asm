:main
	const bp,stack
	const sp,stack
	const ax,2
	sub sp,ax
	const ax,2
	push ax
	const ax,3
	push ax
	pop ax
	pop bx
	add bx,ax
	push bx
	const ax,4
	push ax
	pop ax
	pop bx
	mul bx,ax
	push bx
	const ax,2
	push ax
	pop ax
	pop bx
	sub bx,ax
	push bx
	const ax,0
	push ax
	pop ax
	pop bx
	const cx,erreur
	div bx,ax
	jmpe cx
	push bx
	cp ax,sp
	callprintfd ax
	pop ax
	end
:erreur
	const ax,err_div
	callprintfs ax
	end
:err_div
@string "Erreur : Division par Zéro !"
:stack
@int 0
