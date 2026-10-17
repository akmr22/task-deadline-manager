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

	std::cout << "課題名を入力してください(何も入力せずEnterで終了)" << std::endl;
	
	while (true)
	{
		std::cout << "課題名 >";
		std::getline(std::cin, input);

		if (input.empty())	//空ならループを抜ける
		{
			break;
		}

		Task task;
		task.name = input;
		//期限を入力してtaskに入れる
		std::cout << "提出期限(例: 2026/10/20) >";
		std::getline(std::cin, task.deadline);

		tasks.push_back(task);
	}

	std::cout << "\n--- 課題一覧 ---" << std::endl;

	if (tasks.empty())
	{
		std::cout << "登録された課題はありません。" << std::endl;
	}

	for (size_t i = 0; i < tasks.size(); i++)
	{
		std::cout << i + 1 << ". " << tasks[i].name
			<< "(期限: "<<tasks[i].deadline << ")" << std::endl;
	}


	return 0;
}