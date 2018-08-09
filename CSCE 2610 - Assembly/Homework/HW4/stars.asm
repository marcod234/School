	.data
msg1:   .ascii "Enter a positive number: "
	.text
	
main: 	li $v0, 4 # system call code for print_string
	la $a0, msg1 # load address of msg1
	syscall
	li $v0, 5 # system call code for read_int
	syscall
	move $a0, $v0 # value entered
	addi $t0, $zero, 1 #stars/counter
	addi $t1, $a0, 1 #lines
	
for:	slt $t2, $t0, $t1
	beq $t2, $zero, exit #counter >= #lines
	add $t3, $zero, 1 #2nd counter
	j for2
	
for2:	slt $t4, $t3, $t1
	beq $t4, $zero, exit2

if:	sub $t5, $t1, $t0
	slt $t6, $t3, $t5
	beq $t6, $zero, else
	la $a0, ' ' # load address of msg1
	j exitif

else:	la $a0, '*' # load address of msg1

exitif:	li $v0, 11 # system call code for print_character
	syscall
	addi $t3, $t3, 1
	j for2
	
exit2:	li $v0, 11 # system call code for print_string
	la $a0, '\n' # load address of line
	syscall
	addi $t0, $t0, 1
	j for
	
	
exit:	li $v0, 10
	syscall	 
