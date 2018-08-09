	.data
msg1:   .asciiz "Enter the number of values you want to calculate the mean and range of: "
msg2:	.asciiz "Enter "
msg3:	.asciiz " single precision values:"
msg4:	.asciiz "Mean: "
msg5:	.asciiz " Range: "
	.text
	
main:	li $v0, 4 #system call code for print_string
	la $a0, msg1
	syscall
	
	li $v0, 5 # system call code for read_int
	syscall
	
	move $s0, $v0 #number of values to add
	
	li $v0, 4 #system call code for print_string
	la $a0, msg2
	syscall
	
	li $v0, 1 #system call code for print_int
	move $a0, $s0
	syscall
	
	li $v0, 4 #system call code for print_string
	la $a0, msg3
	syscall
	
	add $t0, $zero, $zero #counter
	mtc1 $zero, $f1 #$f1 used as sum
	
	
for:	slt $t1, $t0, $s0 #0 < n
	beq $t1, $zero, exit #$t0 == n
	
	li $v0, 6 #system call code for read_float
	syscall
	
if:	bne $t0, $zero, else #sets initial high/low values, both the same at first
	mov.s $f11, $f0 #high
	mov.s $f10, $f0 #low
	j forend
else:	
	c.lt.s $f11, $f0 #is it greater than max?
	bc1t greater
	c.lt.s $f0, $f10 #is it lower than min?
	bc1t less
	j forend

greater: mov.s $f11, $f0 #new highest value
	j forend

less:	mov.s $f10, $f0 #new lowest value
	j forend
	
forend:	add.s $f1, $f1, $f0 #update sum
	addi $t0, $t0, 1 #increment counter
	j for #loop back

exit:	mtc1 $s0, $f2 #move # of floats into float register
        cvt.s.w $f2, $f2 # convert int to float
        
	div.s $f12, $f1, $f2 #mean

	li $v0, 4 #system call code for print_string
	la $a0, msg4
	syscall
	
	li $v0, 2 #system call code for print_float
	syscall
	
	sub.s $f12, $f11, $f10
	
	li $v0, 4 #system call code for print_string
	la $a0, msg5
	syscall
	
	li $v0, 2 #system call code for print_float
	syscall
	
	li $v0, 10 #system call code for exit
	syscall
