.data
.text
.align 2
.globl main
main:

addi $s0, $0, 24            # value for up on Joystick
addi $s1, $0, -8            # value for down
addi $s2, $0, -127          # value for left
addi $s3, $0, -113          # value for right
addi $s4, $0, 17            # value at top left that will end the program
addi $s5, $0, 31            # value at top right
addi $s6, $0, -1            # value at bottom right
addi $s7, $0, -15           # value at bottom left

startloop:
    addi $v0,$0,7           # Read value from Joystick
    syscall 

    add $t0, $0, $v0        # store value of joystick at t0

    beq $t0, $s0, ifUp
    beq $t0, $s1, ifDown
    beq $t0, $s2, ifLeft
    beq $t0, $s3, ifRight
    beq $t0, $s4, ifTopLeft
    j startloop

ifUp:
    addi $a0, $0, 85        # ascii U
    addi $v0, $0, 11
    syscall
    addi $a0, $0, 32        # ascii "space"
    addi $v0, $0, 11
    syscall
    j startloop

ifDown:
    addi $a0, $0, 68        # ascii D
    addi $v0, $0, 11
    syscall
    addi $a0, $0, 32        # ascii "space"
    addi $v0, $0, 11
    syscall
    j startloop

ifLeft:
    addi $a0, $0, 76        # ascii L
    addi $v0, $0, 11
    syscall
    addi $a0, $0, 32        # ascii "space"
    addi $v0, $0, 11
    syscall
    j startloop

ifRight:
    addi $a0, $0, 82        # ascii R
    addi $v0, $0, 11
    syscall
    addi $a0, $0, 32        # ascii "space"
    addi $v0, $0, 11
    syscall
    j startloop

ifTopLeft:
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
       #Game ends