#pragma once
#include "Header.h"

class Dish
{
private:
	wstring DishID, DishName, DishType;
public:
	void Input();
	void Output();
	wstring GetDishID();
	wstring GetDishName();
	wstring GetDishType();
};

