.data
	a_var:	.word 0
	b_var:	.word 0
	c_var:	.word 0
	str0:	.asciiz "\n"
	tmp0:	.word 0
	tmp1:	.word 0

.text

main:

#COPY a_var <- 30
li $t0, 30
sw $t0, a_var

#COPY b_var <- 10
li $t0, 10
sw $t0, b_var

#BOP_PLUS tmp1 <- a_var + b_var
lw $t0, a_var
lw $t1, b_var
add $t0, $t0, $t1
sw $t0, tmp1

#BOP_PLUS tmp0 <- tmp1 + 2
lw $t0, tmp1
li $t1, 2
add $t0, $t0, $t1
sw $t0, tmp0

#COPY c_var <- tmp0
lw $t0, tmp0
sw $t0, c_var

#CALL_PRINT print c_var
lw $a0, c_var
li $v0, 1
syscall

#CALL_PRINT print str0
la $a0, str0
li $v0, 4
syscall

#EXIT
li $v0, 10
syscall
