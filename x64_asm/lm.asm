; nasm -f bin lm.asm -o lm
; qemu-system-x86_64 -hda kernel

%define PAGE_PRESENT    (1 << 0)
%define PAGE_WRITE      (1 << 1)

ORG 0x7C00
BITS 16

  jmp Main

DAP:
  dw 0x1000
  dw 16          ; # of 512 byte blocks to do
  dw 0x8000      ; address (offset)
  dw 0x0000      ; address (segment)
  dd 1           ; read sector #
  dd 0

Main:
  in al, 0x92    ;
  or al, 2       ; enable A20
  out 0x92, al   ;

  mov si, DAP    ; disk address packet
  mov ah, 0x42   ; extended read
  mov dl, 0x80   ; drive = C
  int 0x13       ; read using BIOS

  xor ax, ax
  mov ss, ax
  mov sp, Main

  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  cld

  mov edi, 0x9000 ; free space

  jmp SwitchToLongMode

ALIGN 4
IDT:
  .Length       dw 0
  .Base         dd 0

SwitchToLongMode:
  push di
  mov ecx, 0x1000
  xor eax, eax
  cld
  rep stosd
  pop di

  lea eax, [es:di + 0x1000]
  or eax, PAGE_PRESENT | PAGE_WRITE
  mov [es:di], eax

  lea eax, [es:di + 0x2000]
  or eax, PAGE_PRESENT | PAGE_WRITE
  mov [es:di + 0x1000], eax

  lea eax, [es:di + 0x3000]
  or eax, PAGE_PRESENT | PAGE_WRITE
  mov [es:di + 0x2000], eax

  push di
  lea di, [di + 0x3000]
  mov eax, PAGE_PRESENT | PAGE_WRITE

.LoopPageTable:
  mov [es:di], eax
  add eax, 0x1000
  add di, 8
  cmp eax, 0x200000
  jb .LoopPageTable

  pop di

  mov al, 0xFF
  out 0xA1, al
  out 0x21, al

  nop
  nop

  lidt [IDT]

  mov eax, 10100000b
  mov cr4, eax

  mov edx, edi
  mov cr3, edx

  mov ecx, 0xC0000080
  rdmsr

  or eax, 0x00000100
  wrmsr

  mov ebx, cr0
  or ebx,0x80000001
  mov cr0, ebx

  lgdt [GDT.Pointer]

  jmp 0x0008:_start

GDT:
.Null:
  dq 0x0000000000000000

.Code:
  dq 0x00209A0000000000
  dq 0x0000920000000000

ALIGN 4
  dw 0

.Pointer:
  dw $ - GDT - 1
  dd GDT

times 510 - ($-$$) db 0
dw 0xAA55

_start:
