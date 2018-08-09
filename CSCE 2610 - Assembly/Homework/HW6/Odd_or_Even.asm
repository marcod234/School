	.data
msg1:   .asciiz "Enter an integer: "
msg2:	.asciiz " is even."
msg3:	.asciiz " is odd."
	.text
	
main:	li $v0, 4 #system call code for print_string
	la $a0, msg1
	syscall
	
	li $v0, 5 # system call code for read_int
	syscall

	move $s0, $v0
	
	li $v0, 1 #system call code for print_int
	move $a0, $s0
	syscall 
	
	addi $t0, $zero, 2
	div $s0, $t0 #divide by 2
	mfhi $t1 #get remainder
	
	bne $t1, $zero, odd #if there is a remainder number is odd
even:
	la $a0, msg2
	j exit
	
odd:	la $a0, msg3
	j exit

exit:	li $v0, 4 #system call code for print_string
	syscall
	
	li $v0, 10 #system call code for exit
	syscall
