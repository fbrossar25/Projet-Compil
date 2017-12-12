.data
	str0:	.asciiz "Hello, world"

.text

main:

#CALL_PRINT print str0
la $a0, str0
li $v0, 4
syscall

#EXIT
li $v0, 10
syscall
