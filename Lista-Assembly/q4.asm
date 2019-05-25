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


main:
    ;Zera os registradores
    xor ax, ax
    mov ds, ax
    mov es, ax
    
    mov di, string
    call gets

    xor dx, dx
    
    mov si, string
    
    .vemdezap:
        lodsb

        cmp al, 0                   ;Se for o fim da string
        je .done                    ;é o fim do código

        cmp al, ' '                 ;Se o caracter atual for o espaço,
        je .printa                  ;ele deve ser ignorado

        ;Caso contrário, altera-se a string

        .mexenastring:

            cmp dx, 0               ;Se a flag for 0
            je .maiusculo           ;torna-se o caracter maiúsculo

            jmp .minusculo          ;Caso contrário, o caracter mantém-se minusculo

            ;Torna um caracter minúsculo em maiúsculo
            .maiusculo:

                inc dx              ;Incrementa-se a flag       

                cmp al, 'Z'         ;Se a letra já estiver maiúscula 
                jbe .printa         ;apenas printamos e retornamos a leitura da string


                ;Caso contrário, transformamos esta em maiúscula
                sub al, 32
                jmp .printa

            ;Garante que o caracter permaneça minúsculo
            .minusculo:
                dec dx              ;Decrementa-se a flag

                cmp al, 'Z'         ;Se a letra já estiver minúscula
                ja .printa          ;apenas printamos e retornamos a leitura da string    

                ;Caso contrário, transformamos esta em minúscula
                add al, 32
                jmp .printa

            ;Printa o caracter já modificado
            .printa:
                call putchar
                jmp .vemdezap

    .done:
        jmp $       
    
times 510-($-$$) db 0
dw 0xaa55
