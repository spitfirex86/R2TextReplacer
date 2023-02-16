#include "mod.h"
#include "framework.h"


BOOL fn_bFileExists( char const *szPath )
{
	DWORD dwAttrib = GetFileAttributes(szPath);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

int fn_lMessageBox( char *szText, UINT uType )
{
	return MessageBoxA(GAM_fn_hGetWindowHandle(), szText, "R2TextReplacer", uType);
}

void fn_vCreatePrerequisites( void )
{
	CreateDirectory(g_szLangDir, NULL);

	fn_vWriteLangToIni(C_CommonLang, "");
	for ( int i = 0; i < GAM_g_stEngineStructure->ucNbLanguages; i++ )
	{
		fn_vWriteLangToIni(i, "");
	}
}

void fn_vWriteLangToIni( int lId, char *szName )
{
	char szId[8];

	if ( lId == C_CommonLang )
		strcpy(szId, "Common");
	else
		sprintf(szId, "%d", lId);

	WritePrivateProfileString("Language Tables", szId, szName, g_szIniFile);
}

BOOL fn_bReadLangFromIni( int lId, char *szOutName, unsigned int ulSize )
{
	char szId[8];

	if ( lId == C_CommonLang )
		strcpy(szId, "Common");
	else
		sprintf(szId, "%d", lId);

	return GetPrivateProfileString("Language Tables", szId, "", szOutName, ulSize, g_szIniFile);
}
