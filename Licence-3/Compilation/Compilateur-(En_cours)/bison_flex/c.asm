:main
	const bp,stack
	const sp,stack
	const ax,2
	sub sp,ax
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
