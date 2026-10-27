#include <iostream>
#include <string>
#include "Task.h"
#include "DateUtil.h"
#include "TaskManager.h"

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