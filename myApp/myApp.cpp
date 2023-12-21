// App.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <Windows.h>
#include <cctype>
#include <algorithm>
#include <sstream>
using namespace std;

int _stateMenu;

vector <int> makeVectorWord(string s) {
	std::transform(s.begin(), s.end(), s.begin(), tolower);
	vector<int> ans;
	vector <char> alp = { 'а', 'б', 'в', 'г', 'д','е','ё','ж','з','и','й','к','л','м','н','о','п','р','с','т','у','ф','х','ц','ч','ш','щ','ъ','ы','ь','э','ю','я' };//
	for (char sym : alp) {
		ans.push_back(count(s.begin(), s.end(), sym));
	}
	return ans;
}

double moduleVector(const vector <int> a) {
	double sum = 0;
	for (int x : a) {
		sum += pow(x, 2);
	}
	return sqrt(sum);
}

int multiplyVector(const vector <int> a, const vector <int> b) {
	int sum = 0;
	for (int i = 0; i < 33; i++)
	{
		sum += (a[i] * b[i]);
	}
	return sum;
}

string to_string(const double& num) {
	std::ostringstream s;
	s << num;
	return s.str();
}




string deleteSyntax(string word) {
	for (size_t i = 0; i < word.size(); i++)
	{
		if (word[i] == ',' || word[i] == '!' || word[i] == '?' || word[i] == '.' || word[i] == '"' || word[i] == '\'' || word[i] == ':' ||
			word[i] == ';' || word[i] == '(' || word[i] == ')' || word[i] == '\\n' || word[i] == '\\t') {
			word.erase(i, 1);
			i--;
		}
	}

	return word;
}

string correctEnding(int num) {
	string end;
	if (num == 1) {
		end = "слово";
	}
	else if (num == 2 || num == 3 || num == 4) {
		end = "слова";
	}
	else {
		end = "слов";
	}
	return " " + end;
}

void Menu() {
	cout << "Выберите действие:\n"
		<< "(0) Выход из программы\n"
		<< "(1) Чтение данных из файла\n"
		<< "(2) Добавить стоп-слова\n"
		<< "(3) Вывести стоп-слова\n"
		<< "(4) Ввести слово схожие с которым нужно найти\n\n"
		<< "Ваш выбор: ";
	cin >> _stateMenu;
}

string GetFileName() {
	const map<string, string> files{
		{"0", "test"},
		{"1", "prestuplenie-i-nakazanie"},
		{"2", "tomato"}
	};
	string actions;
	cout << "Выберите какой файл открыть:\n";

	for (const auto& obj : files) {
		cout << "(" << obj.first << ") " << obj.second << "\n";
	}
	cout << "Ваш выбор: ";
	cin >> actions;
	//return files[actions];
	for (const auto& obj : files) {
		if (actions == obj.first) return "#data/" + obj.second + ".txt";
	}
	return "#data/test.txt";
}

bool is_stop_word(string& s, vector <string>& stop_words) {
	if (stop_words.size() == 0) {
		return 0;
	}

	for (string word : stop_words)
	{
		if (s == word) return 1;
	}
	return 0;
}

bool DataInitialization(const string& inputPath, const string outputPath = "#data/output.txt") {
	ifstream file(inputPath);
	ofstream out(outputPath);

	if (file && out) {
		return 1;
	}
	else {
		cout << "Файл не открыт\n";
		return 0;
	}
	file.close();
	out.close();
}

void CreateModel(const string& inputPath, map <string, int> & model, map <string, int>& modelLength, long long& counter_symbols, vector <string>& stop_words) {
	ifstream file(inputPath);

	model.clear();
	string psevdo_word, word;
	size_t word_size;
	long long counter_words = 0;

	while (!file.eof()) {
		file >> psevdo_word;
		if (psevdo_word == "-") continue;
		word = deleteSyntax(psevdo_word);

		if (!is_stop_word(word, stop_words)) {
			counter_symbols += psevdo_word.size();
			if (psevdo_word == "-") continue;
			counter_words++;

			word_size = word.size();
			if (word_size <= 5) {
				modelLength["counter_word_length_less_5"] += 1;
			}
			else if (word_size <= 10 && word_size > 5) {
				modelLength["counter_word_length_less_10_more_5"] += 1;
			}
			else if (word_size <= 15 && word_size > 10) {
				modelLength["counter_word_length_less_15_more_10"] += 1;
			}
			else if (word_size <= 20 && word_size > 15) {
				modelLength["counter_word_length_less_20_more_15"] += 1;
			}
			else {
				modelLength["counter_word_length_more_20"] += 1;
			}
			model[word] += 1;

		}
	}
	counter_symbols = counter_symbols + counter_words - 1;
	file.close();
}

