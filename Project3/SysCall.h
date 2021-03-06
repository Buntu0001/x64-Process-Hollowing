#pragma once

#include <windows.h>

extern "C" NTSTATUS NtWriteVirtualMemory10(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress,
	IN PVOID                Buffer,
	IN ULONG                NumberOfBytesToWrite,
	OUT PULONG              NumberOfBytesWritten OPTIONAL
);

extern "C" NTSTATUS NtResumeThread10(
	IN HANDLE               ThreadHandle,
	OUT PULONG              SuspendCount OPTIONAL
);

extern "C" NTSTATUS NtUnmapViewOfSection10(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress
);

NTSTATUS (*NtWriteVirtualMemory)(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress,
	IN PVOID                Buffer,
	IN ULONG                NumberOfBytesToWrite,
	OUT PULONG              NumberOfBytesWritten OPTIONAL
);

NTSTATUS (*NtResumeThread)(
	IN HANDLE               ThreadHandle,
	OUT PULONG              SuspendCount OPTIONAL
);


NTSTATUS (*NtUnmapViewOfSection)(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress
);