#pragma once
#include "Person.h"
#include "Table.h"

class Customer :
    public Person
{
protected:
    int CustomerDiscount;
public:
    wstring Login(SQLHSTMT &hStmt) override;
    void Input(SQLHSTMT &hStmt, wstring PhNum) override;
    bool IsVIPCustomer(SQLHSTMT &hStmt);
    Receipt* ReserveATable(SQLHSTMT &hStmt, Table table);
    void SelectAMenu(SQLHSTMT &hStmt);
    void ReviewAfterEating(SQLHSTMT &hStmt);
    void ChangeReservationIn4(SQLHSTMT &hStmt);
    void ViewMenu(SQLHSTMT &hStmt);
    void ViewReserveHistory(SQLHSTMT &hStmt);
    void ViewReceipt(SQLHSTMT &hStmt);
};

