org 0x7c00
jmp 0x0000:main

data:
    string times 101 db 0

putchar:
    mov ah, 0x0e
    int 10h
    ret

getchar:
    mov ah, 0x00
    int 16h
    ret

delchar:
    mov al, 0x08                    
    call putchar
    mov al, ' '
    call putchar
    mov al, 0x08                    
    call putchar
    ret

endl:
    mov al, 0x0a                    
    call putchar
    mov al, 0x0d                    
    call putchar
    ret

prints:                         
    .loop:
        lodsb                   ;Carrega caracter em al              
        cmp al, 0
        je .endloop
        call putchar
        jmp .loop
    .endloop:
    ret

gets:                           
    xor cx, cx                  
    .loop1:
        call getchar

        cmp al, 0x08            ;0x08 == Backspace                
        je .backspace

        cmp al, 0x0d               
        je .done

        cmp cl, 100             
        je .loop1
        
        stosb
        inc cl
        call putchar
        
        jmp .loop1

        .backspace:
            cmp cl, 0           
            je .loop1
            dec di
            dec cl
            mov byte[di], 0
            call delchar
        jmp .loop1

    .done:
    mov al, 0
    stosb
    call endl
    ret

;Reverte uma string    
reverse:                        
    mov di, si
    xor cx, cx                      ;Zerar o contador

    ;Colocando string na pilha                  
    .loop1:                     
        lodsb
        cmp al, 0
        je .endloop1
        inc cl
        push ax
        jmp .loop1
    .endloop1:
    
    ;Retirando a string da pilha (REVERTIDA)
    .loop2:                     
        cmp cl, 0
        je .endloop2
        dec cl
        pop ax
        stosb
        jmp .loop2
    .endloop2:
    ret

;Transforma inteiro em string
tostring:						
	push di
	.loop1:
		cmp ax, 0
		je .endloop1
		xor dx, dx
		mov bx, 10
		div bx					
		xchg ax, dx				
		add ax, 48				
		stosb
		xchg ax, dx
		jmp .loop1
	.endloop1:
	pop si
	cmp si, di
	jne .done
	mov al, 48
	stosb
	.done:
		mov al, 0
		stosb
		call reverse
		ret

PA:
    ;Aplica-se a fórmula da PA: (1 + n)* (n/2)
    mov bx, ax
    add bx, 1
    mul bx
    mov bx, 2
    div bx
    ret

main:
    ;Zera os registradores
    xor ax, ax
    mov ds, ax
    mov es, ax
    
    ;Lê a string (número 'n')
    mov di, string
    call gets
    
    ;Reverte a string para a conversão
    mov si, string
    call reverse
    
    ;Zera o contador cx e o registrador ax
    mov si, string
    mov cx, 1                       ;10^n
    xor ax, ax

    ;Transforma a string em inteiro
    .loop1:
        push ax

        lodsb

        cmp al, 0
        je .endloop1

        xor ah, ah
        sub al, 48
        mul cx

        pop bx
        add ax, bx

        push ax

        mov ax, cx
        mov bx, 10
        mul bx

        mov cx, ax
        pop ax
        jmp .loop1   

    .endloop1:

    ;Salva em ax o valor de toda a conversão
    pop ax              

    ;Resolve-se a PA 
    call PA
    
    ;Printa o resultado
    mov di, string
    call tostring       
    mov si, string
    call prints         
    
times 510-($-$$) db 0
dw 0xaa55