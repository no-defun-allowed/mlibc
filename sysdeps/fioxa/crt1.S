.section .text
.global _start
_start:
        /* fudge auxiliary vector, envp argv, argc */
        xor %eax, %eax
        push %rax               // null auxv entry
        push %rax               // envp terminator
        push %rax               // argv terminator
        push %rax               // argc = 0
	mov %rsp, %rdi
	mov $main, %rsi
	call __mlibc_entry
.section .note.GNU-stack,"",%progbits

