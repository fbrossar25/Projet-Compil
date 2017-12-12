.data
	a_var:	.word 0
	b_var:	.word 0
	c_var:	.word 0
	d_var:	.word 0
	str0:	.asciiz "\n"
	tmp0:	.word 0
	tmp1:	.word 0
	tmp2:	.word 0
	tmp3:	.word 0

.text

main:

#COPY a_var <- 40
li $t0, 40
sw $t0, a_var

#COPY b_var <- 10
li $t0, 10
sw $t0, b_var

#COPY c_var <- 2
li $t0, 2
sw $t0, c_var

#BOP_DIV tmp3 <- b_var / 1
lw $t0, b_var
li $t1, 1
div $t0, $t0, $t1
sw $t0, tmp3

#BOP_DIV tmp2 <- tmp3 / c_var
lw $t0, tmp3
lw $t1, c_var
div $t0, $t0, $t1
sw $t0, tmp2

#BOP_PLUS tmp1 <- a_var + tmp2
lw $t0, a_var
lw $t1, tmp2
add $t0, $t0, $t1
sw $t0, tmp1

#BOP_MINUS tmp0 <- tmp1 - 3
lw $t0, tmp1
li $t1, 3
sub $t0, $t0, $t1
sw $t0, tmp0

#COPY d_var <- tmp0
lw $t0, tmp0
sw $t0, d_var

#CALL_PRINT print d_var
lw $a0, d_var
li $v0, 1
syscall

#CALL_PRINT print str0
la $a0, str0
li $v0, 4
syscall

#EXIT
li $v0, 10
syscall
