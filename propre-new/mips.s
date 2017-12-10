.data
	a:	.word 0
	b:	.word 0
	c:	.word 0
	str0:	.asciiz "Hello world !\n"
    tmp0:	.word 0
	tmp1:	.word 0
	tmp2:	.word 0
	tmp3:	.word 0
	tmp4:	.word 0
	tmp5:	.word 0
	tmp6:	.word 0
	tmp7:	.word 0
	tmp8:	.word 0
	tmp9:	.word 0
	tmp10:	.word 0
	tmp11:	.word 0

.text

main:

#BOP_MULT tmp2 <- 1 * 2
li $t0, 1
li $t1, 2
mul $t0, $t0, $t1
sw $t0, tmp2

#BOP_DIV tmp1 <- tmp2 / 2
lw $t0, tmp2
li $t1, 2
div $t0, $t0, $t1
sw $t0, tmp1

#BOP_PLUS tmp4 <- 1 + 1
li $t0, 1
li $t1, 1
add $t0, $t0, $t1
sw $t0, tmp4

#BOP_MINUS tmp3 <- tmp4 - 1
lw $t0, tmp4
li $t1, 1
sub $t0, $t0, $t1
sw $t0, tmp3

#BOP_PLUS tmp0 <- tmp3 + tmp1
lw $t0, tmp3
lw $t1, tmp1
add $t0, $t0, $t1
sw $t0, tmp0

#COPY a <- tmp0
lw $t0, tmp0
sw $t0, a

#COPY b <- 5
li $t0, 5
sw $t0, b

#BOP_MINUS tmp7 <- b - 1
lw $t0, b
li $t1, 1
sub $t0, $t0, $t1
sw $t0, tmp7

#BOP_PLUS tmp6 <- tmp7 + 1
lw $t0, tmp7
li $t1, 1
add $t0, $t0, $t1
sw $t0, tmp6

#BOP_MULT tmp5 <- a * tmp6
lw $t0, a
lw $t1, tmp6
mul $t0, $t0, $t1
sw $t0, tmp5

#COPY c <- tmp5
lw $t0, tmp5
sw $t0, c

#BOP_PLUS tmp8 <- c + 1
lw $t0, c
li $t1, 1
add $t0, $t0, $t1
sw $t0, tmp8

#COPY c <- tmp8
lw $t0, tmp8
sw $t0, c

#BOP_MINUS tmp9 <- c - 1
lw $t0, c
li $t1, 1
sub $t0, $t0, $t1
sw $t0, tmp9

#COPY c <- tmp9
lw $t0, tmp9
sw $t0, c

#BOP_PLUS tmp10 <- c + 1
lw $t0, c
li $t1, 1
add $t0, $t0, $t1
sw $t0, tmp10

#COPY c <- tmp10
lw $t0, tmp10
sw $t0, c

#BOP_MINUS tmp11 <- c - 1
lw $t0, c
li $t1, 1
sub $t0, $t0, $t1
sw $t0, tmp11

#COPY c <- tmp11
lw $t0, tmp11
sw $t0, c

#CALL_PRINT print str0
la $a0, str0
li $v0, 4
syscall

#CALL_PRINT print 42
li $a0, 42
li $v0, 1
syscall

#CALL_PRINT print c
lw $a0, c
li $v0, 1
syscall

#EXIT
li $v0, 10
syscall
