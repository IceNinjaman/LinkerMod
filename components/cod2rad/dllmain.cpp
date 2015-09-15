#include "stdafx.h"

static char* techsetPath = "waw_pimp/techsets/%s%s.techset";
static char* techiquePath = "waw_pimp/techniques/%s.tech";

LONG WINAPI MyUnhandledExceptionFilter(PEXCEPTION_POINTERS ExceptionInfo)
{
	printf("\n\nEXCEPTION DETECTED:\n");
	printf("EIP: 0x%p\n", ExceptionInfo->ContextRecord->Eip);
	printf("EX0: 0x%p\n", ExceptionInfo->ExceptionRecord->ExceptionInformation[0]);
	printf("EX1: 0x%p\n", ExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
	printf("\n\n");

	return EXCEPTION_CONTINUE_SEARCH;
	//return PageGuard_Check(ExceptionInfo);
}

bool g_initted = false;

BOOL cod2rad_Init()
{
	if (g_initted)
		return TRUE;

	//
	// Disable STDOUT buffering
	//
	setvbuf(stdout, nullptr, _IONBF, 0);

	//
	// Logo to let the user know this loaded
	//
	printf("----> Loading custom cod2rad\n");

	AddVectoredExceptionHandler(TRUE, (PVECTORED_EXCEPTION_HANDLER)MyUnhandledExceptionFilter);

	//
	// Threading patches (4 threads -> MAX_THREADS threads)
	//
	PatchThreading();

	//
	// Enable Techset / Technique Path Redirection
	//
	PatchMemory(0x0042CA85, (PBYTE)&techiquePath, 4);
	PatchMemory(0x0042CB4C, (PBYTE)&techsetPath, 4);

	//
	// Patch the requested IWI version to match BO1
	//
	PatchMemory(0x00417AA7, (PBYTE)"\xEB", 1);

	//
	// Patch Xmodel loading functions to support Black Ops
	//
	Detours::X86::DetourFunction((PBYTE)0x00425220, (PBYTE)&hk_XModelLoadConfigFile);

	PatchMemory(0x0042635A, (PBYTE)"\xEB", 1); // Xmodelsurfs version check
	PatchMemory(0x004264AE, (PBYTE)"\xEB", 1); // Xmodelparts version check
	Detours::X86::DetourFunction((PBYTE)0x004476B5, (PBYTE)&mfh_XModelReadSurface); // 4 byte xmodelsurfs file adjustment (MagicNumber)

	g_initted = true;
	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		return cod2rad_Init();
	}

	return TRUE;
}