#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

//課題1件分のデータをまとめる型
struct Task
{
	std::string name;//関数名
	std::string deadline;//提出期限
	bool done = false; //完了したかどうか(最初は未完了)
};

bool compareDeadline(const Task& a, const Task& b)
{
	return a.deadline < b.deadline;
}

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

//課題の一覧を表示する関数
void showTasks(const std::vector<Task>& tasks)
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
			<< "(期限: " << tasks[i].deadline
			<< " / あと" << "日)" << std::endl;

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

int main()
{

	std::vector<Task> tasks;	//課題名をためておくリスト
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

			std::cout << "課題名 >";
			std::getline(std::cin, task.name);

			std::cout << "提出期限(例: 2026/10/20) >";
			std::getline(std::cin, task.deadline);

			tasks.push_back(task);
			std::sort(tasks.begin(), tasks.end(), compareDeadline); //期限順に並べ替え
			std::cout << "追加しました。" << std::endl;
		}
		else if (input== "2")
		{
			showTasks(tasks); //課題の一覧を表示する
		}
		else if (input == "3")
		{
			//課題を完了にする
			if (tasks.empty())
			{
				std::cout << "課題がありません。" << std::endl;
			}
			else
			{
				showTasks(tasks); //一覧表示
				std::cout << "完了にする番号 >";
				std::getline(std::cin, input);
				int num = std::stoi(input); //文字を数字に変換

				if (num <1 || num > static_cast<int>(tasks.size()))
				{
					std::cout << "その番号の課題はありません。" << std::endl;
				}
				else
				{
					tasks[num - 1].done = true;
					std::cout << tasks[num - 1].name << " を完了にしました。" << std::endl;
				}
			}

		}
		else if (input == "4")
		{
			//課題を削除する
			if (tasks.empty())
			{
				std::cout << "課題がありません。" << std::endl;
			}
			else
			{
				showTasks(tasks); //一覧表示
				std::cout << "削除する番号 >";
				std::getline(std::cin, input);
				int num = std::stoi(input);

				if (num < 1 || num > static_cast<int>(tasks.size()))
				{
					std::cout << "その番号の課題はありません。" << std::endl;
				}
				else
				{
					std::string name = tasks[num - 1].name; //消す前に名前を取っておく
					tasks.erase(tasks.begin() + (num - 1));
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