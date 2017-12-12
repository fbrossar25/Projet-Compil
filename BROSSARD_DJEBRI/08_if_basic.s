.data
	a_var:	.word 0
	b_var:	.word 0
	str0:	.asciiz "NOT "
	str1:	.asciiz "OK !!!"

.text

main:

#COPY a_var <- 40
li $t0, 40
sw $t0, a_var

#COPY b_var <- 10
li $t0, 10
sw $t0, b_var

#CALL_PRINT print str1
la $a0, str1
li $v0, 4
syscall

#EXIT
li $v0, 10
syscall
