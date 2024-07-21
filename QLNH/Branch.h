#pragma once
#include "Address.h"
#include "Datetime.h"
#include "Table.h"
#include "Staff.h"

class Branch
{
private:
	wstring BranchID, BranchName, BranchPhNum;
	Address BranchAddr;
	Datetime BranchOpenTime, BranchCloseTime;
	vector<Table*> TableList;
	vector<Staff*> StaffList;
public:
	void Input();
	void Output();
};

