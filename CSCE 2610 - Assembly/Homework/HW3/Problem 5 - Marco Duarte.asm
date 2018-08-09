	.data
x:	.asciiz "Enter a value for x: "
y:	.asciiz "Enter a value for y: "
result:	.asciiz "result = "
	.text
	
	la $a0, x
	li $v0, 4
	syscall #"Enter a value for x: "
	
	li $v0, 5
	syscall #read value
	
	move $t0, $v0 #copy read value to temp register
	
	la $a0, y
	li $v0, 4
	syscall #"Enter a value for y: "
	
	li $v0, 5
	syscall #read value
	
	move $t1, $v0 #copy read value to temp register
	
	sll $t2, $t0, 1 #2x
	sub $t2, $t2, $t1 # 2x - y
	sll $t2, $t2, 5 # (2x-y)*32
	
	la $a0, result
	li $v0, 4
	syscall #"result = "
	
	move $a0, $t2
	li $v0, 1
	syscall #print result
	 
	
	
	 
	
		