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

        cmp bl, VECTOR_SIZE

        jb .strcmp_vectorized

        call strcmp

        jmp .func_end

.strcmp_vectorized:

        call strcmp             ; todo vectorized

        jmp .func_end

.not_equal:

        mov rax, 1

.func_end:

        pop rcx
        pop rbx

        pop rbp
        ret
