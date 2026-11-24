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

	if (text.empty())
	{
		return 0;
	}

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
	std::cout << "==============================" << std::endl;
	std::cout << "  課題・提出期限管理ツール" << std::endl;
	std::cout << "  今日: " << todayString() << std::endl;
	std::cout << "==============================" << std::endl;


	TaskManager manager;	//TaskManagerを1つ作る
	manager.load();
	manager.showWarnings();	//起動したら最初に警告
	std::string input;	//1回分の入力

	//メニューを繰り返す
	while (true)
	{
		std::cout << "\n=== メニュー ===" << std::endl;
		std::cout << "1: 課題を追加" << std::endl;
		std::cout << "2: 一覧を表示" << std::endl;
		std::cout << "3: 課題を完了にする" << std::endl;
		std::cout << "4: 課題を削除する" << std::endl;
		std::cout << "5: 課題を編集する" << std::endl;
		std::cout << "6: 未完了の課題だけ表示" << std::endl;
		std::cout << "0: 終了" << std::endl;
		std::cout << "番号 >";
		std::getline(std::cin, input);

		if (input == "1")
		{
			//課題を1件追加する
			Task task;
			
			//課題名:Enterだけならキャンセル
			std::cout << "課題名(Enterだけでキャンセル) >";
			std::getline(std::cin, task.name);
			if (task.name.empty())
			{
				std::cout << "キャンセルしました。" << std::endl;
				continue;	//メニューに戻る
			}
			
			// ★ 期限:Enterだけならキャンセル、変な値なら聞き直す
			bool cancelled = false;
			//正しい日付になるまで聞き直す
			while (true)
			{
				std::cout << "提出期限(例: 1020 または 20261020、Enterだけでキャンセル) >";

				std::string inputDate;
				std::getline(std::cin, inputDate);

				if (inputDate.empty())
				{
					cancelled = true;
					break;
				}

				task.deadline = normalizeDate(inputDate);	//"2026/10/20"の形に直す

				if (isValidDate(task.deadline))
				{
					break;
				}
				std::cout << "正しい日付を入力してください。(例: 1020 または 20261020)" << std::endl;
			}

			if (cancelled)
			{
				std::cout << "キャンセルしました。" << std::endl;
				continue;//メニューへ
			}
			manager.add(task);
			std::cout << "追加しました。(期限: " << task.deadline << ")" << std::endl;
		}
		else if (input == "2")
		{
			manager.show();	//課題の一覧を表示する
		}
		else if (input == "3")
		{
			manager.show();
			//課題を完了にする
			if (manager.isEmpty())
			{
				std::cout << "課題がありません。" << std::endl;
			}
			else
			{
				manager.show();	//一覧表示
				std::cout << "完了にする番号(Enterだけでキャンセル) >";
				int num = readNumber();

				if (num == 0)
				{
					std::cout << "キャンセルしました。" << std::endl;
				}
				else if (!manager.isValidNumber(num))
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
				manager.show();	//一覧表示
				std::cout << "削除する番号(Enterだけでキャンセル) >";
				int num = readNumber();

				if (num == 0)	
				{
					std::cout << "キャンセルしました。" << std::endl;
				}
				else if (!manager.isValidNumber(num))
				{
					std::cout << "その番号の課題はありません。" << std::endl;
				}
				else
				{
					std::string name = manager.getName(num);	//消す前に名前を取っておく
					manager.remove(num);
					std::cout << name << " を削除しました。" << std::endl;
				}
			}
		}
		else if (input == "5")
		{
			//課題を編集する
			if (manager.isEmpty())
			{
				std::cout << "課題がありません。" << std::endl;
			}
			else
			{
				manager.show();	//一覧表示
				std::cout << "編集する番号(Enterだけでキャンセル) >";
				int num = readNumber();

				if (num == 0)	
				{
					std::cout << "キャンセルしました。" << std::endl;
				}
				else if (!manager.isValidNumber(num))
				{
					std::cout << "その番号の課題はありません。" << std::endl;
				}
				else
				{
					Task task = manager.getTask(num);	//今の課題をもらう

					//課題名:空ならそのまま
					std::cout << "新しい課題名(そのままならEnter) >";
					std::string newName;
					std::getline(std::cin, newName);
					if (!newName.empty())
					{
						task.name = newName;
					}

					//期限:空ならそのまま、入れたら正しくなるまで聞き直す
					while (true)
					{
						std::cout << "新しい提出期限(そのままならEnter) >";
						std::string inputDate;
						std::getline(std::cin, inputDate);

						if (inputDate.empty())
						{
							break;
						}

						std::string newDeadline = normalizeDate(inputDate);
						if (isValidDate(newDeadline))
						{
							task.deadline = newDeadline;
							break;
						}
						std::cout << "正しい日付を入力してください。(例: 1020 または 20261020)" << std::endl;
					}

					manager.update(num, task);	//書き換えて並べ替えて保存
					std::cout << "編集しました。(" << task.name << " / 期限: " << task.deadline << ")" << std::endl;
				}
			}
		}
		else if (input == "6")
		{
			manager.show(true);
		}
		else if (input == "0")
		{
			break;
		}
		else
		{
			std::cout << "0～6の番号を入力してください。" << std::endl;
		}
	}

	return 0;
}