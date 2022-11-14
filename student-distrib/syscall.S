#define ASM 1
.global systemcall,done,invalid,sysJumpTable
systemcall:
    pushl   %esi
    pushl   %edi
    pushl   %esp
    pushl   %edx
    pushl   %ecx
    pushl   %ebx
    cmp     $10, %eax
    jg invalid
    cmp     $1, %eax
    jl invalid

    # valid, call the function
    call *sysJumpTable(,%eax,4)
done:
    popl %ebx
    popl %ecx
    popl %edx
    popl %esp
    popl %edi
    popl %esi    
    iret
    
invalid:
    movl    $-1, %eax
    jmp done

sysJumpTable:
    .long   0x0
    .long   halt
    .long   execute
    .long   read
    .long   write
    .long   open
    .long   close
    .long   getargs
    .long   vidmap
    .long   set_handler
    .long   sigreturn


