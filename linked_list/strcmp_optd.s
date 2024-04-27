VECTOR_SIZE equ 128

SECTION .text

global strcmp_optimized

extern strcmp

strcmp_optimized:

        push rbp
        mov rbp, rsp

        push rbx
        push rcx

        mov bl, BYTE [rdi]
        mov cl, BYTE [rsi]

        sub bl, cl

        jnz .not_equal

        mov bl, BYTE [rdi]
        cmp bl, VECTOR_SIZE

        jb .strcmp_vectorized

.std_strcmp:

        call strcmp

        jmp .func_end

.strcmp_vectorized:

        cmp cl, VECTOR_SIZE
        ja .std_strcmp

        xor rax, rax

        ; ymm2 = [rdi], ymm3 = [rsi]
        vmovdqa ymm1, [rsi]
        vmovdqa ymm3, ymm1
        vmovdqa ymm1, [rdi]
        vmovdqa ymm2, ymm1

        vpcmpeqb ymm1, ymm2, ymm3
        vpmovmskb rax, ymm1

        xor eax, -1

        jmp .func_end

.not_equal:

        mov rax, 1

.func_end:

        pop rcx
        pop rbx

        pop rbp
        ret
