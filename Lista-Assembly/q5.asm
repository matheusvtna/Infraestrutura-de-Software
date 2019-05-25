org 0x7c00
jmp 0x0000:begin

;No sports      :: reg = '_'
;Artes Marciais :: reg = 'a'
;Basquete       :: reg = 'b'
;Futebol        :: reg = 'f'
;Rugby          :: reg = 'r'
;Volei          :: reg = 'v'

;-------------------------

;bx = Prioridade 1
;cx = Prioridade 2
;dx = Prioridade 3

data:
    string times 11 db 0
    null db '---', 0
    basquete db 'Basquete', 0
    futebol db 'Futebol', 0
    rugby db 'Rugby', 0
    artes_marciais db 'Artes Marciais', 0
    volei db 'Volei', 0

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

space:
      mov al, ' '
      call putchar
      ret

divisoria:
      mov al, '|'
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

        cmp cl, 10             
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

compara_cinco:
    ;Se o char al == 'b'
    cmp al, 'b'
    je .print_basquete

    ;Se o char al == 'f'
    cmp al, 'f'
    je .print_futebol
    
    ;Se o char al == 'a'
    cmp al, 'a'
    je .print_artesmarciais
    
    ;Se o char al == 'v'
    cmp al, 'v'
    je .print_volei
    
    ;Se o char al == 'r'
    cmp al, 'r'
    je .print_rugby

    ;Caso contrário, a posição está vazia
    jmp .print_null

    ;Printando as strings correspondentes
    .print_null:
        mov si, null
        call prints
        ret

    .print_basquete:
        mov si, basquete
        call prints
        ret
    
    .print_volei:
        mov si, volei
        call prints
        ret

    .print_rugby:
        mov si, rugby
        call prints
        ret

    .print_artesmarciais:
        mov si, artes_marciais
        call prints
        ret
    
    .print_futebol:
        mov si, futebol
        call prints
        ret

begin:
    ;Mantém a pilha apenas com '_'
    mov ax, 95
    push ax
    push ax
    push ax
    
    ;Reseta os registradores
    xor ax, ax
    mov cx, ax
    mov bx, ax
    mov dx, ax
    mov ds, ax
    mov es, ax

    jmp main

main:
    ;Printa o estado atual da vitrine
    .printState:
        ;Desempilha os 3 primeiros esportes
        pop bx                          ;1ª PRIORIDADE
        pop cx                          ;2ª PRIORIDADE
        pop dx                          ;3ª PRIORIDADE
        
        ;Printa o 3º em prioridade
        mov ax, dx
        call compara_cinco
        call space
        call divisoria
        call space

        ;Printa o 1º em prioridade
        mov ax, bx
        call compara_cinco
        call space
        call divisoria
        call space

        ;Printa o 2º em prioridade
        mov ax, cx
        call compara_cinco

        ;Reempilha os esportes para manter a configuraççao atual
        push dx
        push cx
        push bx
    
    .endPrint:    
        call endl

    xor ax, ax
    mov ds, ax
    mov es, ax
    
    ;Scanf da string 'esporte' ',' 'prioridade'
    mov di, string
    call gets

    ;Faz si apontar pro início da string
    mov si, string

    ;Carrega o char que indica o esporte
    lodsb 

    ;Salva em bx o char do esporte 
    mov bx, ax

    ;Zera ax
    xor ax, ax

    ;Carrega a vírgula e ignora
    lodsb

    ;Zera ax
    xor ax, ax

    ;Carrega o valor da prioridade
    lodsb

    ;Salva em cx o "char" da prioridade
    mov cx, ax

    ;Atualiza as prioridades
    .update:
        mov ax, cx

        ;Compara essa prioridade com 1, 2 e 3 para definir o novo estado
        cmp ax, '1'
        je .caso1

        cmp ax, '2'
        je .caso2

        cmp ax, '3'
        je .caso3

        .caso1:
            jmp .first    
        .caso2:
            jmp .second
        .caso3:
            jmp .third

    .third:
        ;Desempilha os esportes de 1ª e 2ª prioridade, respectivamente
        pop ax
        pop dx

        ;Empilha o novo esporte de prioridade 3
        push bx

        ;Reempilha os esportes de 2ª e 1ª prioridades, respectivamente
        push dx
        push ax

        jmp .endUpdate

    .second:
        ;Desempilha o esporte de 1ª prioridade
        pop ax

        ;Desempilha o esporte de 2ª prioridade
        pop cx

        ;Se a posição 2 está vazia, devemos manter o de 3ª prioridade
        cmp cx, 95
        je .onlyPut

        ;Caso contrário...

        ;Reempilha-se o cx como esporte de 3ª prioridade
        push cx

        ;Empilha o novo esporte de prioridade 2
        push bx
        
        ;Reempilha o esporte de 1ª prioridade
        push ax    

        jmp .endUpdate

        .onlyPut:
            ;Empilha bx normalmente e cx é desconsiderado
            push bx

            ;Reempilha o esporte de 1ª prioridade
            push ax

            jmp .endUpdate

    .first:
        ;Desempilha o esporte de 1ª prioridade
        pop ax    

        ;Se a posição 1 estiver vazia, devemos manter os esportes de 2ª e 3ª prioridade
        cmp ax, 95
        je .onlyPut1

        ;Caso contrário...

        ;Desempilhamos o esporte de 2ª prioridade
        pop cx
        
        ;Verificamos se este é vazio, pois, se for, o esporte de 3ª prioridade deve ser mantido
        cmp cx, 95
        je .onlyPut2

        ;Caso a posição de prioridade 2 esteja ocupada, seguimos normalmente

        ;Reempilha-se o cx como esporte de 3ª prioridade
        push cx

        ;Reempilha-se o ax como esporte de 2ª prioridade
        push ax

        ;Empilha o novo esporte de 1ª prioridade
        push bx

        jmp .endUpdate

        .onlyPut1:
            ;Empilha bx normalmente e ax é desconsiderado
            push bx

            jmp .endUpdate
        
        .onlyPut2:
            ;Empilha ax como esporte de 2ª de prioridade
            push ax

            ;Empilha bx como novo esporte de 1ª prioridade
            push bx

            jmp .endUpdate    

    .endUpdate:
        ;Zera ax
        xor ax, ax

        jmp main
    
    
times 510-($-$$) db 0
dw 0xaa55