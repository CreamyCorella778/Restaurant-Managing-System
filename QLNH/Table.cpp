#include "Table.h"

void Table::updateStatusLocal(SQLHSTMT &hStmt)
{
    RETCODE     RetCode;
    SQLSMALLINT sNumResults;

    wstring sqlQuery = L"select * FROM hoadon hd WHERE hd.macn = ? and hd.toadoba = ? and (hd.ngaygioan <= getdate() or hd.ngaygioan is null)";
    WCHAR* wszInput = new WCHAR[sqlQuery.size() + 1];
    memcpy(wszInput, sqlQuery.c_str(), sqlQuery.size() + 1);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)this->BranchID.c_str(), 0, nullptr);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)this->TablePos.c_str(), 0, nullptr);
    RetCode = SQLExecDirect(hStmt, wszInput, SQL_NTS);
    delete[] wszInput;

    switch (RetCode)
    {
    case SQL_SUCCESS_WITH_INFO:
    {
        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, RetCode);
    }
    case SQL_SUCCESS:
    {
        TRYODBC(hStmt, SQL_HANDLE_STMT, SQLNumResultCols(hStmt, &sNumResults));
        if (sNumResults > 0) 
        {
            BINDING* pFirstBinding, * pThisBinding;
            SQLSMALLINT     cDisplaySize;
            RetCode = SQL_SUCCESS;

            AllocateBindings(hStmt, sNumResults, &pFirstBinding, &cDisplaySize);

            bool fNoData = false, fHasDataAlready = false; 
            do {
                TRYODBC(hStmt, SQL_HANDLE_STMT, RetCode = SQLFetch(hStmt));
                if (RetCode == SQL_NO_DATA_FOUND && !fHasDataAlready)
                {
                    fNoData = true;
                    this->TableStatus = false;
                }
                else if (RetCode == SQL_NO_DATA_FOUND)
                    fNoData = true;
                else
                {
                    this->TableStatus = true;
                    fHasDataAlready = true;
                }
                    
            } while (!fNoData);

        Exit:
            while (pFirstBinding)
            {
                pThisBinding = pFirstBinding->sNext;
                free(pFirstBinding->wszBuffer);
                free(pFirstBinding);
                pFirstBinding = pThisBinding;
            }
        }
        break;
    }
    case SQL_ERROR:
    {
        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, RetCode);
        break;
    }
    default:
        fwprintf(stderr, L"Unexpected return code %hd!\n", RetCode);

    }
    TRYODBC(hStmt, SQL_HANDLE_STMT, SQLFreeStmt(hStmt, SQL_CLOSE));
}

bool Table::isTableAvailable()
{
    return !this->TableStatus;
}

int Table::getSeatCapacity()
{
    return this->SeatCapacity;
}

wstring Table::getBranchID()
{
    return this->BranchID;
}

wstring Table::getTablePos()
{
    return this->TablePos;
}