; turns supaplex.exe into a .com-file
; nasm -f bin supa.asm -o supa.com
; CS:IP 0AFF:0010 (45056)
; SS:SP 58D4:0080 (363968)
; relocations don't happen and this will work

BITS 16
ORG 0x100
	mov	ax, cs

	mov	bx, 0x0080	 ; stack is really kind of arbitrary here
	mov	sp, bx		 ; set SP

	mov	bx, 0x58D4       ; stack is really kind of arbitrary here
	mov	ss, bx		 ; set SS
	
	add	ax, 32/16+0x0010+0x0AFF ; skip our code
	push	ax               ; add CS offset
	push	0x0010
	retf                     ; jump to where execution should start

times 32- ($-$$) db 0

begin:

; md5sum supaplex.exe 7caf79506860c98c0e49c1247c81a181
incbin "supaplex.exe",512       ; add the original EXE-file skipping the header
