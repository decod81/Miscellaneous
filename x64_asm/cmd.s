section .text
extern main
global _start

_start:
	pop	rdi
	pop	rsi
	pop	rdx
	call	main

	mov	rax, 60
	mov	rdi, 0
	syscall
