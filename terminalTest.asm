.data
.text
.align 2
.globl main
main:

    addi $a0, $0, 71        # ascii G
    addi $v0, $0, 11
    syscall

    addi $a0, $0, 65        # ascii a
    addi $v0, $0, 11
    syscall

    addi $a0, $0, 77        # ascii m
    addi $v0, $0, 11
    syscall

    addi $a0, $0, 69        # ascii e
    addi $v0, $0, 11
    syscall

    addi $a0, $0, 32        # ascii "space"
    addi $v0, $0, 11
    syscall

    addi $a0, $0, 79        # ascii O
    addi $v0, $0, 11
    syscall

    addi $a0, $0, 86        # ascii v
    addi $v0, $0, 11
    syscall

    addi $a0, $0, 69        # ascii e
    addi $v0, $0, 11
    syscall

    addi $a0, $0, 82        # ascii r
    addi $v0, $0, 11
    syscall

    addi $a0, $0, 33        # ascii !
    addi $v0, $0, 11
    syscall

    addi $a0, $0, 33        # ascii !
    addi $v0, $0, 11
    syscall

    addi $a0, $0, 33        # ascii !
    addi $v0, $0, 11
    syscall

    addi $a0, $0, 33        # ascii !
    addi $v0, $0, 11
    syscall

    addi $v0, $0, 10
    syscall
