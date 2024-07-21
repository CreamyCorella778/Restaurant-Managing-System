#pragma once
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <stdlib.h>
#include <sal.h>

//------------------------

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/*******************************************/
/* Macro to call ODBC functions and        */
/* report an error on failure.             */
/* Takes handle, handle type, and stmt     */
/*******************************************/

#define TRYODBC(h, ht, x)  
/******************************************/
/* Structure to store information about   */
/* a column.
/******************************************/

typedef struct STR_BINDING {
    SQLSMALLINT         cDisplaySize;           /* size to display  */
    WCHAR* wszBuffer;             /* display buffer   */
    SQLLEN              indPtr;                 /* size or null     */
    BOOL                fChar;                  /* character col?   */
    struct STR_BINDING* sNext;                 /* linked list      */
} BINDING;

//------------------------
// Forward reference for osbcsql.cpp
void HandleDiagnosticRecord(SQLHANDLE      hHandle,
    SQLSMALLINT    hType,
    RETCODE        RetCode);

void DisplayResults(HSTMT       hStmt,
    SQLSMALLINT cCols);

void AllocateBindings(HSTMT         hStmt,
    SQLSMALLINT   cCols,
    BINDING** ppBinding,
    SQLSMALLINT* pDisplay);


void DisplayTitles(HSTMT    hStmt,
    DWORD    cDisplaySize,
    BINDING* pBinding);

void SetConsole(DWORD   cDisplaySize,
    BOOL    fInvert);

//------------------------------------
/*****************************************/
/* Some constants                        */
/*****************************************/


#define DISPLAY_MAX 50          // Arbitrary limit on column width to display
#define DISPLAY_FORMAT_EXTRA 3  // Per column extra display bytes (| <data> )
#define DISPLAY_FORMAT      L"%c %*.*s "
#define DISPLAY_FORMAT_C    L"%c %-*.*s "
#define NULL_SIZE           6   // <NULL>
#define SQL_QUERY_SIZE      1000 // Max. Num characters for SQL Query passed in.

#define PIPE                L'|'

SHORT   gHeight = 80;       // Users screen height