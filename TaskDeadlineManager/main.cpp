#include <iostream>
#include <string>
#include <vector>

//課題1件分のデータをまとめる型
struct Task
{
	std::string name;//関数名
	std::string deadline;//提出期限
};



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
			std::cout << "追加しました。" << std::endl;
		}
		else if (input== "2")
		{
			//課題の一覧を表示する
			std::cout << "\n--- 課題一覧 ---" << std::endl;

			if (tasks.empty())
			{
				std::cout << "登録された課題はありません。" << std::endl;
			}

			for (size_t i = 0; i < tasks.size(); i++)
			{
				std::cout << i + 1 << ". " << tasks[i].name
					<< "(期限: " << tasks[i].deadline << ")" << std::endl;
			}
		}
		else if (input =="0")
		{
			break;
		}
		else
		{
			std::cout << "0～2の番号を入力してください。" << std::endl;
		}

	}

	return 0;
}