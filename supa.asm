; nasm -f bin supa.asm -o supa.com
; CS:IP 0AFF:0010 (45056)
; SS:SP 58D4:0080 (363968)

; 8B E8 8C C0 05 10 00 0E 1F A3 04 00 03 06 0C 00

BITS 16
ORG 0x100
	mov	ax, cs
	mov	bx, ax

	add	cx, 0x0080	; add SP offset
	mov	sp, cx		; set SP

	add	bx, 64/16	; move past the this program
	add	bx, 0x58D4	; add SS offset
	mov	ss, bx		; set SS
	
	add	ax, 64/16       ; move past the this program
	add	ax, 16          ; correct org
	add	ax, 0x0AFF	; add CS offset
	mov	bx, 0x0010
	push	ax
	push	bx
	mov	ax, 0
	mov	bx, 0
	retf                    ; just to where execution should start

times 64- ($-$$) db 0

begin:

; md5sum supaplex.exe 7caf79506860c98c0e49c1247c81a181
incbin "supaplex.exe",512       ; add the original EXE-file skipping the header