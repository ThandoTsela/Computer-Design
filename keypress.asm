.data
.text
.align 2
.globl main
main:
       # addi $sp, $0, 65408 #FF80
    addi $t0, $0, 8
    addi $t1, $0, 5   # blue
    addi $t2, $0, 2

    # MUST DELETE. WAS TESTING NOT
    and $s0, $t0, $t1
    or $s1, $t0, $t1
    not $s2, $t0, $t1
    nor $s3, $t0, $t1
    xor $s4, $t0, $t1
    andi $s4, $t0, 3
    ori $s5, $t1, 3
    xori $s6, $t2, 3
    lui $s7, $t0, 3
    sltu $t8, $t0, $t1