#include <fstream>                                
#include <iostream>
#include <string>
#include <Windows.h>
#include <string.h>
#include <vector>
#include <locale.h>
#include <iomanip>
#include <ctime>

using namespace std;

//функция вывода файлов в папке с помощью WinApi
void ListFiles(string folderPath, string exten)
{
	string searchPath = folderPath + "\\*." + exten;

	WIN32_FIND_DATAA findData;
	HANDLE findHandle = FindFirstFileA(searchPath.c_str(), &findData);

	if (findHandle != INVALID_HANDLE_VALUE)
	{
		std::vector<string> textFiles;

		do
		{
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				textFiles.push_back(findData.cFileName);
			}

		} while (FindNextFileA(findHandle, &findData));

		FindClose(findHandle);

		cout << "\nСписок файлов в папке " << folderPath << ":\n";

		for (const auto& fileName : textFiles)
		{
			cout << fileName << "\n";
		}
	}
	else
	{
		cout << "\nНе удалось найти файлы в указанной папке.\n";
		exit(0);
	}
}

//функция для автоматического наименования копии
string BACKUP_COPY(string name_)
{
	string name = name_.substr(0, name_.find('.'));
	string exten = name_.substr(name_.find('.'), -1).substr(1, exten.size() - 1);
	return name + "_copy." + exten;
}

void COPY(string name)
{
	cout << "\nХотите создать резервную копию исходного файла? ";
	string ot, ot2, name_copy;
	cin >> ot;
	if (ot == "Yes")
	{
		cout << "Сгенерировать название копии файла? ";
		cin >> ot2;
		if (ot2 == "Yes")
		{
			name_copy = BACKUP_COPY(name);
			cout << "\nСоздана копия " << name_copy << "\n";
		}
		else if (ot2 == "No")
		{
			cout << "Дайте название файлу (не забудьте указать расширение): ";
			cin >> name_copy;
			cout << "\nНазвание копии заданное пользователем: " << name_copy << "\n";
		}
	}

	//копирую файл
	ifstream File(name);
	ofstream copyFile(name_copy);
	if (File.is_open() && copyFile.is_open())
	{
		string line;
		while (getline(File, line))
		{
			copyFile << line << "\n";
		}
	}
	copyFile.close();
	File.close();
}

void LOG_FILE(string name, string zamen, string zamen1, int count, long double elapsed_time)
{
	//печатаю текущую статистику 
	cout << "\nОперация: замена текста \n";
	cout << "Файл: " << name << "\n";
	cout << "Что заменено: " << zamen << "\n";
	cout << "На что заменено: " << zamen1 << "\n";
	cout << "Количество замен: " << count << "\n";
	cout << "Время, потраченное на операцию: " << elapsed_time << " мс" << "\n";

	//обновляю файл логирования и в случае чего вывожу на экран 
	ofstream logfile("log.txt", ios_base::app);
	if (logfile.is_open())
	{
		logfile << "Операция: замена текста \n";
		logfile << "Файл: " << name << "\n";
		logfile << "Что заменено: " << zamen << "\n";
		logfile << "На что заменено: " << zamen1 << "\n";
		logfile << "Количество замен: " << count << "\n";
		logfile << "Время, потраченное на операцию: " << elapsed_time << " мс" << "\n\n\n";
		logfile.close();

		cout << "\nВывести историю всех операций? (Yes, No) ";
		string otv;
		cin >> otv;
		if (otv == "Yes")
		{
			cout << "\n\nПолный файл логирования:\n\n";
			ifstream logFileRead("log.txt");
			if (logFileRead.is_open())
			{
				string logLine;
				while (getline(logFileRead, logLine))
				{
					cout << logLine << endl;
				}
				logFileRead.close();
			}
		}
	}
}

void TXT(string name)
{
	setlocale(0, "");

	string line, name_copy;

	//файл открыла, проверила, напечатала, дала название копии, скопировала содержимое 
	ifstream file(name);
	if (!file.is_open())
	{
		cout << "Файл " << name << " не найден.\n";
		exit(0);
	}
	else
	{
		string otv;
		cout << "\nВывести полный файл на экран (лучше выводить файлы под номером 1)? (Yes, No) ";
		cin >> otv;
		if (otv == "Yes")
		{
			while (getline(file, line))
			{
				cout << line << "\n";
			}
		}
	}
	file.close();

	COPY(name);

	//текст на замену
	string zamen;
	cout << "\nВведите текст, который необходимо заменить: ";
	cin >> zamen;
	string zamen1;
	cout << "Введите текст, на который необходимо заменить: ";
	cin >> zamen1;

	clock_t the_time;                                                                         /////////
	long double elapsed_time;                                                                 /////////
	the_time = clock();                                                                       /////////

	//работа с файлом. Через чат смотрела
	ifstream inFile(name);
	string content((istreambuf_iterator<char>(inFile)), (istreambuf_iterator<char>()));
	inFile.close();

	//добавила счётчик замен, сами замены 
	int count = 0;
	size_t pos = content.find(zamen);
	while (pos != string::npos)
	{
		content.replace(pos, zamen.length(), zamen1);
		pos = content.find(zamen, pos + zamen1.length());
		count++;
	}

	elapsed_time = long double(clock() - the_time) / (CLOCKS_PER_SEC / 1000);                 /////////

	//меняю содержимое и вывожу на экран файл с заменами 
	ofstream outFile(name);
	outFile << content;
	outFile.close();

	string otv;
	cout << "\nВывести полный файл после замены текста на экран? (Yes, No) ";
	cin >> otv;
	if (otv == "Yes")
	{
		cout << "\n\nСодержимое файла, после замены текста: \n\n" << content << "\n";
	}
	
	LOG_FILE(name, zamen, zamen1, count, elapsed_time);
}

