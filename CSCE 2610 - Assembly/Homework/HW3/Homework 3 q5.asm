	.data
input1: .asciiz "Enter value x: "
input2: .asciiz "Enter value y: "
output: .asciiz "result = "	
	.text
	li $v0, 4		# system call code for print_string
	la $a0, input1		# load address of input1
	syscall
	
	li $v0, 5		# system call code for read_int
	syscall
	move $s0, $v0		# s0 = x
	
	li $v0, 4		# system call code for print_string
	la $a0, input2		# load address of input2
	syscall
	
	li $v0, 5		# system call code for read_int
	syscall
	move $s1, $v0		# s1 = y
	
	sll $t0, $s0, 1		# 2*x
	sub $t0, $t0, $s1	# 2x-y
	sll $t0, $t0, 5		# (2x-y) * 32
	
	li $v0, 4		# system call code for print_string
	la $a0, output		# load address of output
	syscall
	
	move $a0, $t0		# copy the result to a0
	li $v0, 1		# system call code for print_int
	syscall
	
	li $v0, 10		# system call code for exit
	syscall