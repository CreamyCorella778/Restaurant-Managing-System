#pragma once
#include "Header.h"
#include "Manager.h"
#include "Menu.h"

class SubBrand
{
private:
	wstring BrandName;
	Manager* BrandManager;
	vector<Branch*> BranchList;
	vector<Menu*> MenuList;
};

