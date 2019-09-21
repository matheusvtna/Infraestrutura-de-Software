# Infraestrutura-de-Software

Projetos relacionados á disciplina de Infraestrutura de Software / Sistemas Operacionais lecionada no Centro de Informática - CIn UFPE - Campus Recife.

Cada pasta está relacionada a um projeto específico:
Bootloader:
    Códigos em Assembly x86 para o desenvolvimento do jogo GENIUS em Assembly, utilizando um bootloader para sua inicialização.
    Código makefile para a execução do projeto
  
    Execução do projeto:
    $ git clone https://github.com/matheusvtna/Infraestrutura-de-Software
    $ cd Bootloader
    $ makeall
  
Lista-Assembly:
    Códigos em Assembly x86 para a resolução de questões propostas pela monitoria
   
    Instalação NASM e QEMU:
    $ sudo apt-get install nasm
    $ sudo apt-get install qemu
   
    Execução do projeto:
    $ git clone https://github.com/matheusvtna/Infraestrutura-de-Software
    $ cd Lista-Assembly
    $ cd qi             #substitua i pelo número da questão desejada
    $ nasm qi.asm -o qi.bin
    $ qemu-system-i386 qi.bin
    
Threads:
    Códigos em C para a resolução de questões propostas pela monitoria
      
    Execução do projeto:
    $ git clone https://github.com/matheusvtna/Infraestrutura-de-Software
    $ cd Threads
    $ cd qi             #substitua i pelo número da questão desejada
    $ gcc qi.c -pthread -o q
    $ ./q


Equipe: Alexandre de Queiroz Burle, Luis Eduardo Martins Alves e Matheus Vinícius Teotonio do Nascimento Andrade
