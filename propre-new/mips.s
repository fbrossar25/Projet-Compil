.data
	`Æ:	.asciiz "Hello world !"
.text

main:

#CALL_PRINT print `Æ
la $a0, `Æ
li $v0, 4
syscall

#EXIT
li $v0, 10
syscall
