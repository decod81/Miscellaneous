section .text
extern main
global _start
_start:
	pop	rdi       ; first argument
	call	main

	mov	rax, 60
	mov	rdi, 0
	syscall
