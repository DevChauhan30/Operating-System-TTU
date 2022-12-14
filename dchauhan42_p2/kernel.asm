BITS 32

GLOBAL k_printstr
GLOBAL go
GLOBAL dispatch
GLOBAL dispatch_leave
GLOBAL lidtr

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