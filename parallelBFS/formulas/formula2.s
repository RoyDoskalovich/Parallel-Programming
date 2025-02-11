# Tal Achizvi 313555088
# Roy Doskalovich 319088548


.intel_syntax noprefix
.section .data

    ones : .float 1.0, 1.0, 1.0, 1.0
    .balign 16


.section .text
.globl formula2
.globl pai
formula2:
    # rdi = x
    # rsi = y
    # rdx = length

    # initialize the sum
    vxorps xmm6, xmm6, xmm6 # xmm6 = sum (1) = 0.0, 0.0, 0.0, 0.0
    vxorps xmm9, xmm9, xmm9 # xmm9 = sum (1) = 0.0, 0.0, 0.0, 0.0
    vxorps xmm3, xmm3, xmm3 # xmm3 = sum (1) = 0.0, 0.0, 0.0, 0.0
    vxorps xmm4, xmm4, xmm4 # xmm4 = sum (1) = 0.0, 0.0, 0.0, 0.0
    vxorps xmm7, xmm7, xmm7 # xmm7 = sum (1) = 0.0, 0.0, 0.0, 0.0
    vmovaps xmm5, [ones] # xmm5 = product = 1.0, 1.0, 1.0, 1.0
    vmovaps xmm8, [ones] # xmm8 = ones = 1.0, 1.0, 1.0, 1.0

    # loop counters
    xor r8, r8 # byte counter
    xor r9, r9 # loop counter
    sub rdx, 3 # length - 3

.pai_loop:
    cmp r9, rdx # loop counter < length - 3
    jge .pai_end # if not, jump to the end

    vmovaps xmm0, [rdi + r8] # xmm0 = x
    vmovaps xmm1, [rsi + r8] # xmm1 = y

    vmulps xmm7, xmm0, xmm1 # xmm7 = xy
    vaddps xmm7, xmm7, xmm7 # xmm7 = 2xy

    vmulps xmm0, xmm0, xmm0 # xmm0 = x^2
    vmulps xmm1, xmm1, xmm1 # xmm1 = y^2

    vaddps xmm3, xmm0, xmm1 # xmm3 = x^2 + y^2
    vsubps xmm4, xmm3, xmm7 # xmm3 = x^2 + y^2 - 2xy

    vaddps xmm4, xmm4, xmm8 # xmm4 = 1 + x^2 + y^2 - 2xy

    vmulps xmm5, xmm4, xmm5 # xmm5 = product * (1 + x^2 + y^2 - 2xy)

    add r8, 16 # increment byte counter
    add r9, 4 # increment loop counter
    jmp .pai_loop # jump to the beginning of the loop

.pai_end:
    vshufps xmm2, xmm5, xmm5, 0b01001110 # xmm2 = product[1], product[0], product[3], product[2]
    vmulps xmm5, xmm2, xmm5 # xmm5 = product[0] * product[1], product[1] * product[0], product[2] * product[3], product[3] * product[2]
    vshufps xmm2, xmm5, xmm5, 0b10110001 # xmm2 = product[2] * product[3], product[3] * product[2], product[0] * product[1], product[1] * product[0]
    vmulps xmm5, xmm2, xmm5 # xmm5 = product[0] * product[1] * product[2] * product[3], product[1] * product[0] * product[3] * product[2], product[2] * product[3] * product[0] * product[1], product[3] * product[2] * product[1] * product[0]

    add rdx, 3 # length + 3

.pai_rest:
    cmp r9, rdx # loop counter < length
    je .pai_end2 # if not, jump to the end

    movss xmm0, [rdi + r8] # xmm0 = x
    movss xmm1, [rsi + r8] # xmm1 = y
    shufps xmm0, xmm0,  0b00000000 # xmm0 = x, x, x, x
    shufps xmm1, xmm1,  0b00000000 # xmm1 = y, y, y, y

    vmulps xmm7, xmm0, xmm1 # xmm7 = xy
    vaddps xmm7, xmm7, xmm7 # xmm7 = 2xy

    vmulps xmm0, xmm0, xmm0 # xmm0 = x^2
    vmulps xmm1, xmm1, xmm1 # xmm1 = y^2

    vaddps xmm3, xmm0, xmm1 # xmm3 = x^2 + y^2
    vsubps xmm4, xmm3, xmm7 # xmm4 = x^2 + y^2 - 2xy

    vaddps xmm4, xmm4, xmm8 # xmm4 = 1 + x^2 + y^2 - 2xy

    vmulps xmm5, xmm4, xmm5 # xmm5 = product * (1 + x^2 + y^2 - 2xy)

    add r8, 4 # increment byte counter
    add r9, 1 # increment loop counter
    jmp .pai_rest # jump to the beginning of the loop

.pai_end2:

    xor r8, r8 # byte counter
    xor r9, r9 # loop counter
    sub rdx, 3 # length - 3

.sum_loop:
    cmp r9, rdx # loop counter < length - 3
    jge .sum_end # if not, jump to the end

    vmovaps xmm0, [rdi + r8] # xmm0 = x
    vmovaps xmm1, [rsi + r8] # xmm1 = y

    vmulps xmm7, xmm0, xmm1 # xmm7 = xy
    vdivps xmm7, xmm7, xmm5 # xmm7 = xy / product
    vaddps xmm6, xmm7, xmm6 # xmm6 = sum + xy / product

    add r8, 16 # increment byte counter
    add r9, 4 # increment loop counter
    jmp .sum_loop # jump to the beginning of the loop

.sum_end:
    vshufps xmm9, xmm6, xmm6, 0b01001110 # xmm9 = sum[1], sum[0], sum[3], sum[2]
    vaddps xmm6, xmm9, xmm6 # xmm6 = sum[0] + sum[1], sum[1] + sum[0], sum[2] + sum[3], sum[3] + sum[2]
    vshufps xmm9, xmm6, xmm6, 0b10110001 # xmm9 = sum[2] + sum[3], sum[3] + sum[2], sum[0] + sum[1], sum[1] + sum[0]
    vaddps xmm6, xmm9, xmm6 # xmm6 = sum[0] + sum[1] + sum[2] + sum[3], sum[1] + sum[0] + sum[3] + sum[2], sum[2] + sum[3] + sum[0] + sum[1], sum[3] + sum[2] + sum[1] + sum[0]

    add rdx, 3 # length + 3

.sum_rest:
    cmp r9, rdx # loop counter < length
    je .sum_end2 # if not, jump to the end

    movss xmm0, [rdi + r8] # xmm0 = x
    movss xmm1, [rsi + r8] # xmm1 = y
    shufps xmm0, xmm0, 0b00000000 # xmm0 = x, x, x, x
    shufps xmm1, xmm1, 0b00000000 # xmm1 = y, y, y, y

    vmulps xmm7, xmm0, xmm1 # xmm7 = xy
    vdivps xmm7, xmm7, xmm5 # xmm7 = xy / product
    vaddps xmm6, xmm7, xmm6 # xmm6 = sum + xy / product

    add r8, 4 # increment byte counter
    add r9, 1 # increment loop counter
    jmp .sum_rest # jump to the beginning of the loop

.sum_end2:
    vmovaps xmm0, xmm6 # xmm0 = sum
    ret
