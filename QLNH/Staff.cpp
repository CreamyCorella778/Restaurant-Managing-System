#include "Staff.h"
#include "Table.h"

wstring Staff::Login(SQLHSTMT &hStmt)
{
    wstring PhNum, Password, PasswordFromDB, ID;
    bool endLoop = false;
    int count = 0;

    RETCODE     RetCode;
    SQLSMALLINT sNumResults;

    while (!endLoop)
    {
        wcout << L"Nhap sdt: "; wcin >> PhNum;

        // Using parameterized query to prevent SQL injection
        wstring sqlQuery = L"SELECT manv, matkhau FROM nhanvien WHERE sdt = ?";
        
        WCHAR* wszInput = new WCHAR[sqlQuery.size() + 1];
        memcpy(wszInput, sqlQuery.c_str(), sqlQuery.size() + 1);

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)PhNum.c_str(), 0, nullptr);
        RetCode = SQLExecDirect(hStmt, wszInput, SQL_NTS);
        delete[] wszInput;

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
                // int             iCount = 0;

                // Allocate memory for each column
                AllocateBindings(hStmt, sNumResults, &pFirstBinding, &cDisplaySize);

                // Fetch and work with the data
                bool fNoData = false;
                do {
                    // Fetch a row
                    TRYODBC(hStmt, SQL_HANDLE_STMT, RetCode = SQLFetch(hStmt));

                    if (RetCode == SQL_NO_DATA_FOUND)
                    {
                        // Truong hop truy van khong co ket qua, in ra ket qua khong tim thay khach hang co sdt, nhap lai
                        // pending...
                        // Truong hop truy van het ket qua
                            fNoData = true;
                    }
                    else // Work with the data. Ignore truncations
                    {
                        for (pThisBinding = pFirstBinding; pThisBinding; pThisBinding = pThisBinding->sNext)
                        {
                            if (pThisBinding->indPtr != SQL_NULL_DATA)
                            {
                                ID = wstring(pThisBinding->wszBuffer);
                                endLoop = true;
                                PasswordFromDB = wstring(pThisBinding->sNext->wszBuffer);
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

            if (++count > 5)
            {
                // wcout << L"Da vuot qua so lan thu toi da.\n";
                ID.clear();
                return ID;
            }
            }
        }
        break;

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
        }
        else // Neu dung mat khau
        {
            // Tra ve dau hieu da dang nhap thanh cong
            endLoop = true;
            this->PersonID = ID;
            return ID;
        }
    }
    return L"";
}

vector<wstring> GetManagerList(SQLHSTMT &hStmt)
{
    vector<wstring> ManagerList;
    RETCODE     RetCode;
    SQLSMALLINT sNumResults;

    wstring sqlQuery = L"select distinct manvql from nhanvien where manvql is not null";

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
                    ManagerList.push_back(wstring(pThisBinding->wszBuffer));
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
    return ManagerList;
}

bool Staff::IsManager(SQLHSTMT &hStmt)
{
    vector<wstring> ManagerList = this->GetManagerList(hStmt);
    auto it = find(ManagerList.begin(), ManagerList.end(), this->PersonID);
    return it != ManagerList.end();
}

