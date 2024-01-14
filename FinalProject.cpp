#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

string CaesarCodingKeyOutput(vector <string> alphabets, string str, short key) {
    // Создаём новый алфавит CaesarAlphabet по заданному ключу
    vector <string> CaesarAlphabets;
    for (int j = 0; j < alphabets.size(); j++) {
        string CaesarAlphabet = "";
        short temp = key;
        if (temp >= alphabets[j].size()) {
            temp = temp % alphabets[j].size();
        }
        for (int i = temp; i < alphabets[j].size(); i++) {
            CaesarAlphabet += alphabets[j][i];
        }
        for (int i = 0; i < temp; i++) {
            CaesarAlphabet += alphabets[j][i];
        }
        CaesarAlphabets.push_back(CaesarAlphabet);
    }
    // Дешифруем строку методом Цезаря

    // А = -64
    // а = -1
    // я = -32
    // Я = -33
    string CaesarStr = "";
    string probel = " ";
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == probel[0]) {
            CaesarStr += " ";
        }
        else if (str[i] >= 65 and str[i] <= 90) {
            CaesarStr += CaesarAlphabets[2][alphabets[2].find(str[i])];
        }
        else if ((str[i] >= 33 and str[i] <= 47) or (str[i] >= 58 and str[i] <= 64) or (str[i] >= 91 and str[i] <= 96) or (str[i] >= 123 and str[i] <= 126)) {
            CaesarStr += CaesarAlphabets[5][alphabets[5].find(str[i])];
        }
        else if (str[i] >= 48 and str[i] <= 57) {
            CaesarStr += CaesarAlphabets[4][alphabets[4].find(str[i])];
        }
        else if (str[i] >= 97 and str[i] <= 122) {
            CaesarStr += CaesarAlphabets[3][alphabets[3].find(str[i])];
        }
        else if (str[i] >= -64 and str[i] <= -33) {
            CaesarStr += CaesarAlphabets[0][alphabets[0].find(str[i])];
        }
        else if (str[i] >= -32 and str[i] <= -1) {
            CaesarStr += CaesarAlphabets[1][alphabets[1].find(str[i])];
        }
    }
    // Шифруем строку методом Цезаря
    return CaesarStr;
    
}

string CaesarDecodingKeyOutput(vector <string> alphabets, string CaesarStr, short key) {
    // Создаём новый алфавит CaesarAlphabet по заданному ключу
    vector <string> CaesarAlphabets;
    for (int j = 0; j < alphabets.size(); j++) {
        string CaesarAlphabet = "";
        short temp = key;
        if (temp >= alphabets[j].size()) {
            temp = temp % alphabets[j].size();
        }
        for (int i = temp; i < alphabets[j].size(); i++) {
            CaesarAlphabet += alphabets[j][i];
        }
        for (int i = 0; i < temp; i++) {
            CaesarAlphabet += alphabets[j][i];
        }
        CaesarAlphabets.push_back(CaesarAlphabet);
    }
    // Дешифруем строку методом Цезаря

    // А = -64
    // а = -1
    // я = -32
    // Я = -33
    string str = "";
    string probel = " ";
    for (int i = 0; i < CaesarStr.size(); i++) {
        if (CaesarStr[i] == probel[0]) {
            str += " ";
        }
        else if (CaesarStr[i] >= 65 and CaesarStr[i] <= 90) {
            str += alphabets[2][CaesarAlphabets[2].find(CaesarStr[i])];
        }
        else if ((CaesarStr[i] >= 33 and CaesarStr[i] <= 47) or (CaesarStr[i] >= 58 and CaesarStr[i] <= 64) or (CaesarStr[i] >= 91 and CaesarStr[i] <= 96) or (CaesarStr[i] >= 123 and CaesarStr[i] <= 126)) {
            str += alphabets[5][CaesarAlphabets[5].find(CaesarStr[i])];
        }
        else if (CaesarStr[i] >= 48 and CaesarStr[i] <= 57) {
            str += alphabets[4][CaesarAlphabets[4].find(CaesarStr[i])];
        }
        else if (CaesarStr[i] >= 97 and CaesarStr[i] <= 122) {
            str += alphabets[3][CaesarAlphabets[3].find(CaesarStr[i])];
        }
        else if (CaesarStr[i] >= -64 and CaesarStr[i] <= -33) {
            str += alphabets[0][CaesarAlphabets[0].find(CaesarStr[i])];
        }
        else if (CaesarStr[i] >= -32 and CaesarStr[i] <= -1) {
            str += alphabets[1][CaesarAlphabets[1].find(CaesarStr[i])];
        }

    }


    return str;
}


