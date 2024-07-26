#pragma once
#include "Header.h"
#include "Datetime.h"
#include "Branch.h"
#include "Menu.h"
#include "Customer.h"
#include "Review.h"

class Receipt
{
private:
	wstring ReceiptID;
	Datetime ReserveTime, EatTime;
	int PersonCount;
	Branch* BranchIn4;
	Table* TableIn4;
	Menu* MenuIn4;
	Customer* CustomerIn4;
	Review* ReviewIn4;
public:
	Receipt(wstring id, Datetime rt, Datetime et, int pc, Branch* br, Table* tbl, Menu* mn, Customer* cus, Review* rv);
	void Input();
	void Output();
	bool isAvailableForChanges();
};

