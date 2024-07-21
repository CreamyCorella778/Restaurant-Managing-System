#pragma once
#include "Person.h"

class Customer :
    public Person
{
protected:
    int CustomerDiscount;
public:
    void Login(SQLHSTMT &hStmt) override;
    void ReserveATable();
    void SelectAMenu();
    void ReviewAfterEating();
    void ChangeReservationIn4();
    void ViewMenu();
    void ViewReserveHistory();
    void ViewReceipt();
};

