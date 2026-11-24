#include "TaskManager.h"
#include "DateUtil.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <Windows.h>

const WORD COLOR_DEFAULT = 7;
const WORD COLOR_GLAY = 8;
const WORD COLOR_RED = 12;
const WORD COLOR_YELLOW = 14;

static void setColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//並べ替えの比べ方:未完了が先 → 期限が早い順 → 同じ期限なら名前順
static bool compareTasks(const Task& a, const Task& b)
{
	if (a.done != b.done)
	{
		return !a.done;	//aが未完了なら前に
	}
	if (a.deadline != b.deadline)
	{
		return a.deadline < b.deadline;
	}

	return a.name < b.name;
}

void TaskManager::load()
{
	tasks.clear();
	std::ifstream file("tasks.txt");

	if (!file)
	{
		return;
	}

	Task task;
	std::string doneText;

	int skippedCount = 0; //読み込めなかった件数

	// 期限と完了の行が読めなかった(ファイルが途中で終わっている)
	while (std::getline(file, task.name))
	{
		if (!std::getline(file, task.deadline) || !std::getline(file, doneText))
		{
			skippedCount++;
			break;
		}

		// 中身がおかしい課題は飛ばす
		if (task.name.empty() || !isValidDate(task.deadline) || (doneText !="0" && doneText !="1"))
		{
			skippedCount++;
			continue;
		}

		task.done = (doneText == "1");
		tasks.push_back(task);
	}

	// 手で書き換えられて順番が崩れていても大丈夫なように並べ直す
	std::sort(tasks.begin(), tasks.end(), compareTasks);

	//飛ばした課題があれば知らせる
	if (skippedCount > 0)
	{
		std::cout << "保存データに読み込めない課題が " << skippedCount << " 件ありました。(スキップしました)" << std::endl;
	}
}

void TaskManager::save() const
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

void TaskManager::show(bool onlyUndone) const
{
	//未完了と期限切れの件数を数える
	int undoneCount = 0;
	int overdueCount = 0;

	for (size_t i = 0; i < tasks.size(); i++)
	{
		if (!tasks[i].done)
		{
			undoneCount++;

			if (daysLeft(tasks[i].deadline)<0)
			{
				overdueCount++;
			}
		}
	}

	std::cout << "\n--- 課題一覧(未完了 " << undoneCount 
		<< "件 / 期限切れ " << overdueCount << "件) ---" << std::endl;

	if (tasks.empty())
	{
		std::cout << "登録された課題はありません。" << std::endl;
	}

	for (size_t i = 0; i < tasks.size(); i++)
	{
		if (onlyUndone && tasks[i].done) //未完了だけかつ完了済み
		{
			continue;
		}

		WORD color = COLOR_DEFAULT;
		if (tasks[i].done)
		{
			color = COLOR_GLAY;
		}
		else
		{
			int d = daysLeft(tasks[i].deadline);
			if (d < 0)
			{
				color = COLOR_RED;
			}
			else if (d <= 3)
			{
				color = COLOR_YELLOW;
			}
		}
		setColor(color);

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
		setColor(COLOR_DEFAULT);
	}
}

//期限切れ・3日以内の課題を警告する
void TaskManager::showWarnings() const
{
	bool found = false; //警告する課題が一つでもあったか

	for (size_t i = 0; i < tasks.size(); i++)
	{
		//完了している課題は飛ばす
		if (tasks[i].done)
		{
			continue;
		}
		
		int days = daysLeft(tasks[i].deadline);

		//4日以上先なら警告しないので飛ばす
		if (days > 3)
		{
			continue;
		}

		//最初の1件のときだけ見出しを出す
		if (!found)
		{
			std::cout << "\n!!! 注意が必要な課題 !!!" << std::endl;
			found = true;
		}

		if (days < 0)
		{
			setColor(COLOR_RED);
		}
		else
		{
			setColor(COLOR_YELLOW);
		}

		std::cout << "・" << tasks[i].name;

		if (days < 0)
		{
			std::cout << "(" << -days << "日超過)" << std::endl;
		}
		else if (days == 0)
		{
			std::cout << "(今日まで!)" << std::endl;
		}
		else
		{
			std::cout << "(あと" << days << "日)" << std::endl;
		}
		setColor(COLOR_DEFAULT);
	}
}



bool TaskManager::isEmpty() const
{
	return tasks.empty();
}

bool TaskManager::isValidNumber(int num) const
{
	return num >= 1 && num <= static_cast<int>(tasks.size());
}

std::string TaskManager::getName(int num) const
{
	return tasks[num - 1].name;
}

Task TaskManager::getTask(int num) const
{
	return tasks[num - 1];
}


void TaskManager::add(const Task& task)
{
	tasks.push_back(task);
	std::sort(tasks.begin(), tasks.end(), compareTasks);
	save();
}

void TaskManager::update(int num, const Task& task)
{
	tasks[num - 1] = task;
	std::sort(tasks.begin(), tasks.end(), compareTasks);
	save();
}

void TaskManager::toggleDone(int num)
{
	tasks[num - 1].done = !tasks[num-1].done;	//trueならfalse falseならtrueに
	std::sort(tasks.begin(), tasks.end(), compareTasks);
	save();
}

void TaskManager::remove(int num)
{
	tasks.erase(tasks.begin() + (num - 1));
	save();
}
