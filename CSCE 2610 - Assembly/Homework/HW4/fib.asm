	.data
msg1:   .ascii "Calculate Fibonacci number at which place?: "
	.text
	
main:	li $v0, 4 # system call code for print_string
	la $a0, msg1 # load address of msg1
	syscall
	li $v0, 5 # system call code for read_int
	syscall
	move $a0, $v0 # value entered
	jal fib
	li $v0, 1 #print_int call code
	move $a0, $s3 #value v[i]
	syscall
	li $v0, 10 #exit call code
	syscall

fib:	addi $sp, $sp, -20
	sw $ra, 16($sp)
	sw $s3, 12($sp) # fibonacci outcome
	sw $s2, 8($sp)	# temp 1
	sw $s1, 4($sp)	# temp 2
	sw $s0, 0($sp)	# nth place
	move $s0, $a0 

if:	bne $s0, $zero, elif #n=0
	add $s3, $zero, $zero # fib(0)=0
	j exitif

elif:	bne $s0, 1, else #n=1
	addi $s3, $zero, 1 #fib(1)=1
	j exitif

else:	slt $t0, $s0, $zero 
	bne $t0, $zero, exitif # branch to exit if a negative number is given
	addi $a0, $a0, -1
	jal fib #fib(n-1)
	move $s1, $s3
	move $a0, $s0
	addi $a0, $a0, -2
	jal fib #fib(n-2)
	move $s2, $s3
	add $s3, $s1, $s2 #fib(n-1)+fib(n-2)
	
exitif: lw $ra, 16($sp) # load previous location
	lw $s2, 8($sp)	# load previous temp values
	lw $s1, 4($sp)	# 
	lw $s0, 0($sp)	# load n at previous call
	addi $sp, $sp, 20 # restore stack
	jr $ra 
	
	
