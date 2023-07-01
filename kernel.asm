.data
.text
.align 2
.globl main

    # To use any register other than $k0, $k1, we must store them first
    # addi $sp, $sp, -16         # sp + (-16) allocate stack memory

syscall:
    
    beq $v0, $0, syscall0   # if $v0 is 0, jump to main
    addi $k1, $0, 12
    beq $v0, $k1, syscall12     # if v0 is 12 jump to syscall 12
    addi $k1, $0, 9
    beq $v0, $k1, syscall9      # if v0 is 9 jump to syscall 9
    addi $k1, $0, 10
    beq $v0, $k1, syscall10     # if v0 is 10 jump to syscall 10
    addi $k1, $0, 11
    beq $v0, $k1, syscall11     # if v0 is 11 jump to syscall 11
    addi $k1, $0, 5
    beq $v0, $k1, syscall5     # if v0 is 5 jump to syscall 5
    addi $k1, $0, 7
    beq $v0, $k1, syscall7     # if v0 is 7 jump to syscall 7
    addi $k1, $0, 6
    beq $v0, $k1, syscall6     # if v0 is 7 jump to syscall 7
    jr $k0

syscall0:   
    addi $sp, $0, 65408
    la $k1, __STATICMEMORYEND__  # for syscall9
    sw $k1, 65280($0)
    j main

# syscall1:    # integer read

# syscall2:

# syscall3:

# syscall4:

# syscall5:   # Integer write. todo


syscall6:   # sound output
    sw $a0, 65441($0) # store the ascii at FFa1
    jr $k0

syscall7: # joystick. uses ffa0
    lw $v0, 65440($0)   # 0xFFa0 = joystick ready
    beq $v0, $0, end7
    sw $0, 65440($0)

    end7:
    jr $k0

# syscall8: # read strings

syscall9:   # Heap allocation todo             # load static memory usage in ram 
    lw $v0, 65280($0)           # store the current value of FF00
    add $k1, $a0, $v0           # update 
    sw $k1, 65280($0)           # store pointer to heap in OS (FF00 to FF80)

    jr $k0

syscall10:  # todo
    j syscall10

syscall11:    # Terminal print character syscall
    sw $a0, 65416($0) # store the ascii at FF88
    jr $k0

syscall12:  # Read Character. todo

    lw $k1, 65408($0)   # 0xFF80 = keyboard ready
    beq $k1, $0, end12
    lw $v0, 65412($0)    # v0 = *0xFF84
    sw $0, 65408($0) # set keyboard ready to 0 to get next character
    jr $k0

    end12:
    add $v0, $0, $0
    jr $k0


    # lw $s0, 0($sp)
    # lw $s1, 4($sp)
    # lw $s2, 8($sp)
    # lw $s3, 12($sp)
    # addi $sp, $sp, 16         # sp + 16 return stack memory


    # heap memory allocation
    # addi $a0, $0, 80    # number of bytes allocated
    # addi $v0, $0, 9     # syscall code for mem allocation
    # syscall

    

    