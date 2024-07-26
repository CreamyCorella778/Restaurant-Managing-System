#pragma once
#include "Header.h"
#include "Receipt.h"
class Table
{
private:
	wstring TablePos;
	wstring BranchID;
	int SeatCapacity;
	bool TableStatus;
public:
	void Input();
	void Output();
	int getSeatCapacity();
	wstring getTablePos();
	wstring getBranchID();
	bool isTableAvailable();
	void updateStatusLocal(SQLHSTMT &hStmt);
};

