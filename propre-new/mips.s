.data
	`�:	.asciiz "Hello world !"
.text

main:

#CALL_PRINT print `�
la $a0, `�
li $v0, 4
syscall

#EXIT
li $v0, 10
syscall
