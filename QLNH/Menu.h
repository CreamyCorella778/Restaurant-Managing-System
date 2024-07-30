#pragma once
#include "Header.h"
#include "Dish.h"

class Menu
{
private:
	wstring MenuID;
	int MenuPrice;
	vector<Dish*> DishList;
public:
	void Input();
	void Output();
	bool IsCompleteMenu();
	wstring GetMenuID();
	int GetMenuPrice();
	vector<Dish*> GetDishList();
};

