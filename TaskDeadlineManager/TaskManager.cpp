#include "TaskManager.h"
#include "DateUtil.h"
#include <iostream>
#include <fstream>
#include <algorithm>

//期限が早いほうを前にするための比べ方(このファイルの中だけで使う)
static bool compareDeadline(const Task& a, const Task& b)
{
	return a.deadline < b.deadline;
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

	while (std::getline(file, task.name))
	{
		std::getline(file, task.deadline);
		std::getline(file, doneText);
		task.done = (doneText == "1");

		tasks.push_back(task);
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

void TaskManager::show() const
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
	std::sort(tasks.begin(), tasks.end(), compareDeadline);
	save();
}

void TaskManager::update(int num, const Task& task)
{
	tasks[num - 1] = task;
	std::sort(tasks.begin(), tasks.end(), compareDeadline);
	save();
}

void TaskManager::complete(int num)
{
	tasks[num - 1].done = true;
	save();
}

void TaskManager::remove(int num)
{
	tasks.erase(tasks.begin() + (num - 1));
	save();
}
