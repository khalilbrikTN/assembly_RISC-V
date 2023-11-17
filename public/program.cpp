ADDI a0, zero, 6
JAL ra, fact 
ECALL
fact:
ADDI sp, sp, -8
sw ra, 4(sp)
sw a0, 0(sp)
slti t0, a0, 1 
beq t0, zero, L1
addi a0, zero, 1
addi sp, sp, 8
jalr zero, 0(ra)
L1:
addi a0, a0, -1 
jal ra, fact 
addi t0, a0, 0
lw a0, 0(sp)
lw ra, 4(sp)
addi sp, sp, 8
mul a0, a0, t0 
jalr zero, 0(ra)