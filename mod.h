#pragma once
#include "framework.h"


#define C_LangNameMenuID	528
#define C_CommonLang		(-1)
#define C_MaxLang			240

extern char const g_szIniFile[];
extern char const g_szLangDir[];


void fn_vInit( void );
void fn_vWriteLangToIni( int lId, char *szName );
BOOL fn_bReadLangFromIni( int lId, char *szOutName, unsigned int ulSize );

BOOL fn_bFileExists( char const *szPath );
int fn_lMessageBox( char *szText, UINT uType );
void fn_vCreatePrerequisites( void );