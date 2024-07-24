#pragma once
#include "Person.h"

class Customer :
    public Person
{
protected:
    int CustomerDiscount;
public:
    wstring Login(SQLHSTMT &hStmt) override;
    void Input(SQLHSTMT &hStmt, wstring PhNum) override;
    Customer* IdentifyCustomerType(SQLHSTMT &hStmt)
    void ReserveATable();
    void SelectAMenu();
    void ReviewAfterEating();
    void ChangeReservationIn4();
    void ViewMenu();
    void ViewReserveHistory();
    void ViewReceipt();
};

