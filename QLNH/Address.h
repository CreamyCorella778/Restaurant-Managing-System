#pragma once
#include "Header.h"

class Address
{
private:
	wstring Province, City, District, Ward, Road, Number;
public:
	void Input();
	void Output();
	void StringToAddress(wstring str);
};

