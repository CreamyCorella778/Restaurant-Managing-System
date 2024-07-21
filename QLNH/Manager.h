#pragma once
#include "Staff.h"

class Manager :
    public Staff
{
public:
    void CalculateTotalBrandSale();
    void CreateDiscountEvent();
    void CreateBranch();
};

