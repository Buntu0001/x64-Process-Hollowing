.code

NtWriteVirtualMemory10 proc
             mov r10, rcx
             mov eax, 3Ah
             syscall
             ret
NtWriteVirtualMemory10 endp

NtResumeThread10 proc
             mov r10, rcx
             mov eax, 52h
             syscall
             ret
NtResumeThread10 endp

NtUnmapViewOfSection10 proc
             mov r10, rcx
             mov eax, 2Ah
             syscall
             ret
NtUnmapViewOfSection10 endp

end