#include "DateUtil.h"
#include <ctime>

//期限まであと何日かを計算する関数
int daysLeft(const std::string& deadline)
{
	// "2026/10/20"　を年・月・日に分ける
	int year = std::stoi(deadline.substr(0, 4));
	int month = std::stoi(deadline.substr(5, 2));
	int day = std::stoi(deadline.substr(8, 2));

	// 期限の日付を作る
	std::tm deadlineTm = {};
	deadlineTm.tm_year = year - 1900;
	deadlineTm.tm_mon = month - 1;
	deadlineTm.tm_mday = day;
	std::time_t deadlineTime = std::mktime(&deadlineTm);

	// 今日の日付を作る（時刻は0時0分0秒にそろえる）
	std::time_t now = std::time(nullptr);
	std::tm todayTm = {};
	localtime_s(&todayTm, &now);
	todayTm.tm_hour = 0;
	todayTm.tm_min = 0;
	todayTm.tm_sec = 0;
	std::time_t todayTime = std::mktime(&todayTm);

	// 差を秒で出して、1日の秒数で割る
	double diff = std::difftime(deadlineTime, todayTime);
	return static_cast<int>(diff / (60 * 60 * 24));
}

//"2026/10/20" の形で、実在する日付かを調べる関数
bool isValidDate(const std::string& date)
{
	//長さが10文字で、5文字と8文字目が'/'か
	if (date.size() != 10 || date[4] != '/' || date[7] != '/')
	{
		return false;
	}

	//　'/'以外が全部数字か
	for (size_t i = 0; i < date.size(); i++)
	{
		if (i == 4 || i == 7)
		{
			continue;
		}
		if (date[i] < '0' || date[i]>'9')
		{
			return false;
		}
	}

	int year = std::stoi(date.substr(0, 4));
	int month = std::stoi(date.substr(5, 2));
	int day = std::stoi(date.substr(8, 2));

	//月が1～12か
	if (month < 1 || month>12)
	{
		return false;
	}

	//その月に存在する日か(うるう年も考える)
	int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	if (isLeapYear)
	{
		daysInMonth[1] = 29;
	}

	if (day <1 || day>daysInMonth[month - 1])
	{
		return false;
	}

	return true;
}

std::string normalizeDate(const std::string& input)
{
	// 1. もう "2026/10/20" の形(10文字)なら、そのまま返す
	if (input.size() == 10)
	{
		return input;
	}

	//　'/'以外が全部数字か
	for (size_t i = 0; i < input.size(); i++)
	{
		if (input[i] < '0' || input[i]>'9')
		{
			return "";
		}
	}

	// 3. 今年の年を取る
	std::time_t now = std::time(nullptr);
	std::tm todayTm = {};
	localtime_s(&todayTm, &now);
	int year = todayTm.tm_year + 1900;

	// 4. 4桁なら:今年 + "/" + 前2文字 + "/" + 後ろ2文字
	if (input.size() == 4)
	{
		std::string date = std::to_string(year) + "/" + input.substr(0, 2) + "/" + input.substr(2, 2);

		//　今日より前の日付なら来年にする
		if (date < todayString())
		{
			date = std::to_string(year + 1) + "/" + input.substr(0, 2) + "/" + input.substr(2, 2);
		}
		return date;
	}
	// 5. 8桁なら:前4文字 + "/" + 次の2文字 + "/" + 最後の2文字
	if (input.size() == 8)
	{
		return input.substr(0, 4) + "/" + input.substr(4, 2) + "/" + input.substr(6, 2);
	}
	
	return "";
}

std::string todayString()
{
	std::time_t now = std::time(nullptr);
	std::tm todayTm = {};
	localtime_s(&todayTm, &now);

	int year = todayTm.tm_year + 1900;
	int month = todayTm.tm_mon + 1;
	int day = todayTm.tm_mday;

	//1桁なら前に0を付ける(11/5→11/05)
	std::string monthText = std::to_string(month);
	if (month < 10)
	{
		monthText = "0" + monthText;
	}

	std::string dayText = std::to_string(day);
	if (day < 10)
	{
		dayText = "0" + dayText;
	}

	return std::to_string(year) + "/" + monthText + "/" + dayText;
}