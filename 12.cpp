#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_set>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cctype> 

using namespace std;

string removePunctuation(const string& word) {
    string result;
    for (char c : word) {
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
            if (!ispunct(c) && c != '\'' && c != '«' && c != '»' && c != '…' && c != '„' && c != '“' && c != '-' && c != '–' && c != ',' && c != '!' && c != '?' && c != '.' && c != '(' && c != ')' && c != '.')
            {
                result += tolower(c);
            }
        } else {
            result += tolower(c);
        }
    }
    return result;
}

void removeStopWords(vector<string>& words, const unordered_set<string>& stopWords) {
    vector<string> result;
    for (const string& word : words) {
        string cleanWord = removePunctuation(word);
        if (stopWords.find(cleanWord) == stopWords.end()) {
            result.push_back(word);
        }
    }
    words = result;
}

void addStopWords(unordered_set<string>& stopWords) {
    cout << "Введите стоп-слова для добавления (вводите по одному, для завершения введите пустую строку):\n";
    string stopWord;
    while (true) {
        getline(cin, stopWord);
        if (stopWord.empty()) {
            break;
        }
        stopWords.insert(stopWord);
    }
}

void removeStopWords(unordered_set<string>& stopWords) {
    cout << "Введите стоп-слова для удаления (вводите по одному, для завершения введите пустую строку):\n";
    string stopWord;
    while (true) {
        getline(cin, stopWord);
        if (stopWord.empty()) {
            break;
        }
        stopWords.erase(stopWord);
    }
}

void writeResultsToFile(const string& fileName, int wordCount, int uniqueWords, int sentenceCount,
    const vector<pair<string, int>>& mostFrequentWords, const vector<int>& lengthCounts, const unordered_set<string>& uniqueWordSet) {
    ofstream outputFile(fileName);

    if (!outputFile.is_open()) {
        cerr << "Ошибка открытия файла для записи результатов.\n";
        return;
    }

    outputFile << "Статистика по количеству слов:\n";
    outputFile << "Общее количество слов: " << wordCount << endl;
    outputFile << "Количество уникальных слов: " << uniqueWords << endl;
    outputFile << "Количество предложений: " << sentenceCount << endl;

    outputFile << "\nСтатистика по длине слов:\n";
    outputFile << "Количество слов длинною <5: " << lengthCounts[0] << endl;
    outputFile << "Количество слов длинною 5-10: " << lengthCounts[1] << endl;
    outputFile << "Количество слов длинною 10-15: " << lengthCounts[2] << endl;
    outputFile << "Количество слов длинною 15-20: " << lengthCounts[3] << endl;
    outputFile << "Количество слов длинною >=20: " << lengthCounts[4] << endl;

    outputFile << "\nНаиболее часто встречающиеся слова:\n";
    int wordsToWrite = min(static_cast<int>(mostFrequentWords.size()), 30);
    for (int i = 0; i < wordsToWrite; i++) {
        if (mostFrequentWords[i].first != "—") {
            outputFile << mostFrequentWords[i].first << ": " << mostFrequentWords[i].second << " раз\n";
        }
    }

    outputFile << "\nВсе уникальные слова:\n";
    for (const auto& word : uniqueWordSet) {
        outputFile << word << endl;
    }

    outputFile.close();
}


int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    unordered_set<string> stopWords = { "и", "в", "на", "не", "с", "по", "за", "к", "из", "о", "а", "я" };

    cout << "Выберите действие:\n";
    cout << "1. Оставить без изменений\n";
    cout << "2. Удалить стоп-слова\n";
    cout << "3. Добавить стоп-слова\n";

    int choice;
    cin >> choice;
    cin.ignore();

    switch (choice) {
    case 1:
        break;
    case 2:
        removeStopWords(stopWords);
        break;
    case 3:
        addStopWords(stopWords);
        break;
    default:
        cerr << "Неверный выбор. Программа будет завершена.\n";
        return 1;
    }

    ifstream inputFile("prestuplenie_i_nakazanie.txt");
    if (!inputFile.is_open()) {
        cerr << "Ошибка открытия файла с текстом. Проверьте, что файл существует и доступен.\n";
        return 1;
    }

    vector<string> words;
    string word;
    while (inputFile >> word) {
        words.push_back(word);
    }
    inputFile.close();

    if (words.empty()) {
        cerr << "Ошибка: текстовый файл пуст.\n";
        return 1;
    }

    removeStopWords(words, stopWords);

    map<string, int> wordFrequency;
    vector<pair<int, string>> sortedWords;

    unordered_set<string> uniqueWordSet;

    for (const string& w : words) {
        string cleanedWord = removePunctuation(w);
        wordFrequency[cleanedWord]++;
        uniqueWordSet.insert(cleanedWord);
    }

    for (const auto& pair : wordFrequency) {
        sortedWords.push_back({ pair.second, pair.first });
    }

    sort(sortedWords.begin(), sortedWords.end(), greater<pair<int, string>>());

    int wordCount = words.size();

    vector<pair<string, int>> mostFrequentWords;
    mostFrequentWords.reserve(min(10, static_cast<int>(sortedWords.size())));

    for (const auto& pair : sortedWords) {
        mostFrequentWords.push_back({ pair.second, pair.first });
    }

    cout << "Статистика по количеству слов:\n";
    cout << "Общее количество слов: " << wordCount << endl;

    int uniqueWords = uniqueWordSet.size();
    cout << "Количество уникальных слов: " << uniqueWords << endl;

    int sentenceCount = 0;
    for (const auto& w : words) {
        for (char c : w) {
            if (c == '.' || c == '!' || c == '?') {
                sentenceCount++;
                break;
            }
        }
    }
    cout << "Количество предложений: " << sentenceCount << endl;

    cout << "Уникальные слова:\n";
    int count = 0;
    for (const auto& word : uniqueWordSet) {
        if (count < 10) {
            cout << word << endl;
        }
        count++;
    }
    cout << "Наиболее часто встречающиеся слова:\n";
    for (int i = 0; i < 10 && i < mostFrequentWords.size(); i++) {
        if (mostFrequentWords[i].first != "—") {
            cout << mostFrequentWords[i].first << ": " << mostFrequentWords[i].second << " раз\n";
        }
    }


    cout << "Статистика по длине слов:\n";
    vector<int> lengthCounts(20);
    for (const auto& pair : sortedWords) {
        int length = pair.second.length();
        if (length < 5) {
            lengthCounts[0]++;
        }
        else if (length < 10) {
            lengthCounts[1]++;
        }
        else if (length < 15) {
            lengthCounts[2]++;
        }
        else if (length < 20) {
            lengthCounts[3]++;
        }
        else {
            lengthCounts[4]++;
        }
    }

    cout << "Количество слов длинною <5: " << lengthCounts[0] << endl;
    cout << "Количество слов длинною 5-10: " << lengthCounts[1] << endl;
    cout << "Количество слов длинною 10-15: " << lengthCounts[2] << endl;
    cout << "Количество слов длинною 15-20: " << lengthCounts[3] << endl;
    cout << "Количество слов длинною >=20: " << lengthCounts[4] << endl;

    string outputFileName = "output_results.txt";
    writeResultsToFile(outputFileName, wordCount, uniqueWords, sentenceCount, mostFrequentWords, lengthCounts, uniqueWordSet);

    cout << "Результаты сохранены в файл: " << outputFileName << endl;

    system("pause");
    return 0;
}