void Staff::Input(SQLHSTMT &hStmt, wstring PhNum)
{
    RETCODE     RetCode;
    SQLSMALLINT sNumResults;

    wstring sqlQuery = L"select nv.manv, nv.tennv, nv.nambatdau from nhanvien nv where nv.sdt = " + PhNum;

    // Execute the query
    SQLWCHAR* wszInput = new SQLWCHAR[sqlQuery.size() + 1];
    memcpy(wszInput, sqlQuery.c_str(), sqlQuery.size() + 1);
    RetCode = SQLExecDirect(hStmt, wszInput, SQL_NTS);

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
            int             iCount = 0;

            AllocateBindings(hStmt, sNumResults, &pFirstBinding, &cDisplaySize);

            bool fNoData = false;
            do {
                TRYODBC(hStmt, SQL_HANDLE_STMT, RetCode = SQLFetch(hStmt));

                if (RetCode == SQL_NO_DATA_FOUND)
                {                    }
                else
                {
                    pThisBinding = pFirstBinding;
                    this->PersonID = pThisBinding->wszBuffer;
                    pThisBinding = pThisBinding->sNext;
                    if (pThisBinding->indPtr != SQL_NULL_DATA)
                        this->PersonName = pThisBinding->wszBuffer;
                    pThisBinding = pThisBinding->sNext;
                    this->PersonPhNum = PhNum;
                    if (pThisBinding->indPtr != SQL_NULL_DATA)
                        this->StaffStartYear = stoi(wstring(pThisBinding->wszBuffer));
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

void Staff::CreateTable(SQLHSTMT &hStmt)
{
    Table tb;
    tb.Input(); // Input From keyboard via UI

    // Output to Database 

    RETCODE     RetCode;
    SQLSMALLINT sNumResults;
    
    wstring sqlQuery = L"insert into banan values (?, ?, ?)";
    WCHAR* wszInput = new WCHAR[sqlQuery.size() + 1];
    memcpy(wszInput, sqlQuery.c_str(), sqlQuery.size() + 1);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)tb.getBranchID().c_str(), 0, nullptr);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)tb.getTablePos().c_str(), 0, nullptr);
    SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)to_wstring(tb.getSeatCapacity()).c_str(), 0, nullptr);
    RetCode = SQLExecDirect(hStmt, wszInput, SQL_NTS);

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
        { // Khong lam gi ca vi day khong phai la truy van        
        }
        else
        {
            SQLLEN cRowCount;

            TRYODBC(hStmt, SQL_HANDLE_STMT, SQLRowCount(hStmt,&cRowCount));

            if (cRowCount >= 0)
            {
                // In ra so hang bi anh huong
                
                // wprintf(L"%Id %s affected\n",
                //             cRowCount,
                //             cRowCount == 1 ? L"row" : L"rows");
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

void Staff::ChangeTableStatus(SQLHSTMT &hStmt)
{
    Table tb; // The table that needs changing its status
    tb.Input();
    
    // Ban an khong thay doi tinh trang trong csdl duoc
}

void Staff::RemoveTable(SQLHSTMT &hStmt) // Chu y tim cach tao macro/chuong trinh xoa tu dong cac du lieu chua khoa ngoai cua ban an bi xoa
{
    Table tb; // The table that needs removing from the database
    tb.Input();

    RETCODE     RetCode;
    SQLSMALLINT sNumResults;
    
    wstring sqlQuery = L"delete from banan where macn = ? and toadoba = ";
    WCHAR* wszInput = new WCHAR[sqlQuery.size() + 1];
    memcpy(wszInput, sqlQuery.c_str(), sqlQuery.size() + 1);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)tb.getBranchID().c_str(), 0, nullptr);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)tb.getTablePos().c_str(), 0, nullptr);
    RetCode = SQLExecDirect(hStmt, wszInput, SQL_NTS);

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
        { // Khong lam gi ca vi day khong phai la truy van        
        }
        else
        {
            SQLLEN cRowCount;

            TRYODBC(hStmt, SQL_HANDLE_STMT, SQLRowCount(hStmt,&cRowCount));

            if (cRowCount >= 0)
            {
                // In ra so hang bi anh huong
                
                // wprintf(L"%Id %s affected\n",
                //             cRowCount,
                //             cRowCount == 1 ? L"row" : L"rows");
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

void Staff::AddMenu(SQLHSTMT &hStmt)
{
    Menu m;
    m.Input();

    RETCODE     RetCode;
    SQLSMALLINT sNumResults;
    
    wstring sqlQuery = L"insert into thucdon values (?, ?)";
    WCHAR* wszInput = new WCHAR[sqlQuery.size() + 1];
    memcpy(wszInput, sqlQuery.c_str(), sqlQuery.size() + 1);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)m.GetMenuID().c_str(), 0, nullptr);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)to_wstring(m.GetMenuPrice()).c_str(), 0, nullptr);
    RetCode = SQLExecDirect(hStmt, wszInput, SQL_NTS);

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
        { // Khong lam gi ca vi day khong phai la truy van        
        }
        else
        {
            SQLLEN cRowCount;

            TRYODBC(hStmt, SQL_HANDLE_STMT, SQLRowCount(hStmt,&cRowCount));

            if (cRowCount >= 0)
            {
                // In ra so hang bi anh huong
                
                // wprintf(L"%Id %s affected\n",
                //             cRowCount,
                //             cRowCount == 1 ? L"row" : L"rows");
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

    for (int i = 0; i < m.GetDishList().size(); ++i)
    {    
        sqlQuery = L"insert into ct_thucdon values (?, ?)";
        wszInput = new WCHAR[sqlQuery.size() + 1];
        memcpy(wszInput, sqlQuery.c_str(), sqlQuery.size() + 1);
        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)m.GetMenuID().c_str(), 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)m.GetDishList()[i]->GetDishID().c_str(), 0, nullptr);
        RetCode = SQLExecDirect(hStmt, wszInput, SQL_NTS);

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
            { // Khong lam gi ca vi day khong phai la truy van        
            }
            else
            {
                SQLLEN cRowCount;

                TRYODBC(hStmt, SQL_HANDLE_STMT, SQLRowCount(hStmt,&cRowCount));

                if (cRowCount >= 0)
                {
                    // In ra so hang bi anh huong
                    
                    // wprintf(L"%Id %s affected\n",
                    //             cRowCount,
                    //             cRowCount == 1 ? L"row" : L"rows");
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
}

void Customer::ChangeReservationIn4(SQLHSTMT &hStmt)
{
    // Hien thi menu hien thi toan bo nhung noi co the thay doi cung o trong de chinh sua
    // Ghi nhan cac thay doi bang cac bien
    // Chinh sua du lieu trong csdl
    RETCODE     RetCode;
    SQLSMALLINT sNumResults;
    wstring sqlQuery = L"update table menu set ? = ?, ? = ?,... where makh = ? and ngaygioan is null";
    WCHAR* wszInput = new WCHAR[sqlQuery.size() + 1];
    memcpy(wszInput, sqlQuery.c_str(), sqlQuery.size() + 1);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)receiptid.c_str(), 0, nullptr);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)to_wstring(rv.getStars()).c_str(), 0, nullptr);
    SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)rv.getDetails().c_str(), 0, nullptr);
    RetCode = SQLExecDirect(hStmt, wszInput, SQL_NTS);

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
        { // Khong lam gi ca vi day khong phai la truy van        
        }
        else
        {
            SQLLEN cRowCount;

            TRYODBC(hStmt, SQL_HANDLE_STMT, SQLRowCount(hStmt,&cRowCount));

            if (cRowCount >= 0)
            {
                // In ra so hang bi anh huong
                
                // wprintf(L"%Id %s affected\n",
                //             cRowCount,
                //             cRowCount == 1 ? L"row" : L"rows");
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

void Staff::RemoveTable(SQLHSTMT &hStmt) // Chu y tim cach tao macro/chuong trinh xoa tu dong cac du lieu chua khoa ngoai cua ban an bi xoa
{
    Menu m; // The table that needs removing from the database
    m.Input();

    RETCODE     RetCode;
    SQLSMALLINT sNumResults;
    
    wstring sqlQuery = L"delete from thucdon where matd = ?";
    WCHAR* wszInput = new WCHAR[sqlQuery.size() + 1];
    memcpy(wszInput, sqlQuery.c_str(), sqlQuery.size() + 1);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)m.GetMenuID().c_str(), 0, nullptr);
    RetCode = SQLExecDirect(hStmt, wszInput, SQL_NTS);

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
        { // Khong lam gi ca vi day khong phai la truy van        
        }
        else
        {
            SQLLEN cRowCount;

            TRYODBC(hStmt, SQL_HANDLE_STMT, SQLRowCount(hStmt,&cRowCount));

            if (cRowCount >= 0)
            {
                // In ra so hang bi anh huong
                
                // wprintf(L"%Id %s affected\n",
                //             cRowCount,
                //             cRowCount == 1 ? L"row" : L"rows");
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

void Staff::ChangeReceiptIn4(SQLHSTMT &hStmt)
{
    // Hien thi menu hien thi toan bo nhung noi co the thay doi cung o trong de chinh sua
    // Ghi nhan cac thay doi bang cac bien
    // Chinh sua du lieu trong csdl
    RETCODE     RetCode;
    SQLSMALLINT sNumResults;
    wstring sqlQuery = L"update table hoadon set ? = ?, ? = ?,... where makh = ? and ngaygioan is null";
    WCHAR* wszInput = new WCHAR[sqlQuery.size() + 1];
    memcpy(wszInput, sqlQuery.c_str(), sqlQuery.size() + 1);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)receiptid.c_str(), 0, nullptr);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)to_wstring(rv.getStars()).c_str(), 0, nullptr);
    SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)rv.getDetails().c_str(), 0, nullptr);
    RetCode = SQLExecDirect(hStmt, wszInput, SQL_NTS);

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
        { // Khong lam gi ca vi day khong phai la truy van        
        }
        else
        {
            SQLLEN cRowCount;

            TRYODBC(hStmt, SQL_HANDLE_STMT, SQLRowCount(hStmt,&cRowCount));

            if (cRowCount >= 0)
            {
                // In ra so hang bi anh huong
                
                // wprintf(L"%Id %s affected\n",
                //             cRowCount,
                //             cRowCount == 1 ? L"row" : L"rows");
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

Receipt* Staff::CreateReceipt(SQLHSTMT &hStmt, Table table)
{
    RETCODE     RetCode;
    SQLSMALLINT sNumResults;
    bool fTableReady = true;

    if (!table.isTableAvailable())
    {
        //In thong bao table khong kha dung de dat
        return nullptr;
    }

    int PersonCount = 0;
    // Input person count

    if (PersonCount > table.getSeatCapacity())
    {
        // In thong bao du nguoi an, khong dat cho duoc
        return nullptr;
    }

    wstring mahd = L"HD" + randomForIDs();
    wstring today = getTodayDate();
    wstring sqlQuery = L"insert into hoadon (MaHD, NgayGioDat, SoNguoiAn, MaCN, ToaDoBA, MaKH) values (?, ?, ?, ?, ?, ?)";
    WCHAR* wszInput = new WCHAR[sqlQuery.size() + 1];
    memcpy(wszInput, sqlQuery.c_str(), sqlQuery.size() + 1);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)mahd.c_str(), 0, nullptr);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)today.c_str(), 0, nullptr);
    SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)to_wstring(PersonCount).c_str(), 0, nullptr);
    SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)table.getBranchID().c_str(), 0, nullptr);
    SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)table.getTablePos().c_str(), 0, nullptr);
    SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 0, 0, (SQLPOINTER)this->PersonID.c_str(), 0, nullptr);
    RetCode = SQLExecDirect(hStmt, wszInput, SQL_NTS);

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
        { // Khong lam gi ca vi day khong phai la truy van        
        }
        else
        {
            SQLLEN cRowCount;

            TRYODBC(hStmt, SQL_HANDLE_STMT, SQLRowCount(hStmt,&cRowCount));

            if (cRowCount >= 0)
            {
                // In ra so hang bi anh huong
                
                // wprintf(L"%Id %s affected\n",
                //             cRowCount,
                //             cRowCount == 1 ? L"row" : L"rows");
            }
        }
        break;
    }
    case SQL_ERROR:
    {
        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, RetCode);
        return nullptr;
        break;
    }
    default:
        fwprintf(stderr, L"Unexpected return code %hd!\n", RetCode);
        return nullptr;
    }
    TRYODBC(hStmt, SQL_HANDLE_STMT, SQLFreeStmt(hStmt, SQL_CLOSE));
    
    Receipt* res = new Receipt(...);
    return res;

}