void CSV(string name)
{
	setlocale(0, "");

	ifstream file(name);
	string line, name_copy;

	if (!file.is_open())
	{
		cout << "Файл " << name << " не найден.\n";
		exit(0);
	}
	else
	{
		string otv;
		cout << "\nВывести полный файл на экран (лучше выводить файлы под номером 1)? (Yes, No) ";
		cin >> otv;
		if (otv == "Yes")
		{
			string line;
			vector<vector<string>> data;

			while (getline(file, line))
			{
				vector<string> row;
				string cell;
				for (char c : line)
				{
					if (c != ';')
					{
						cell += c;
					}
					else
					{
						row.push_back(cell);
						cell.clear();
					}
				}
				row.push_back(cell);
				data.push_back(row);
			}

			for (const auto& row : data)
			{
				for (const auto& cell : row)
				{
					cout << cell << "\t";
				}
				cout << "\n";
			}
		}
	}
	file.close();

	COPY(name);

	//текст на замену
	string zamen;
	cout << "Введите текст, который необходимо заменить: ";
	cin >> zamen;
	string zamen1;
	cout << "Введите текст, на который необходимо заменить: ";
	cin >> zamen1;

	clock_t the_time;                                                                         /////////
	long double elapsed_time;                                                                 /////////
	the_time = clock();                                                                       /////////

	//работа с файлом????? Через чат смотрела
	ifstream inFile(name);
	string content((istreambuf_iterator<char>(inFile)), (istreambuf_iterator<char>()));
	inFile.close();

	//добавила счётчик замен, сами замены 
	int count = 0;
	size_t pos = content.find(zamen);
	while (pos != string::npos)
	{
		content.replace(pos, zamen.length(), zamen1);
		pos = content.find(zamen, pos + zamen1.length());
		count++;
	}

	elapsed_time = long double(clock() - the_time) / (CLOCKS_PER_SEC/1000);                   /////////

	//меняю содержимое и вывожу на экран файл с заменами 
	ofstream outFile(name);
	outFile << content;
	outFile.close();

	string otv;
	cout << "\nВывести полный файл после замены текста на экран? (Yes, No) ";
	cin >> otv;
	if (otv == "Yes")
	{
		string line;
		vector<vector<string>> data;

		while (getline(file, line))
		{
			vector<string> row;
			string cell;
			for (char c : line)
			{
				if (c != ';')
				{
					cell += c;
				}
				else
				{
					row.push_back(cell);
					cell.clear();
				}
			}
			row.push_back(cell);
			data.push_back(row);
		}

		for (const auto& row : data)
		{
			for (const auto& cell : row)
			{
				cout << cell << "\t";
			}
			cout << "\n";
		}

		string line1;
		vector<vector<string>> data1;

		ifstream outfile(name);
		while (getline(outfile, line1))
		{
			vector<string> row1;
			string cell1;
			for (char c1 : line1)
			{
				if (c1 != ';')
				{
					cell1 += c1;
				}
				else
				{
					row1.push_back(cell1);
					cell1.clear();
				}
			}
			row1.push_back(cell1);
			data1.push_back(row1);
		}
		outfile.close();

		cout << "\n\nСодержимое файла, после замены текста: \n\n";
		for (const auto& row1 : data1)
		{
			for (const auto& cell1 : row1)
			{
				cout << cell1 << "\t";
			}
			cout << "\n";
		}
	}

	LOG_FILE(name, zamen, zamen1, count, elapsed_time);
}

int main()
{
	setlocale(0, "");

	string type;

	cout << "С файлом какого типа вы хотите работать? (txt, csv)\n";
	cin >> type;

	if (type == "txt")
	{
		string folderPath;
		cout << "Введите путь к папке: ";
		cin >> folderPath;

		ListFiles(folderPath, type);

		string name;
		cout << "Введите имя файла, с которым хотите работать: ";
		cin >> name;

		TXT(name);
		cout << "\nПрограмма завершилась.\n";
	}

	else if (type == "csv")
	{
		string folderPath;
		cout << "Введите путь к папке: ";
		cin >> folderPath;

		ListFiles(folderPath, type);

		string name;
		cout << "Введите имя файла, с которым хотите работать: ";
		cin >> name;

		CSV(name);
		cout << "\nПрограмма завершилась.\n";
	}

	else
	{
		cout << "Тип файла указан не верно.\n";
	}
}