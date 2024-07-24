#pragma once
#include "Header.h"

class Person
{
protected:
	wstring PersonID, PersonName, PersonPhNum;
public:
	Person* createPersonByAccountType(int AccType);
	virtual wstring Login(SQLHSTMT &hStmt);
	void Logout();
	virtual void Input(SQLHSTMT &hStmt, wstring PhNum);
	void Output();
	void ChangeAccIn4();
};

