.data
.text
.align 2
.globl main

syscall:
    beq $v0, $0, main   # if $v0 is 0, jump to main
    addi $k1, $0, 12
    beq $v0, $k1, syscall12     # if v0 is 12 jump to syscall 12
    addi $k1, $0, 9
    beq $v0, $k1, syscall9      # if v0 is 9 jump to syscall 9
    addi $k1, $0, 10
    beq $v0, $k1, syscall10     # if v0 is 10 jump to syscall 10
    jr $k0

syscall1:

syscall2:

syscall3:

syscall4:

syscall5:   # must do
    jr $k0

syscall6:

syscall7:

syscall8:

syscall9:   # must do
    jr $k0

syscall10:  # must do
    j syscall10

syscall11:

syscall12:  # must do
    jr $k0

main:

    