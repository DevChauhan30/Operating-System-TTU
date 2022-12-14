BITS 32

GLOBAL k_printstr
GLOBAL go
GLOBAL dispatch
GLOBAL dispatch_leave
GLOBAL lidtr
GLOBAL init_timer_dev
GLOBAL outportb

extern enqueue
extern dequeue
extern round_robin
extern run

k_printstr:

	push ebp
	mov ebp, esp
	pushfd
	push eax
	push ebx 
	push esi
	push edi

    mov esi, [ebp + 8]     ; moves address string into esi
    mov eax, [ebp + 12]    ; moves row in eax
    mov ebx, [ebp + 16]    ; moves column into ebx

   
	imul eax, 80	    ; multiply row*80	
	add eax, ebx        ; adds column into row*80	
	imul eax, 2	        ; multiply 2(row*80 + column)
	add eax, 0xB8000    ; add 0xB8000 + 2(row*80 + column)	
	mov edi, eax        ; moves offset into edi		

loop:
	cmp BYTE[esi], 0
	je done
    cmp edi, 0xB8FA0
	movsb 
	mov BYTE[edi], 31
	inc edi
	jmp loop

done:
	pop edi
	pop esi
	pop ebx
	pop eax
	popfd
	pop ebp
	ret

go:
	push DWORD [round_robin]
    call dequeue
	mov [run], eax     ;makes eax the run process

go_rest:
    mov esp, [eax]     ;restore registers gs, fs, es, and ds
    popad
    ret  

dispatch:
    call yield

dispatch_leave:
    push eax
    mov al, 0x20  ;send EOI to PIC
    out 0x20, al
    pop eax
    iret

yield:
    pusha
    mov esi, [run] ; mov pointer into esi
    mov [esi], esp ; mov esp into the process's esp
    
    push DWORD [run] ; push run context to round_robin queue
    push DWORD [round_robin]
    call enqueue

    jmp go 

lidtr:
    push ebp
    mov ebp, esp
    push eax
    mov eax, [ebp+8]
    lidt [eax]
    pop eax
    pop ebp
    ret

outportb:
    push ebp 
    mov ebp, esp
    push eax
    push edx
    
    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, al

    pop edx
    pop eax
    pop ebp
    ret

init_timer_dev:
    push ebp
	mov ebp, esp
	pushfd
	pushad
	
	mov edx, [ebp+8]
	mov ax, 1193
	imul dx, ax

	mov al, 0b00110110 ;0x43 is the write control word
	out 0x43, al
	
	mov ax, dx
	out 0x40, al ;LSB
	xchg ah, al
	out 0x40, al ;MSB

	popad
	popfd
	pop ebp
	ret