int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "rus");


    // Алфавит

    

    string alphabetUppercaseRu = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    string alphabetLowercaseRu = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    string alphabetUppercaseEn = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string alphabetLowercaseEn = "abcdefghijklmnopqrstuvwxyz";
    string alphbetNumbers = "1234567890";
    string alphabetDigits = ".,;:'|/><[]{}-_+=*&^%$#№\"@!`~\\";
    vector <string> alphabets = { alphabetUppercaseRu, alphabetLowercaseRu, alphabetUppercaseEn, alphabetLowercaseEn, alphbetNumbers, alphabetDigits };


    // Программа ------------------------------------------------------------------------

    
    short var;
    cout << "(1) Шифрование \n" <<
        "(2) Дешифрование \n" << 
        "Ваш выбор: ";
    cin >> var;

    // Проверяю на правильность ввода
    if (var != 1 and var != 2) {
        cout << "Такого варианта не существует!" << endl;
        string path = "log.txt";
        ofstream fout;

        fout.open(path, ofstream::app);
        time_t seconds = time(NULL);
        string result = "Пользователь ввёл несуществующий вариант(1) " + (string)asctime(localtime(&seconds));
        fout << result;
        fout.close();
        return 0;
    }

    // Обработка ключа


    short var2;
    short key;
    cout << endl << "(1) Случайный ключ \n" <<
        "(2) Ввести ключ \n" <<
        "Ваш выбор: ";
    cin >> var2;
    // проверяю на правильность ввода
    if (var2 != 1 and var2 != 2) {
        cout << "Такого варианта не существует!" << endl;
        string path = "log.txt";
        ofstream fout;
        fout.open(path, ofstream::app);
        time_t seconds = time(NULL);
        string result = "Пользователь ввёл несуществующий вариант(2) " + (string)asctime(localtime(&seconds));
        fout << result;
        fout.close();
        return 0;
    }

    if (var2 == 1) {
        key = rand();
        cout << endl << "Ваш ключ: " << key << endl;
    }
    else {
        cout << endl << "Введите ключ: ";
        cin >> key;
        
    }


    // Открываем файл, шифруем или дешифруем каждую строку методом Цезаря


    cout << endl << "Результат: " << endl;


    string str;
    string path = "Caesar.txt";
    ifstream fin;
    fin.open(path);
    if (!fin.is_open()) {
        cout << "Ошибка открытия файла!" << endl;
        return 0;
    }
    else {
        while (!fin.eof()) {
            str = "";
            getline(fin, str);
            if (var == 1) {
                //CaesarCodingKeyOutput(alphabets, str, key);
                cout << str << " | to | " << CaesarCodingKeyOutput(alphabets, str, key) << endl;
                cout << "Проверка: " << CaesarCodingKeyOutput(alphabets, str, key) << " | to | " <<CaesarDecodingKeyOutput(alphabets, CaesarCodingKeyOutput(alphabets, str, key), key) << endl;
                // Логируе
                string path = "log.txt";
                ofstream fout;

                fout.open(path, ofstream::app);
                time_t seconds = time(NULL);
                string result = "'" + str + "'" + " Code to " + "'" + CaesarCodingKeyOutput(alphabets, str, key) + "'" + " at " + (string)asctime(localtime(&seconds));
                fout << "Ключ: ";
                fout << key;
                fout << result;
                fout.close();
            }
            else if (var == 2) {
                cout << str << " | to | " << CaesarDecodingKeyOutput(alphabets, str, key) << endl;
                cout << "Проверка: " << CaesarDecodingKeyOutput(alphabets, str, key) << " | to | " << CaesarCodingKeyOutput(alphabets, CaesarDecodingKeyOutput(alphabets, str, key), key) << endl;
                // Логируем
                string path = "log.txt";
                ofstream fout;

                fout.open(path, ofstream::app);
                time_t seconds = time(NULL);
                string result = "'" + str + "'" + " Decode to " + "'" + CaesarDecodingKeyOutput(alphabets, str, key) + "'" + " at " + (string)asctime(localtime(&seconds));
                fout << "Ключ: ";
                fout << key;
                fout << result;
                fout.close();
                
              
            }
        }
    }
    fin.close();
    
    

    //  ---------------------------------------------------------------------------------
    return 0;
}
