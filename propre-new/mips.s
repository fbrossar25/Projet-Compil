.data
	a:	.word 0
	b:	.word 0
	c:	.word 0
	tmp0:	.word 0
	tmp1:	.word 0

.text

main:

#BOP_PLUS
li $t0, 1
li $t1, 1
add $t0, $t0, $t1
sw $t0, tmp0

#COPY
lw $t0, tmp0
sw $t0, a

#COPY
li $t0, 5
sw $t0, b

#BOP_MULT
lw $t0, a
lw $t1, b
mul $t0, $t0, $t1
sw $t0, tmp1

#COPY
lw $t0, tmp1
sw $t0, c

lw $a0 c
li $v0, 1
syscall

#EXIT
li $v0, 10
syscall
