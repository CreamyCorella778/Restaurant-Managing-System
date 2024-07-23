#pragma once
#include "Customer.h"
#include "Staff.h"

void Person::Login(SQLHSTMT &hStmt)
{}

Person* Person::createPersonByAccountType(int AccType)
{
	switch (AccType)
	{
	case 1:
		return new Customer();
		break;
	case 2:
		return new Staff();
		break;
	default:
		return nullptr;
	}
}

void Person::Logout()
{
	// ??? khong xac dinh hanh dong
}

void Person::Input(SQLHSTMT &hStmt)
{
}

void Person::Output()
{

}