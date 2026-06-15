.section .text
.global _start
_start:
        call fioxa_fudge_sysv_stack
        mov %rax, %rdi
        mov $main, %rsi
        call __mlibc_entry
.section .note.GNU-stack,"",%progbits

