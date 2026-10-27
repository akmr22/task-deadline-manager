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
