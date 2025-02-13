.intel_syntax noprefix

.section .data
# EOS_mask: db 0x1,0xFF times 14 db 0
EOS_mask:
    .byte 0x1,0xFF
    .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0

.section .text

.globl strlen1
strlen1:
    push rbp
    mov rbp, rsp

    # Zero our counting registers
    xor rax, rax
    xor rcx, rcx

    # Load mask into xmm1
    lea rsi, EOS_mask
    movdqu xmm1, [rsi]

.loop_strlen1:
    add rax, rcx
    pcmpistri xmm1, [rdi+rax], 0b00010100
    jnz .loop_strlen1

    add rax, rcx

    pop rbp
    ret

.globl hamming_dist
hamming_dist:
    push rbp
    mov rbp, rsp

    xor rax, rax
    xor rcx, rcx
    xor r9, r9

    movdqu xmm2, [EOS_mask]

    .loop:
    movdqu xmm0, [rdi + rax] # Loads 16 Bytes of str1 into xmm0
    movdqu xmm1, [rsi + rax] # Loads 16 Bytes of str2 into xmm1

    pcmpistri xmm2, xmm0, 0x14 # rcx <= '\0' index (or 16 if str1 is not ending in this chunk).
    mov r8, rcx
    pcmpistri xmm2, xmm1, 0x14 # rcx <= '\0' index (or 16 if str2 is not ending in this chunk).

    cmp r8, 16
    jl .str1_end # Goto .str1_end in case that str1 ends in this chunk.

    cmp rcx, 16
    jl .str2_end # Goto .str2_end in case that str2 ends in this chunk.

    # Both of the string are not ending in this current chunk.
    pcmpeqb xmm0, xmm1 # xmm0 <= 0xFF for equal bytes, 0x00 for unequal bytes.
    pmovmskb r11, xmm0 # r11 <= msb of each byte of xmm0.
    popcnt r11, r11 # Count the amout of differences. (count the amout of bytes that their msb is 1).
    mov r12, 16
    sub r12, r11 # r12 <= 16 - amount of matching bytes in this chunk.
    add r9, r12 # add it the counter r9.
    add rax, 16
    jmp .loop

    # str1 is ending in this current chunk.
    .str1_end:
    cmp rcx, 16
    jl .both_end # In case both of the strings are ending in this chunk goto .both_end.

    pcmpeqb xmm0, xmm1
    pmovmskb r11, xmm0
    popcnt r11, r11
    mov r12, 16
    sub r12, r11
    add r9, r12 # Adding the amount of unmatiching bytes in this chunk to the counter.

    add rax, 16
    add rsi, rax
    mov rdi, rsi
    call strlen1 # Returns the remaining length of str2 after this chunk.
    add r9, rax # adding the remaining length of str2 to the counter.
    jmp .end

    .str2_end:
    pcmpeqb xmm0, xmm1
    pmovmskb r11, xmm0
    popcnt r11, r11
    mov r12, 16
    sub r12, r11
    add r9, r12

    add rax, 16
    add rdi, rax
    call strlen1 # Returns the remaining length of str1 after this chunk.
    add r9, rax
    jmp .end

    .both_end:
    pcmpeqb xmm0, xmm1 # Note that it would count as matching bytes cases in which both of the bytes are nulls.
    pmovmskb r11, xmm0
    popcnt r11, r11

    mov r12, 16
    sub r12, r11
    add r9, r12
    jmp .end

    .end:
    mov rax, r9

    pop rbp
    ret

