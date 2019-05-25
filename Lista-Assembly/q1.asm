org 0x7c00
jmp 0x0000:start

bandeirinha db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 7, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 8, 7, 8, 8, 8, 8, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 8, 8, 8, 8, 3, 1, 8, 8, 8, 8, 1, 8, 0, 0, 0, 0, 0, 0, 8, 8, 1, 3, 9, 9, 8, 1, 9, 8, 0, 0, 0, 0, 0, 0, 8, 8, 9, 9, 15, 15, 9, 9, 9, 8, 0, 0, 0, 0, 8, 0, 8, 9, 9, 9, 9, 3, 9, 9, 9, 1, 0, 0, 0, 0, 8, 8, 8, 9, 15, 15, 15, 3, 9, 9, 9, 1, 0, 0, 0, 0, 8, 0, 8, 9, 9, 9, 15, 15, 9, 9, 3, 8, 0, 0, 0, 0, 8, 8, 8, 8, 8, 9, 9, 9, 9, 8, 8, 0, 0, 0, 0, 0, 8, 8, 8, 0, 0, 8, 1, 9, 9, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

start:
    xor ax, ax              ;Zera ax    
    mov ds, ax              ;Zera ds
    mov cx, ax              ;Zera cx (Coluna 0)
    mov dx, ax              ;Zera dx (Linha 0)
        
    mov si, bandeirinha     ;si aponta pro início de 'bandeirinha'          
    call modoVideo          ;Inicia o modo de vídeo
    call picasso            ;Inicia a função para desenhar a bandeira

    jmp done                ;Fim do código

modoVideo:
    mov al, 13H             ;Parâmetros necessários para modo de vídeo
    mov ah, 0
    int 10h

    ret

pintaPixel:
    mov ah, 0ch             ;Desenha um pixel no ecrã
    int 10h
    ret

picasso:
    mov dx, 0               ;Define a linha inicial para 0          
    
    call loopi              ;Chama o loop 
    ret 

loopi:
    cmp dl, 16              ;Se a linha for 16
    je .done1               ;é o fim da bandeira

                            ;Caso contŕário
    mov cx, 0               ;Seta a coluna para 0
    call loop2              ;e chama o loop para a próxima coluna   

    inc dx                  ;Incrementa-se a linha atual
    jmp loopi               ;e retorna-se ao loop

    .done1:
        ret
    

loop2:
    cmp cl, 16              ;Se a coluna for 16
    je .done2               ;é o fim da linha

                            ;Caso contrário
    lodsb                   ;faz si apontar pro próximo caractere da string
    
    call pintaPixel         ;Chama a função para pintar um pixel na janela
    inc cx                  ;incrementa a linha
    jmp loop2               ;retorna-se ao loop2


    .done2:
        ret
        

done:
    jmp $

times 510 - ($ - $$) db 0
dw 0xaa55