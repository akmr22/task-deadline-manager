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
	void show() const;	//一覧を表示する

	bool isEmpty() const; //課題が1件もないか
	bool isValidNumber(int num) const; //その番号の課題があるか
	std::string getName(int num) const; //その番号の課題名を返す

	void add(const Task& task); //課題を追加して、期限順に並べて保存
	void complete(int num); //課題を完了にして保存
	void remove(int num);	//課題を削除して保存

private:
	std::vector<Task> tasks;
};