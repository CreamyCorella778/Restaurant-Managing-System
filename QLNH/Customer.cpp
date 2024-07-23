#include "Customer.h"

void Customer::Login(SQLHSTMT& hStmt)
{
    wstring PhNum, Password, PasswordFromDB;
    bool endLoop = false;
    int count = 0;

    RETCODE     RetCode;
    SQLSMALLINT sNumResults;

    while (!endLoop)
    {
        wcout << L"Nhap sdt: "; wcin >> PhNum;

        wstring sqlQuery = L"select password from customer c where c.sdt = " + PhNum;

        // Execute the query
        WCHAR* wszInput = new WCHAR[sqlQuery.size() + 1];
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
                    {
                        // In ra ket qua khong tim thay khach hang co sdt, nhap lai
                        fNoData = true;
                        // Giai phong bo nho cu
                        while (pFirstBinding)
                        {
                            pThisBinding = pFirstBinding->sNext;
                            free(pFirstBinding->wszBuffer);
                            free(pFirstBinding);
                            pFirstBinding = pThisBinding;
                        }
                        ++count;
                        if (count > 5)
                        {
                            // in ra rang da het so lan nhap
                            fNoData = true;
                        }
                        else
                            continue;
                    }
                    else // Work with the data. Ignore truncations
                    {
                        for (pThisBinding = pFirstBinding; pThisBinding; pThisBinding = pThisBinding->sNext)
                        {
                            if (pThisBinding->indPtr != SQL_NULL_DATA)
                            {
                                PasswordFromDB = wstring(pThisBinding->wszBuffer);
                                endLoop = true;
                            }
                            else
                            {
                                // Giai phong bo nho cu
                                while (pFirstBinding)
                                {
                                    pThisBinding = pFirstBinding->sNext;
                                    free(pFirstBinding->wszBuffer);
                                    free(pFirstBinding);
                                    pFirstBinding = pThisBinding;
                                }
                                ++count;
                                if (count > 5)
                                {
                                    // in ra rang da het so lan nhap
                                    fNoData = true;
                                }
                                else
                                    continue;
                            }
                        }
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
            else // Neu lenh SQL la insert, delete, update, create,...
            {
                SQLLEN cRowCount;

                TRYODBC(hStmt,
                    SQL_HANDLE_STMT,
                    SQLRowCount(hStmt, &cRowCount));

                if (cRowCount >= 0)
                {
                    // In ra so dong bi anh huong
                    /*wprintf(L"%Id %s affected\n",
                        cRowCount,
                        cRowCount == 1 ? L"row" : L"rows");*/
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
        // Giai phong tai nguyen lien quan den truy van
        TRYODBC(hStmt,
            SQL_HANDLE_STMT,
            SQLFreeStmt(hStmt, SQL_CLOSE));
    }

    count = 0; endLoop = false;
    while (!endLoop)
    {
        wcout << L"Nhap mat khau: "; getline(wcin, Password);

        // Neu mat khau da luu trong co so du lieu khong phai la mat khau da nhap
        if (PasswordFromDB != Password)
        {
            ++count;
            if (count > 5)
            {
                // in ra rang da het so lan nhap
                endLoop = true;
            }
            else
                continue;
        }
        else // Neu dung mat khau
        {
            // Tra ve dau hieu da dang nhap thanh cong
            endLoop = true;
            this->PersonPhNum = PhNum;
        }
    }
}

void Customer::Input(SQLHSTMT &hStmt)
{
    // bool endLoop = false;
    // int count = 0;

    // RETCODE     RetCode;
    // SQLSMALLINT sNumResults;

    // while (!endLoop)
    // {
    //     wstring sqlQuery = L"select * from customer c where c.sdt = " + this->PersonPhNum;

    //     // Execute the query
    //     SQLWCHAR* wszInput = new SQLWCHAR[sqlQuery.size() + 1];
    //     memcpy(wszInput, sqlQuery.c_str(), sqlQuery.size() + 1);
    //     RetCode = SQLExecDirect(hStmt, wszInput, SQL_NTS);

    //     switch (RetCode)
    //     {
    //     case SQL_SUCCESS_WITH_INFO:
    //     {
    //         HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, RetCode);
    //         // fall through
    //     }
    //     case SQL_SUCCESS:
    //     {
    //         // If this is a row-returning query, handle results
    //         TRYODBC(hStmt, SQL_HANDLE_STMT, SQLNumResultCols(hStmt, &sNumResults));
    //         if (sNumResults > 0) // Noi xu ly ket qua cau lenh 
    //         {
    //             BINDING* pFirstBinding, * pThisBinding; // 1 dslk co dau la FirstBinding, con tro chi so la ThisBinding
    //             SQLSMALLINT     cDisplaySize;
    //             RetCode = SQL_SUCCESS;
    //             int             iCount = 0;

    //             // Allocate memory for each column
    //             AllocateBindings(hStmt, sNumResults, &pFirstBinding, &cDisplaySize);

    //             // Fetch and work with the data
    //             bool fNoData = false;
    //             do {
    //                 // Fetch a row
    //                 TRYODBC(hStmt, SQL_HANDLE_STMT, RetCode = SQLFetch(hStmt));

    //                 if (RetCode == SQL_NO_DATA_FOUND)
    //                 {
    //                     // In ra ket qua khong tim thay khach hang co sdt, nhap lai
    //                     fNoData = true;
    //                     // Giai phong bo nho cu
    //                     while (pFirstBinding)
    //                     {
    //                         pThisBinding = pFirstBinding->sNext;
    //                         free(pFirstBinding->wszBuffer);
    //                         free(pFirstBinding);
    //                         pFirstBinding = pThisBinding;
    //                     }
    //                     ++count;
    //                     if (count > 5)
    //                     {
    //                         // in ra rang da het so lan nhap
    //                         fNoData = true;
    //                     }
    //                     else
    //                         continue;
    //                 }
    //                 else // Work with the data. Ignore truncations
    //                 {
    //                     pThisBinding = pFirstBinding;
    //                     this->PersonID = pThisBinding->wszBuffer;
    //                     pThisBinding = pThisBinding->sNext;
    //                     if (pThisBinding->indPtr != SQL_NULL_DATA)
    //                         this->PersonName = pThisBinding->wszBuffer;
    //                     pThisBinding = pThisBinding->sNext;
    //                     if (pThisBinding->indPtr != SQL_NULL_DATA)
    //                         this->PersonPhNum = pThisBinding->wszBuffer;
    //                     pThisBinding = pThisBinding->sNext;
    //                     pThisBinding = pThisBinding->sNext;
    //                     wstring LoaiKH;
    //                     if (pThisBinding->indPtr != SQL_NULL_DATA)
    //                         LoaiKH = pThisBinding->wszBuffer;
    //                     this->CustomerDiscount = 0;
    //                     for (pThisBinding = pFirstBinding; pThisBinding; pThisBinding = pThisBinding->sNext)
    //                     {
    //                         if (pThisBinding->indPtr != SQL_NULL_DATA)
    //                         {
    //                             // This action only works well when Password & PhNum contain only ASCII chars
    //                             this->PersonID = 
    //                             wstring ws(pThisBinding->wszBuffer);
    //                             string s_pTBBuffer(ws.begin(), ws.end());
    //                             PasswordFromDB = s_pTBBuffer;
    //                             endLoop = true;
    //                         }
    //                         else
    //                         {
    //                             // Giai phong bo nho cu
    //                             while (pFirstBinding)
    //                             {
    //                                 pThisBinding = pFirstBinding->sNext;
    //                                 free(pFirstBinding->wszBuffer);
    //                                 free(pFirstBinding);
    //                                 pFirstBinding = pThisBinding;
    //                             }
    //                             ++count;
    //                             if (count > 5)
    //                             {
    //                                 // in ra rang da het so lan nhap
    //                                 fNoData = true;
    //                             }
    //                             else
    //                                 continue;
    //                         }
    //                     }
    //                 }
    //             } while (!fNoData);

    //         Exit:
    //             // Clean up the allocated buffers

    //             while (pFirstBinding)
    //             {
    //                 pThisBinding = pFirstBinding->sNext;
    //                 free(pFirstBinding->wszBuffer);
    //                 free(pFirstBinding);
    //                 pFirstBinding = pThisBinding;
    //             }
    //         }
    //         else // Neu lenh SQL la insert, delete, update, create,...
    //         {
    //             SQLLEN cRowCount;

    //             TRYODBC(hStmt,
    //                 SQL_HANDLE_STMT,
    //                 SQLRowCount(hStmt, &cRowCount));

    //             if (cRowCount >= 0)
    //             {
    //                 // In ra so dong bi anh huong
    //                 /*wprintf(L"%Id %s affected\n",
    //                     cRowCount,
    //                     cRowCount == 1 ? L"row" : L"rows");*/
    //             }
    //         }
    //         break;
    //     }

    //     case SQL_ERROR:
    //     {
    //         HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, RetCode);
    //         break;
    //     }

    //     default:
    //         fwprintf(stderr, L"Unexpected return code %hd!\n", RetCode);

    //     }
    //     // Giai phong tai nguyen lien quan den truy van
    //     TRYODBC(hStmt,
    //         SQL_HANDLE_STMT,
    //         SQLFreeStmt(hStmt, SQL_CLOSE));
    // }

    // count = 0; endLoop = false;
    // while (!endLoop)
    // {
    //     cout << "Nhap mat khau: "; getline(cin, Password);

    //     // Neu mat khau da luu trong co so du lieu khong phai la mat khau da nhap
    //     if (PasswordFromDB != Password)
    //     {
    //         ++count;
    //         if (count > 5)
    //         {
    //             // in ra rang da het so lan nhap
    //             endLoop = true;
    //         }
    //         else
    //             continue;
    //     }
    //     else // Neu dung mat khau
    //     {
    //         // Tra ve dau hieu da dang nhap thanh cong
    //         endLoop = true;
    //     }
    // }

    
}