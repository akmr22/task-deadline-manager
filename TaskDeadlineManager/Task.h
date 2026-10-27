#pragma once
#include <string>

//課題1件分のデータをまとめる型
struct Task
{
	std::string name;	//課題名
	std::string deadline;	//提出期限
	bool done = false;	//完了したかどうか(最初は未完了)
};