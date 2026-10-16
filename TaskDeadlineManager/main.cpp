#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::vector<std::string> tasks;  // 課題名をためておくリスト
    std::string input;               // 1回分の入力

    std::cout << "課題名を入力してください(何も入力せずEnterで終了)" << std::endl;

    // 入力を繰り返す
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input.empty())   // 空ならループを抜ける
        {
            break;
        }

        tasks.push_back(input);  // リストの末尾に追加
    }

    // 一覧を表示
    std::cout << "\n--- 課題一覧 ---" << std::endl;

    if (tasks.empty())
    {
        std::cout << "登録された課題はありません。" << std::endl;
    }

    for (size_t i = 0; i < tasks.size(); i++)
    {
        std::cout << i + 1 << ". " << tasks[i] << std::endl;
    }

    return 0;
}