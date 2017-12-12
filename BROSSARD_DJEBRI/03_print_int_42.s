.data
	val_var:	.word 0
	str0:	.asciiz "\n"

.text

main:

#COPY val_var <- 42
li $t0, 42
sw $t0, val_var

#CALL_PRINT print val_var
lw $a0, val_var
li $v0, 1
syscall

#CALL_PRINT print str0
la $a0, str0
li $v0, 4
syscall

#EXIT
li $v0, 10
syscall
