#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <fstream>

//課題1件分のデータをまとめる型
struct Task
{
	std::string name;//課題名
	std::string deadline;//提出期限
	bool done = false; //完了したかどうか(最初は未完了)
};

//期限が早いほうを前にするための比べ方
bool compareDeadline(const Task& a, const Task& b)
{
	return a.deadline < b.deadline;
}

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

//数字を1つ読み込む関数　数字でなければ-1を返す
int readNumber()
{
	std::string text;
	std::getline(std::cin, text);

	try
	{
		size_t pos = 0;
		int number = std::stoi(text, &pos);

		if (pos != text.size()) // "3a"のように後ろに余計な文字がある
		{
			return -1;
		}
		return number;
	}
	catch (...)	//数字に変換できなかった
	{
		return -1;
	}
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
		if (date[i]<'0'||date[i]>'9')
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
	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
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

class TaskManager
{
public:

	//ファイルから読み込む
	void load()
	{
		tasks.clear();
		std::ifstream file("tasks.txt");

		if (!file)
		{
			return;
		}

		Task task;
		std::string doneText;

		while (std::getline(file,task.name))
		{
			std::getline(file, task.deadline);
			std::getline(file, doneText);
			task.done = (doneText == "1");

			tasks.push_back(task);
		}

	}


	//ファイルに保存する
	void save() const
	{
		std::ofstream file("tasks.txt");
		
		if (!file)
		{
			std::cout << "保存に失敗しました。" << std::endl;
			return;
		}

		for (size_t i = 0; i < tasks.size(); i++)
		{
			file << tasks[i].name << "\n";
			file << tasks[i].deadline << "\n";
			file << tasks[i].done << "\n";
		}
	}
	
	//一覧を表示する
	void show() const
	{
		std::cout << "\n--- 課題一覧 ---" << std::endl;

		if (tasks.empty())
		{
			std::cout << "登録された課題はありません。" << std::endl;
		}

		for (size_t i = 0; i < tasks.size(); i++)
		{
			std::cout << i + 1 << ". ";

			if (tasks[i].done)
			{
				std::cout << "[済] ";
			}
			else
			{
				std::cout << "[未] ";
			}

			std::cout << tasks[i].name
				<< "(期限: " << tasks[i].deadline;

			// 完了していない課題の残り日数を出す
			if (!tasks[i].done)
			{
				int days = daysLeft(tasks[i].deadline);

				if (days > 0)
				{
					std::cout << " / あと" << days << "日";
				}
				else if (days == 0)
				{
					std::cout << " / 今日まで!";
				}
				else
				{
					std::cout << " / " << -days << "日超過";
				}
			}

			std::cout << ")" << std::endl;
		}
	}

	//課題が1件もないか
	bool isEmpty() const
	{
		return tasks.empty();
	}

	//その番号の課題があるか(番号は1から)
	bool isValidNumber(int num) const
	{
		return num >= 1 && num <= static_cast<int>(tasks.size());
	}

	//その番号の課題名を返す
	std::string getName(int num) const
	{
		return tasks[num - 1].name;
	}

	//課題を追加して、期限順に並べて保存
	void add(const Task& task)
	{
		tasks.push_back(task);
		std::sort(tasks.begin(), tasks.end(), compareDeadline);
		save();
	}

	//課題を完了にして保存
	void complete(int num)
	{
		tasks[num - 1].done = true;
		save();
	}

	//課題を削除して保存
	void remove(int num)
	{
		tasks.erase(tasks.begin() + (num - 1));
		save();
	}

	private:
		std::vector<Task> tasks;
};



int main()
{

	TaskManager manager;	//TaskManagerを１つ作る
	manager.load();
	std::string input;	//1回分の入力

	//メニューを繰り返す
	while (true)
	{
		std::cout << "\n=== メニュー ===" << std::endl;
		std::cout << "1: 課題を追加" << std::endl;
		std::cout << "2: 一覧を表示" << std::endl;
		std::cout << "3: 課題を完了にする" << std::endl;
		std::cout << "4: 課題を削除する" << std::endl;
		std::cout << "0: 終了" << std::endl;
		std::cout << "番号 >";
		std::getline(std::cin, input);

		if (input == "1")
		{
			//課題を1件追加する
			Task task;
			while (true)
			{
				std::cout << "課題名 >";
				std::getline(std::cin, task.name);
				if (!task.name.empty())
				{
					break;
				}
				std::cout << "課題名を入力してください。" << std::endl;
			}

			while (true)
			{
				std::cout << "提出期限(例: 2026/10/20) >";
				std::getline(std::cin, task.deadline);
				
				if (isValidDate(task.deadline))
				{
					break;
				}
				std::cout << "2026/10/20 の形で、正しい日付を入力してください。" << std::endl;
			}
	
			manager.add(task);	//追加・並べ替え・保存はTaskManagerに任せる
			std::cout << "追加しました。" << std::endl;
		}
		else if (input== "2")
		{
			manager.show(); //課題の一覧を表示する
		}
		else if (input == "3")
		{
			//課題を完了にする
			if (manager.isEmpty())
			{
				std::cout << "課題がありません。" << std::endl;
			}
			else
			{
				manager.show(); //一覧表示
				std::cout << "完了にする番号 >";
				int num = readNumber();

				if (!manager.isValidNumber(num))
				{
					std::cout << "その番号の課題はありません。" << std::endl;
				}
				else
				{
					manager.complete(num);
					std::cout << manager.getName(num) << " を完了にしました。" << std::endl;
				}
			}

		}
		else if (input == "4")
		{
			//課題を削除する
			if (manager.isEmpty())
			{
				std::cout << "課題がありません。" << std::endl;
			}
			else
			{
				manager.show(); //一覧表示
				std::cout << "削除する番号 >";
				int num = readNumber();

				if (!manager.isValidNumber(num))
				{
					std::cout << "その番号の課題はありません。" << std::endl;
				}
				else
				{
					std::string name = manager.getName(num); //消す前に名前を取っておく
					manager.remove(num);
					std::cout << name << " を削除しました。" << std::endl;
				}
			}
		}
		else if (input =="0")
		{
			break;
		}
		else
		{
			std::cout << "0～4の番号を入力してください。" << std::endl;
		}

	}

	return 0;
}