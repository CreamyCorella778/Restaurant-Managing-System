#include "StandardCustomer.h"

void StandardCustomer::Input(SQLHSTMT &hStmt, wstring PhNum)
{
    RETCODE     RetCode;
    SQLSMALLINT sNumResults;

        wstring sqlQuery = L"select k.makh, k.tenkh, k.sdt from khachhang k where k.sdt = " + PhNum;

        // Execute the query
        SQLWCHAR* wszInput = new SQLWCHAR[sqlQuery.size() + 1];
        memcpy(wszInput, sqlQuery.c_str(), sqlQuery.size() + 1);
        RetCode = SQLExecDirect(hStmt, wszInput, SQL_NTS);

        switch (RetCode)
        {
        case SQL_SUCCESS_WITH_INFO:
        {
            HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, RetCode);
            // fall through
        }
        case SQL_SUCCESS:
        {
            // If this is a row-returning query, handle results
            TRYODBC(hStmt, SQL_HANDLE_STMT, SQLNumResultCols(hStmt, &sNumResults));
            if (sNumResults > 0) // Noi xu ly ket qua cau lenh 
            {
                BINDING* pFirstBinding, * pThisBinding; // 1 dslk co dau la FirstBinding, con tro chi so la ThisBinding
                SQLSMALLINT     cDisplaySize;
                RetCode = SQL_SUCCESS;
                int             iCount = 0;

                // Allocate memory for each column
                AllocateBindings(hStmt, sNumResults, &pFirstBinding, &cDisplaySize);

                // Fetch and work with the data
                bool fNoData = false;
                do {
                    // Fetch a row
                    TRYODBC(hStmt, SQL_HANDLE_STMT, RetCode = SQLFetch(hStmt));

                    if (RetCode == SQL_NO_DATA_FOUND)
                    {                    }
                    else // Work with the data. Ignore truncations
                    {
                        pThisBinding = pFirstBinding;
                        this->PersonID = pThisBinding->wszBuffer;
                        pThisBinding = pThisBinding->sNext;
                        if (pThisBinding->indPtr != SQL_NULL_DATA)
                            this->PersonName = pThisBinding->wszBuffer;
                        pThisBinding = pThisBinding->sNext;
                        if (pThisBinding->indPtr != SQL_NULL_DATA)
                            this->PersonPhNum = pThisBinding->wszBuffer;
                        pThisBinding = pThisBinding->sNext;
                        pThisBinding = pThisBinding->sNext;
                        wstring LoaiKH;
                        if (pThisBinding->indPtr != SQL_NULL_DATA)
                            LoaiKH = pThisBinding->wszBuffer;
                        this->CustomerDiscount = 0;
                    }
                } while (!fNoData);

            Exit:
                // Clean up the allocated buffers

                while (pFirstBinding)
                {
                    pThisBinding = pFirstBinding->sNext;
                    free(pFirstBinding->wszBuffer);
                    free(pFirstBinding);
                    pFirstBinding = pThisBinding;
                }
            }
        }

        case SQL_ERROR:
        {
            HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, RetCode);
            break;
        }

        default:
            fwprintf(stderr, L"Unexpected return code %hd!\n", RetCode);

        }
        // Giai phong tai nguyen lien quan den truy van
        TRYODBC(hStmt, SQL_HANDLE_STMT, SQLFreeStmt(hStmt, SQL_CLOSE));
}