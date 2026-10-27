#pragma once
#include <string>

//期限まであと何日かを計算する関数
int daysLeft(const std::string& deadline);

//"2026/10/20" の形で、実在する日付かを調べる関数
bool isValidDate(const std::string& date);