void DataWritting(map <string, int>& model, map <string, int>& modelLength, long long &counter_symbols, string s1 = "мама", const string outputPath = "#data/output.txt") {
	setlocale(LC_ALL, "Rus");
	ofstream out(outputPath);

	out << "Количесвто символов в тексте: " << counter_symbols << "\n\n";

	out << "Количесвто слов длины <=5: " << modelLength["counter_word_length_less_5"]
		<< correctEnding(modelLength["counter_word_length_less_5"] % 10) << "\n"
		<< "Количесвто слов длины <=10 и >5: " << modelLength["counter_word_length_less_10_more_5"]
		<< correctEnding(modelLength["counter_word_length_less_10_more_5"] % 10) << "\n"
		<< "Количесвто слов длины <=15 и >10: " << modelLength["counter_word_length_less_15_more_10"]
		<< correctEnding(modelLength["counter_word_length_less_15_more_10"] % 10) << "\n"
		<< "Количесвто слов длины <=20 и >15: " << modelLength["counter_word_length_less_20_more_15"]
		<< correctEnding(modelLength["counter_word_length_less_20_more_15"] % 10) << "\n"
		<< "Количесвто слов длины >20: " << modelLength["counter_word_length_more_20"]
		<< correctEnding(modelLength["counter_word_length_more_20"] % 10) << "\n\n";

	const int frequentWord = 100;
	out << "Слова наиболее часто встречающиеся в тексте(>" << frequentWord << "):\n";

	vector<string> ans;

	for (auto obj : model) {
		/*находим слова наиболее схожие с введеным словом*/
		string s = obj.first;
		transform(s.begin(), s.end(), s.begin(), tolower);
		vector <char> alp = { 'а', 'б', 'в', 'г', 'д','е','ё','ж','з','и','й','к','л','м','н','о','п','р','с','т','у','ф','х','ц','ч','ш','щ','ъ','ы','ь','э','ю','я' };//

		vector <int> a = makeVectorWord(s1);
		vector <int> b;
		for (char sym : alp) {
			b.push_back(count(s.begin(), s.end(), sym));
		}

		double cos_a_b = multiplyVector(a, b) / (moduleVector(a)*moduleVector(b));

		if (cos_a_b > 0.9) {
			ans.push_back(obj.first);
		}

		/**/
		if (obj.second > frequentWord) {
			out << obj.first << " " << obj.second << correctEnding(obj.second % 10) << " ";
			/*Построение векторного представления слова*/
			string s = obj.first;
			transform(s.begin(), s.end(), s.begin(), tolower);
			vector <char> alp = { 'а', 'б', 'в', 'г', 'д','е','ё','ж','з','и','й','к','л','м','н','о','п','р','с','т','у','ф','х','ц','ч','ш','щ','ъ','ы','ь','э','ю','я' };

			out << "[";
			for (char sym : alp) {
				out << count(s.begin(), s.end(), sym) << ", ";
			}
			out << "]\n";
			/**/
		}
	}

	out << "\n\n\nСлова, наиболее схожие со словом " << s1 << ": ";
	for (string s : ans) {
		out << s << " ";
	}
	out << "\n";

	out << "\nУникальные слова в тексте, встречающиеся в тексте 1 раз:\n";
	for (auto obj : model) {
		if (obj.second == 1) {
			out << obj.first << "\n";
		}
	}
	std::cout << "Данные успешно записаны в файл " << outputPath << "\n";
	out.close();
}

void GetStopWords(vector <string> &stop_words) {
	int n;
	string word;
	cout << "Введите количество стоп-слов: ";
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cout << "Введите слово номер " << i + 1 << ": ";
		cin >> word;
		stop_words.push_back(word);
	}
}

void PrintStopWords(vector <string>& stop_words) {
	cout << "Стоп-слова: ";
	if (stop_words.size()) {
		for (size_t i = 0; i < stop_words.size(); i++)
		{
			if (i != stop_words.size() - 1) {
				std::cout << stop_words[i] << ", ";
			}
			else {
				std::cout << stop_words[i] << ".\n";
			}

		}
	}
	else {
		cout << "Стоп-слов нет!\n";
	}
}



int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	cout << 1;
	Menu();

	string fileName;
	string s1;
	map <string, int> modelFrequencyWord;
	map <string, int> modelLengthWord;
	long long counter_symbols = 0;
	vector <string> stopWords;

	while (_stateMenu != 0) {
		switch (_stateMenu)
		{
		case 1:
			system("cls");

			fileName = GetFileName();
			if (DataInitialization(fileName)) {
				CreateModel(fileName, modelFrequencyWord, modelLengthWord, counter_symbols, stopWords);
				DataWritting(modelFrequencyWord, modelLengthWord, counter_symbols, s1);
			}

			system("pause");
			system("cls");
			Menu();
			break;

		case 2:
			system("cls");

			GetStopWords(stopWords);

			system("pause");
			system("cls");
			Menu();
			break;

		case 3:
			system("cls");

			PrintStopWords(stopWords);

			system("pause");
			system("cls");
			Menu();
			break;

		case 4:
			system("cls");

			cout << "Введите слово: ";
			cin >> s1;

			system("pause");
			system("cls");
			Menu();
			break;

		default:
			system("pause");
			system("cls");
			Menu();
			break;

		}
	}
	cout << "Работа завершена.";
	return 0;
}
