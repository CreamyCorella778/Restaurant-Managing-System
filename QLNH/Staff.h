#pragma once
#include "Person.h"

class Staff :
    public Person
{
protected:
    int StaffStartYear;
public:
    wstring Login(SQLHSTMT &hStmt) override;
    void Input(SQLHSTMT &hStmt, wstring PhNum) override;
    vector<wstring> GetManagerList(SQLHSTMT &hStmt);
    bool IsManager(SQLHSTMT &hStmt);
    void CreateTable(SQLHSTMT &hStmt);
    void ChangeTableStatus(SQLHSTMT &hStmt);
    void RemoveTable(SQLHSTMT &hStmt);
    void AddMenu(SQLHSTMT &hStmt);
    void ChangeMenuIn4(SQLHSTMT &hStmt);
    void RemoveMenu(SQLHSTMT &hStmt);
    void CreateReceipt(SQLHSTMT &hStmt);
    void ChangeReceiptIn4(SQLHSTMT &hStmt);
    void CalculateTotalBranchSale(SQLHSTMT &hStmt);
};

