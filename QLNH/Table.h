#pragma once
#include "Header.h"
class Table
{
private:
	wstring TablePos;
	int SeatCapacity;
	bool TableStatus;
public:
	void Input();
	void Output();
	bool isSeatSufficient(Receipt rec);
};

