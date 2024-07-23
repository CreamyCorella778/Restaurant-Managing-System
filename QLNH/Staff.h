#pragma once
#include "Person.h"

class Staff :
    public Person
{
protected:
    int StaffStartYear;
public:
    void Login(SQLHSTMT &hStmt) override;
    void Input(SQLHSTMT &hStmt) override;
    void CreateTable();
    void ChangeTableStatus();
    void RemoveTable();
    void AddMenu();
    void ChangeMenuIn4();
    void RemoveMenu();
    void CreateReceipt();
    void ChangeReceiptIn4();
    void CalculateTotalBranchSale();
};

