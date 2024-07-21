#pragma once
#include "Customer.h"
#include "Staff.h"

void Person::Login()
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

}

void Person::Input()
{

}

void Person::Output()
{

}