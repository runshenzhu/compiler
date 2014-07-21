.data
_ret: .asciiz "\n"
.globl main
.text

write:
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, _ret
	syscall
	move $v0, $0
	jr $ra
ADD:
	add $t9, $a0, $a1	#gen add code
	move $v0, $t9	#gen return code
	jr $ra	#gen return address code
SUB:
	sub $t9, $a0, $a1	#gen sub code
	move $v0, $t9	#gen return code
	jr $ra	#gen return address code
MUL:
	mul $t9, $a0, $a1	#gen mult code
	move $v0, $t9	#gen return code
	jr $ra	#gen return address code
DIV:
	div $a0, $a1	#gen div code
	mflo $t9
	move $v0, $t9	#gen return code
	jr $ra	#gen return address code
main:
	li $t9, 6
	move, $t1 $t9
	li $t9, 2
	move, $t2 $t9
	move $a0, $t1	#copy and rewrite ParamSReg
	move $a1, $t2	#copy and rewrite ParamSReg
	addi $sp, $sp, -4	#gen general func jump code
	sw $ra, 0($sp)
	jal ADD
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	move, $t0 $v0
	move $a0, $t0	#copy and rewrite ParamSReg
	addi $sp, $sp, -4	#gen general func jump code
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	move $a0, $t1	#copy and rewrite ParamSReg
	move $a1, $t2	#copy and rewrite ParamSReg
	addi $sp, $sp, -4	#gen general func jump code
	sw $ra, 0($sp)
	jal SUB
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	move, $t0 $v0
	move $a0, $t0	#copy and rewrite ParamSReg
	addi $sp, $sp, -4	#gen general func jump code
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	move $a0, $t1	#copy and rewrite ParamSReg
	move $a1, $t2	#copy and rewrite ParamSReg
	addi $sp, $sp, -4	#gen general func jump code
	sw $ra, 0($sp)
	jal MUL
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	move, $t0 $v0
	move $a0, $t0	#copy and rewrite ParamSReg
	addi $sp, $sp, -4	#gen general func jump code
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	move $a0, $t1	#copy and rewrite ParamSReg
	move $a1, $t2	#copy and rewrite ParamSReg
	addi $sp, $sp, -4	#gen general func jump code
	sw $ra, 0($sp)
	jal DIV
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	move, $t0 $v0
	move $a0, $t0	#copy and rewrite ParamSReg
	addi $sp, $sp, -4	#gen general func jump code
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t9, 0
	move $v0, $t9	#gen return code
	jr $ra	#gen return address code
