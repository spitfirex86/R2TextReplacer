#include "framework.h"
#include "mod.h"


void fn_vInitGameLoopHook( void )
{
	GAM_fn_vInitGameLoop();
	fn_vInit();
}

__declspec(dllexport)
int ModMain(BOOL bInit)
{
	if ( bInit )
	{
		DetourTransactionBegin();
		DetourAttach((PVOID*)&GAM_fn_vInitGameLoop, (PVOID)fn_vInitGameLoopHook);
		DetourTransactionCommit();
	}
	else
	{
		DetourTransactionBegin();
		DetourDetach((PVOID*)&GAM_fn_vInitGameLoop, (PVOID)fn_vInitGameLoopHook);
		DetourTransactionCommit();
	}

	return 0;
}
