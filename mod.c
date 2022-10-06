#include "mod.h"
#include "framework.h"


char const g_szIniFile[] = ".\\R2TextReplacer.ini";
char const g_szLangDir[] = ".\\Languages";

BOOL g_bIsInit = FALSE;


void fn_vReplaceMenuString( int lId, char const *szName )
{
	char *szLangMenu = malloc(strlen(szName) + 4);
	if ( !szLangMenu ) return;

	sprintf(szLangMenu, "/C:%s", szName);
	FON_g_stGeneral->p_stCommonLanguage->d_pTextsArray[C_LangNameMenuID + lId] = szLangMenu;
}

void fn_vDumpLanguageTable( FON_tdstLanguage *p_stLanguage, char const *szName )
{
	char szFileName[MAX_PATH];
	sprintf(szFileName, "%s\\%s.tbl", g_szLangDir, szName);

	FILE *hFile = fopen(szFileName, "w");
	if ( !hFile )
		return;

	for ( unsigned short i = 0; i < p_stLanguage->uwNumberOfTexts; i++ )
	{
		fprintf(hFile, "%hu=%s\n", i, p_stLanguage->d_pTextsArray[i]);
	}

	fclose(hFile);
}

BOOL fn_bReadLanguageTable( int lId, char const *szName, FON_tdstLanguage *p_stLanguage )
{
	char szFileName[MAX_PATH];
	sprintf(szFileName, "%s\\%s.tbl", g_szLangDir, szName);

	// TODO: error reporting, potential null pointer

	FILE *hFile = fopen(szFileName, "r");
	if ( !hFile )
		return FALSE;

	/* alloc max instead of current because ??? */
	char **a_szLangTable = malloc(p_stLanguage->uwMaxNumberOfTexts * sizeof(char *));
	if ( !a_szLangTable )
	{
		fclose(hFile);
		return FALSE;
	}

	/* replace the original language table */
	p_stLanguage->d_pTextsArray = a_szLangTable;

	for ( unsigned short i = 0; i < p_stLanguage->uwNumberOfTexts; i++ )
	{
		unsigned short uwIndex;
		char szBuffer[1024];

		if ( fscanf(hFile, "%hu=%1023[^\n]\n", &uwIndex, szBuffer) == 2 )
		{
			char *szString = malloc(strlen(szBuffer) + 1);
			if ( !szString ) continue;

			strcpy(szString, szBuffer);
			a_szLangTable[uwIndex] = szString;
		}
	}

	if ( lId != C_CommonLang )
	{
		/* replace menu string */
		fn_vReplaceMenuString(lId, szName);
	}

	fclose(hFile);
	return TRUE;
}

void fn_vDumpAllTables( void )
{
	fn_vDumpLanguageTable(FON_g_stGeneral->p_stCommonLanguage, "Common");

	for ( int i = 0; i < GAM_g_stEngineStructure->ucNbLanguages; i++ )
	{
		fn_vDumpLanguageTable(
			&FON_g_stGeneral->d_sLanguageArray[i],
			GAM_g_stEngineStructure->p_stLanguageTable[i].szLanguageCode
		);
	}
}

void fn_vLoadAllTables( void )
{
	char szLang[C_MaxLang];

	if ( fn_bReadLangFromIni(C_CommonLang, szLang, C_MaxLang) )
		fn_bReadLanguageTable(C_CommonLang, szLang, FON_g_stGeneral->p_stCommonLanguage);

	for ( int i = 0; i < GAM_g_stEngineStructure->ucNbLanguages; i++ )
	{
		if ( fn_bReadLangFromIni(i, szLang, C_MaxLang) )
			fn_bReadLanguageTable(i, szLang, &FON_g_stGeneral->d_sLanguageArray[i]);
	}
}

void fn_vInit( void )
{
	if ( g_bIsInit )
		return;

	ShowCursor(TRUE);

	if ( !fn_bFileExists(g_szIniFile) )
		fn_vCreatePrerequisites();

	if ( strstr(GAM_g_szCmdLine, "-langdump") )
	{
		if ( fn_lMessageBox(
			"WARNING:\nThis will replace existing .tbl files for all base languages!\nAre you sure you want to dump language tables?",
			MB_OKCANCEL | MB_ICONWARNING) == IDOK )
		{
			fn_vDumpAllTables();
		}
	}
	else
	{
		fn_vLoadAllTables();
	}

	ShowCursor(FALSE);
	g_bIsInit = TRUE;
}
