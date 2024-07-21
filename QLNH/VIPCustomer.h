#pragma once
#include "Customer.h"

class VIPCustomer :
    public Customer
{
private:
    static int numVIPOrder, totalVIPPrice;
public:
    void TransferIn4FromStdAcc();
};

