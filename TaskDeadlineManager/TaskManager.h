#pragma once
#include <string>
#include <vector>
#include "Task.h"

//課題のリストと、それを扱う処理をまとめたクラス
class TaskManager
{
public:
	void load();	//ファイルから読み込む
	void save() const;	//ファイルに保存する
	void show(bool onlyUndone = false) const;	//一覧を表示する
	void showWarnings() const;	// ★ 期限切れ・3日以内の課題を警告する

	bool isEmpty() const; //課題が1件もないか
	bool isValidNumber(int num) const; //その番号の課題があるか
	std::string getName(int num) const; //その番号の課題名を返す
	Task getTask(int num) const; //その番号の課題を丸ごと返す

	void add(const Task& task); //課題を追加して、期限順に並べて保存
	void toggleDone(int num); //完了⇔未完了を切り替えて保存
	void remove(int num);	//課題を削除して保存
	void update(int num, const Task& task); //その番号の課題を書き換えて、並べ替えて保存

private:
	std::vector<Task> tasks;
};