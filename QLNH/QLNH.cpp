// QLNH.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Header.h"

int main()
{
    // Person* ph...
    // Input AccType from outside
    // ph = ph->createAccByType
    // ph->Login
    // if can't login => return, else get phid
    // identify acc type (vip or standard, normal or manager) by query (select acctype from kh/nv where kh.id = phid)
    // delete ph, new acc
    // ph -> input
    // print menu for functions to use
    // ...
}

wstring randomForIDs()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // Định nghĩa khoảng giá trị từ 0 đến 9999
    std::uniform_int_distribution<> dis(0, 9999);

    // Tạo số ngẫu nhiên
    int randomNumber = dis(gen);

    std::wostringstream woss;
    woss << std::setw(4) << std::setfill(L'0') << randomNumber;
    std::wstring randomWString = woss.str();
    return randomWString;
}

wstring getTodayDate()
{
    time_t t = time(0);
    tm* t_ = localtime(&t);
    wstring res = to_wstring(t_->tm_year + 1900) + L"-" + to_wstring(t_->tm_mon + 1) + L"-" + to_wstring(t_->tm_mday) + L" "\
    + to_wstring(t_->tm_hour) + L":" + to_wstring(t_->tm_min) + L":" + to_wstring(t_->tm_sec);
    return res;
}