	.data
msg1:   .asciiz "Enter value x: "
msg2:	.asciiz "Enter value y: "
msg3:	.asciiz "Product = "
msg4:	.asciiz "\nQuotient = "
msg5:	.asciiz "\nRemainder = "
	.text
	
main:	li $v0, 4 # system call code for print_string
	la $a0, msg1
	syscall
	
	li $v0, 5 # system call code for read_int
	syscall

	move $s0, $v0 #x
	
	li $v0, 4 # system call code for print_string
	la $a0, msg2
	syscall

	li $v0, 5 # system call code for read_int
	syscall
	
	move $s1, $v0 #y
	
	mul $t0, $s0, $s1 # x * y
	div $s0, $s1 # x / y
	mflo $t1 #get quotient
	mfhi $t2 #get remainder
	
	li $v0, 4 #system call code for print_string
	la $a0, msg3
	syscall
	
	li $v0, 1 #print product
	move $a0, $t0
	syscall
	
	li $v0, 4 #system call code for print_string
	la $a0, msg4
	syscall
	
	li $v0, 1 #print quotient
	move $a0, $t1
	syscall
	
	li $v0, 4 #system call code for print_string
	la $a0, msg5
	syscall
	
	li $v0, 1 #print remainder
	move $a0, $t2
	syscall

	li $v0, 10 #system call code for exit
	syscall
