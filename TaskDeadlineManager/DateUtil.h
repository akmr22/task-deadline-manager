#pragma once
#include <string>

//期限まであと何日かを計算する関数
int daysLeft(const std::string& deadline);

//"2026/10/20" の形で、実在する日付かを調べる関数
bool isValidDate(const std::string& date);

// 入力を "20026/10/20"の形に直す関数。直せなければ""を返す
std::string normalizeDate(const std::string& input);

//今日の日付を "2026/11/24" の形で返す関数
std::string todayString();