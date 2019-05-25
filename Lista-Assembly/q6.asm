org 0x7c00
jmp 0x0000:main

data:
    string times 51 db 0

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

        cmp cl, 50             
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

;Printa o caracter de al, resetando os contador
printa:
        mov al, cl
        add al, '0'
        xor cl ,cl
        call putchar
        call endl
        ret

;Função principal
main:
    xor ax, ax
    mov ds, ax
    mov es, ax
    
    mov di, string
    call gets
    
    mov si, string
    call reverse
    
    mov si, string
    mov cx, 1       
    xor ax, ax
    
    ;Converte a string para inteiro e salva na pilha
    .loop1:

        push ax

        lodsb

        ;Ao fim da string, conta-se as notas
        cmp al, 0
        je .notas

        ;Ao encontrar a virgula, dá-se push dos centavos na pilha
        cmp al, ','
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

    ;Zera o contador para pegar os reais
    .endloop1:
        
        mov cx, 1
        xor ax, ax

        jmp .loop1

    ;Define a quantidade de notas
    .notas:

        ;Retira o valor em reais da pilha
        pop bx

        ;Limpa o contador de notas
        xor cl, cl

        ;Notas de 100 reais
        .n100:
            cmp bx, 100
            jb .printa100
            
            sub bx, 100
            inc cl

            jmp .n100

        .printa100:
            call printa
            jmp .n50
    
        ;Notas de 50 reais
        .n50:
            cmp bx, 50
            jb .printa50
            
            sub bx, 50
            inc cl

            jmp .n50
            
        .printa50:
            call printa
            jmp .n20
        
        ;Notas de 20 reais
        .n20:
            cmp bx, 20
            jb .printa20
            
            sub bx, 20
            inc cl

            jmp .n20
        
        .printa20:
            call printa
            jmp .n10

        ;Notas de 10 reais
        .n10:
            cmp bx, 10
            jb .printa10
            
            sub bx, 10
            inc cl

            jmp .n10
        
        .printa10:
            call printa
            jmp .n5

        ;Notas de 5 reais
        .n5:
            cmp bx, 5
            jb .printa5
            
            sub bx, 5
            inc cl

            jmp .n5
        
        .printa5:
            call printa
            jmp .n2

        ;Notas de 2 reais
        .n2:
            cmp bx, 2
            jb .printa2
            
            sub bx, 2
            inc cl

            jmp .n2
        
        .printa2:
            call printa
            jmp .n1

        ;"Notas" de 1 real 
        .n1:
            cmp bx, 1
            jb .printa1
            
            sub bx, 1
            inc cl

            jmp .n1
        
        .printa1:
            call printa
            jmp .moedas

    ;Define a quantidade de moedas    
    .moedas:

        ;Retira o valor de centavos da pilha
        pop bx

        ;Zera o contador
        xor cl, cl

        ;Moedas de 50 centavos
        .c50:
            cmp bx, 50
            jb .printa50c
            
            sub bx, 50
            inc cl

            jmp .c50
            
        .printa50c:
            call printa
            jmp .c25

        ;Moedas de 25 centavos
        .c25:
            cmp bx, 25
            jb .printa25c
                
            sub bx, 25
            inc cl

            jmp .c25
            
        .printa25c:
            call printa
            jmp .c10

        ;Moedas de 10 centavos
        .c10:
            cmp bx, 10
            jb .printa10c
                
            sub bx, 10
            inc cl

            jmp .c10
            
        .printa10c:
            call printa
            jmp .c5

        ;Moedas de 5 centavos
        .c5:
            cmp bx, 5
            jb .printa5c
                
            sub bx, 5
            inc cl

            jmp .c5
            
        .printa5c:
            call printa
            jmp .c1

        ;Moedas de 1 centavo
        .c1:
            cmp bx, 1
            jb .printa1c
                
            sub bx, 1
            inc cl

            jmp .c1
            
        .printa1c:
            call printa
            jmp .done

        .done:
            jmp $

times 510-($-$$) db 0
dw 0xaa55