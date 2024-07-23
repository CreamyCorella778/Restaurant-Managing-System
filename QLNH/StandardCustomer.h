#pragma once
#include "Customer.h"

class StandardCustomer :
    public Customer
{
public:
    void PromoteToVIP();
    void Input(SQLHSTMT &hStmt, wstring PhNum) override;
};

