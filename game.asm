.data
.text
.align 2
.globl main
main:
    
    addi $s0, $0, 128       # screenw=128
    addi $s1, $0, 126       # screenh=126

    addi $s2, $0, 64        # snakex = 64
    addi $s3, $0, 63        # snakey = 63

    addi $s4, $0, 1         # xdirection = 1 
    addi $s5, $0, 0         # ydirection = 0

    #initialize an array
    mult $s0,$s1
    mflo $a0
    sll $a0,$a0,2
    addi $v0,$0,9
    syscall     #save the array into the heap
    add $s6,$0,$v0 #array address


addi $t0, $0, 1
startloop:
#start the while loop

    beq $t0, $0,  endgame     # end the game when false

    # lw $t1, 65412($0) # 0xFF84 = read keyboard character
    addi $v0,$0,12
    syscall 
    add $t1,$0,$v0          # store value from keyboard

joystick:
    addi $v0,$0,7           # Read value from Joystick
    syscall 
    add $t3,$0,$v0          # store value of joystick at t3

    addi $t2, $0, 97 # ascii a
    bne $t1, $t2, ifD       # if x=="a"
    addi $s4,$0,-1
    addi $s5,$0,0
    addi $a0, $0, 349       # frequency f
    addi $v0, $0, 6
    syscall
    j endloop

ifD:
    addi $t2, $0, 100 # ascii d
    bne $t1, $t2, ifS
    addi $s4,$0,1
    addi $s5,$0,0
    addi $a0, $0, 261       # frequency c
    addi $v0, $0, 6
    syscall
    j endloop

ifS:
    addi $t2, $0, 115 # ascii s
    bne $t1, $t2, ifW
    addi $s4,$0,0
    addi $s5,$0,1
    addi $a0, $0, 370       # frequency f#
    addi $v0, $0, 6
    syscall
    j endloop

ifW:
    addi $t2, $0, 119 # ascii w
    bne $t1, $t2, ifUp
    addi $s4,$0,0
    addi $s5,$0,-1
    addi $a0, $0, 415       # frequency g
    addi $v0, $0, 6
    syscall
    j endloop


ifUp:
    addi $t2, $0, 24            # value for up on Joystick
    bne $t3, $t2, ifDown
    addi $s4,$0,0
    addi $s5,$0,-1
    addi $a0, $0, 415       # frequency g
    addi $v0, $0, 6
    syscall
    j endloop

ifDown:
    addi $t2, $0, -8            # value for down on Joystick
    bne $t3, $t2, ifLeft
    addi $s4,$0,0
    addi $s5,$0,1
    addi $a0, $0, 370       # frequency f#
    addi $v0, $0, 6
    syscall
    j endloop

ifLeft:
    addi $t2, $0, -127          # value for left on Joystick
    bne $t3, $t2, ifRight
    addi $s4,$0,-1
    addi $s5,$0,0
    addi $a0, $0, 349       # frequency f
    addi $v0, $0, 6
    syscall
    j endloop

ifRight:
    addi $t2, $0, -113          # value for right on Joystick
    bne $t3, $t2, endloop
    addi $s4,$0,1
    addi $s5,$0,0
    addi $a0, $0, 261       # frequency c
    addi $v0, $0, 6
    syscall

endloop:
       add $s2,$s2,$s4
       add $s3,$s3,$s5

#Final iffs
#get the array address/pointer
# add $s6,$0,$v0 #array address
# lw $s6, 65280($0)

# Address = baseAddr + ( rowIndex * ColSize + ColIndex) * 4
#accessing the value of the elem
mult $s2,$s1 # rowIndex * Col numb
mflo $t3
add $t3,$s3,$t3 # rowIndex * col numb + colIndex
sll $t3,$t3,2 # mult by 4 to get the memory address of the elem
add $t3, $s6, $t3 # get the final array memory address
lw $t4, 0($t3) # the array element

# t4 = board[snakex][snakey]

addi $t5,$0,1
beq $t4,$t5, endgame

#first
slt $t6,$s2,$0   #snakex < 0
beq $t6,$t5, endgame

#sec
slt $t7,$s0,$s2 #screenw < snakex
beq $t7,$t5, endgame

#3rd
slt $t8,$s3,$0
beq $t8,$t5, endgame  #snakey < 0

#4th
slt $t9,$s1,$s3
beq $t9,$t5, endgame


# display function
sw $s2, 65424($0) # 0xFF90 = monitor x coordinate
sw $s3, 65428($0) # 0xFF94 = monitor y coordinate
sw $t1, 65432($0) # 0xFF98 = monitor color
sw $0, 65436($0)  # 0xFF9c = write pixel 
sw $t5, 0($t3)

j startloop

endgame:
    addi $a0, $0, 0       # frequency 0 to stop sound.
    addi $v0, $0, 6
    syscall

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
    

