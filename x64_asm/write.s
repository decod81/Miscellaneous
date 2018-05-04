; standard paramater order: rdi, rsi, rdx, rcx, r8, r9, stack
section .text
global write
write:
	mov	rax, 1        ;
;	mov	rdi, 1        ; parameter 1
;	mov	rsi, [rsp+8]  ; parameter 2: address to text
;	mov	rdx, [rsp+16] ; parameter 3: length
	syscall
	ret
