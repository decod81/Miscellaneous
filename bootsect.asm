ORG 0x7C00
BITS 16

  mov ah, 0x00      ; reset
  mov dl, 0x80      ; drive
  int 0x13

  mov bx, 0x8000    ; segment
  mov es, bx
  mov bx, 0         ; offset (address = 16*segment+offset)

  mov ah, 0x02      ; read
  mov al, 1         ; read 1 sector (512 bytes)
  mov ch, 0         ; cylinder (top 8 bits)
  mov cl, 2         ; sector (starts at 1, cylinder bottom 2 bits and sector 6 bits)
  mov dh, 0         ; head
  mov dl, 0x80      ; drive (A = 0x00, C = 0x80)
  int 0x13          ; BIOS low-level disk services

  jmp 0x8000:0x0000

times 510 - ($-$$) db 0
dw 0xAA55

  mov ax, 0x13
  int 0x10

  mov dx, 0x03C6    ; palette mask
  mov al, 0xFF
  out dx, al

  mov al, 0         ; color index
loop1:
  mov dx, 0x03C8    ; set color index
  out dx, al
  mov dx, 0x03C9    ; set color values
  out dx, al        ; red = index
  out dx, al        ; green = index
  out dx, al        ; blue = index
  inc al
  cmp al, 64
  jne loop1

  mov bx, 0xA000
  mov es, bx
  mov di, 0

loop:
  mov ax, di
  shr al, 2
  stosb
  cmp di, 320*200
  jne loop

  cli
  hlt
