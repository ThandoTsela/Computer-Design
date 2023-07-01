.data
.text
.align 2
.globl main
main:
start:
    addi $a0, $0, 415       # frequency g
    addi $v0, $0, 6
    syscall

    addi $a0, $0, 370       # frequency f#
    addi $v0, $0, 6
    syscall

    addi $a0, $0, 349       # frequency f
    addi $v0, $0, 6
    syscall
j start