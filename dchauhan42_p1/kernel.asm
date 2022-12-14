BITS 32

GLOBAL k_printstr

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

