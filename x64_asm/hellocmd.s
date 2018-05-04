global _start
extern printf

section .data
fmt:	db "%d", 10, 0

section .text
_start:
	pop	rax
	mov	rdi, fmt
	mov	rsi, rax
	call	printf

	mov	rax, 60
	mov	rdi, 0
	syscall
