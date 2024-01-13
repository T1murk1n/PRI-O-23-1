
// last_lab_c++.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <Windows.h>
#define time_for_test 5
using namespace std;
// показывать сколько времени осталась до конца, осведомить пользователя о том, что в этом вопросе 1 или сколько - то правильных ответов
string getAnswer(string& answer);

void ReaderAndWriter(ifstream& questions, ofstream& answers, string answer, string line);

string to_sort(string& line);

void checkAnswers(ifstream& user, ifstream& ans, int& result, int& question);

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	string choose = "", line;
	setlocale(LC_ALL, "Russian");
	cout << "Выберете уровень сложности: low, medium, hard" << endl;
	cin >> choose;
	vector <string> any_choose{ "low", "medium", "hard" };
	bool flag_correct_input = false;
	for (auto now : any_choose) {
		if (choose == now) {
			flag_correct_input = true;
			break;
		}
	}
	bool flag_hard = false;
	int start_time = 0;
	if (flag_correct_input == true) {
		switch (choose[0])
		{
		case 'l': {cout << "Выберете один правильный ответ" << endl; break; }
		case 'm': {cout << "Выберете от одного до трёх правильных ответов" << endl; break; }		
		default:
			cout << "Все вопросы в этом тесте открытые, вам просто следует написать то, о чём вас просят " << endl;
			flag_hard = true;
			break;
		}
		choose = choose.append(".").append("txt");
	}
	else {
		cout << "Неверно выбран уровень сложности " << endl;
		return -1;
	}
	ifstream in(choose); // окрываем файл для чтения
	ofstream file_out("user_answer.txt");
	string answer;
	
	start_time = clock();
	if (in.is_open() && file_out.is_open()) {
		ReaderAndWriter(in, file_out, answer, line);
	}
	start_time = clock() - start_time;


	choose = choose.substr(0, choose.size() - 4) + "_answers.txt";
	//int cut = (choose == "low_answers.txt" ? 2 : 10);
	//cout << choose << endl;
	file_out.close();
	in.close();
	ifstream user_answers("user_answer.txt");
	ifstream answers(choose);

	int result = 0;
	int question = 0;
	if (user_answers.is_open() && answers.is_open()) {
		checkAnswers(user_answers, answers, result, question);
	}

	if (flag_hard == false) {
		cout << "Ваш результат " << result << "/" << question << " За " << (float)start_time / CLOCKS_PER_SEC << " секунд" << endl;
	}
	else {
		cout << "Задания с открытыми вопросами проверяются человеком " << endl;
		cout << "Вы ответили на вопросы за " << (float)start_time / CLOCKS_PER_SEC << " секунд" << endl;
	}
	user_answers.close();
	answers.close();
	system("pause");
	return 0;

}
void ReaderAndWriter(ifstream& questions, ofstream& answers, string answer, string line) {
	if (questions.is_open())
	{
		cout << "При ответе на вопрос, сначали введите ответ, а через пробел от ответа end, как окончание ответа " << endl;
		int all_time = 0;
		int count_sentence = 0;
		while (getline(questions, line)) {
			if (((float)all_time / CLOCKS_PER_SEC / 60) > time_for_test) {
				cout << "Время вышло: " << endl;
				return;
			}
			if (line.size() != 0) {
				cout << line << endl;
			}
			count_sentence++;
			if (count_sentence % 6 == 0) {
				cout << "Вам осталось " << (time_for_test * 60) - ((float)all_time / CLOCKS_PER_SEC) << " cекунд" << endl;
				cout << "Введите ответ: " << endl;
				int finish = clock();
				answers << getAnswer(answer) << endl;
				finish = clock() - finish;
				all_time += finish;
				answer = "";
				finish = 0;
			}

		}
	}
}
string getAnswer(string& answer) {
	string temp = "start";
	while (temp != "end") {
		cin >> temp;
		answer += temp;
		answer += ' ';
	}
	answer = answer.substr(0, answer.size() - temp.size() - 2);
	return answer;
}
string to_sort(string& line) {
	vector <char> temp;
	for (char symbol : line) {
		temp.push_back(symbol);
	}
	sort(temp.begin(), temp.end());
	line = "";
	for (auto now : temp) {
		line += now;
	}
	return line;
}
void checkAnswers(ifstream& user, ifstream& ans, int& result, int& question) {
	string line, answer;
	while (getline(user, line) && getline(ans, answer)) {
		line = to_sort(line);
		if (line == answer) {
			result++;
		}
		question++;
	}
}