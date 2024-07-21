#pragma once
#include "Header.h"

class Person
{
protected:
	wstring PersonID, PersonName, PersonPhNum;
public:
	Person* createPersonByAccountType(int AccType);
	virtual void Login(SQLHSTMT &hStmt);
	void Logout();
	void Input(SQLHSTMT &hStmt);
	void Output();
	void ChangeAccIn4();
};

