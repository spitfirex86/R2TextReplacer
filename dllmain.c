#include "framework.h"
#include "mod.h"


void (*R2_GAM_fn_vInitGameLoop)( void ) = NULL;

void fn_vInitGameLoopHook( void )
{
	R2_GAM_fn_vInitGameLoop();
	fn_vInit();
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD dwReason, LPVOID lpReserved )
{
	switch ( dwReason )
	{
		case DLL_PROCESS_ATTACH:
		{
			R2_GAM_fn_vInitGameLoop = GAM_fn_vInitGameLoop;

			DetourTransactionBegin();
			DetourAttach((PVOID*)&R2_GAM_fn_vInitGameLoop, (PVOID)fn_vInitGameLoopHook);
			DetourTransactionCommit();
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			DetourTransactionBegin();
			DetourDetach((PVOID*)&R2_GAM_fn_vInitGameLoop, (PVOID)fn_vInitGameLoopHook);
			DetourTransactionCommit();
			break;
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
	}
	return TRUE;
}
