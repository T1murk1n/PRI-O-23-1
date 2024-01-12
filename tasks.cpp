//#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <ctime>
#include <map>
using namespace std;

// Класс группы задач
class Group
{
public:
    string name; //Название группы
    class Task
    {
    public:
        string priority;
        string task;
        bool status;
        string date;
        bool overdue;

        Task(string task = "", string priority = "", bool status = false, string date = "") {
            this->task = task;
            this->priority = priority;
            this->status = status;
            this->date = date;
            if (status) {
                this->overdue = false;
            }
            else {
                this->overdue = isOverdue(date);
            }
        }
        void newStatus(bool status = false) {
            this->status = status;
            if (status) {
                this->overdue = false;
            }
            else {
                this->overdue = isOverdue(this->date);
            }
        }
        void newDate(string date = "") {
            this->date = date;
            if (this->status) {
                this->overdue = false;
            }
            else {
                this->overdue = isOverdue(date);
            }
        }
    private:
        // проверка просроченности
        bool isOverdue(string Fdate) {
            // Fdate - срок из файла
            int Fday = stoi(Fdate.substr(0, 2));
            int Fmonth = stoi(Fdate.substr(3, 2));
            int Fyear = stoi(Fdate.substr(6, 4));

            // получение текущей даты
            SYSTEMTIME sys_time;
            GetLocalTime(&sys_time);
            bool overdue = false;
            if (Fyear < sys_time.wYear) {
                overdue = true;
            }
            else if (Fyear == sys_time.wYear && Fmonth < sys_time.wMonth) {
                overdue = true;
            }
            else if (Fyear == sys_time.wYear && Fmonth == sys_time.wMonth && Fday < sys_time.wDay) {
                overdue = true;
            }
            return overdue;
        }
    };
    vector<Task> tasks;
    Group(string name) {
        this->name = name;
    }
};

// преобразование задачи в строку
string taskToStr(Group::Task Task) {
    string taskStr;
    taskStr = " - " + Task.task + "; [";
    if (Task.status) {
        taskStr += "выполнена";
    }
    else {
        taskStr += "не выполнена";
    }
    taskStr += "] срок: " + Task.date;
    if (Task.overdue) {
        taskStr += " {просрочено}";
    }
    return taskStr;
}

//сортировка
void sortGroup(vector<Group>* groups, int sort = 0) {
    vector<Group> temp = *groups;
    string priorities[3];
    if (sort) {
        priorities[0] = "низкий";
        priorities[1] = "средний";
        priorities[2] = "высокий";
    }
    else {
        priorities[0] = "высокий";
        priorities[1] = "средний";
        priorities[2] = "низкий";
    }
    groups->clear();
    for (int i = 0; i < temp.size(); i++) {
        (*groups).push_back(Group(temp[i].name));
        for (string priority : priorities) {
            for (int j = 0; j < temp[i].tasks.size(); j++) {
                if (temp[i].tasks[j].priority == priority) {
                    groups->at(i).tasks.push_back(Group::Task(temp[i].tasks[j].task, temp[i].tasks[j].priority, temp[i].tasks[j].status, temp[i].tasks[j].date));
                }
            }
        }
    }
}

// Вывод в консоль все задачи одной группы
void printTasks(vector<Group>* groups, int i, int sort = 0) {
    int id = 1;
    cout << "Группа \"" << groups->at(i).name << "\":" << endl;
    string priorities[3];
    if (sort) {
        priorities[0] = "низкий";
        priorities[1] = "средний";
        priorities[2] = "высокий";
    }
    else {
        priorities[0] = "высокий";
        priorities[1] = "средний";
        priorities[2] = "низкий";
    }
    for (string priority : priorities) {
        cout << "  " << priority << " приоритет:" << endl;
        for (int j = 0; j < groups->at(i).tasks.size(); j++) {
            Group::Task task = groups->at(i).tasks[j];
            if (task.priority == priority) {
                cout << "    " << id++ << ")" << taskToStr(task) << endl;
            }
        }
    }
    cout << endl;
}

// Вывод в консоль все группы и задачи
void printGroups(vector<Group>* groups, int sort = 0) {
    for (int i = 0; i < (*groups).size(); i++) {
        printTasks(groups, i, sort);
    }
}

// Запись в файл
void toFile(vector<Group>* groups, string tasksFileName) {
    ofstream otaskFile(tasksFileName);
    string priorities[]{ "высокий", "средний", "низкий" };
    for (int i = 0; i < (*groups).size(); i++) {
        int id = 1;
        otaskFile << "Группа \"" << groups->at(i).name << "\":" << endl;
        for (string priority : priorities) {
            otaskFile << "  " << priority << " приоритет:" << endl;
            for (int j = 0; j < groups->at(i).tasks.size(); j++) {
                Group::Task task = groups->at(i).tasks[j];
                if (task.priority == priority) {
                    otaskFile << "    " << id++ << ")" << taskToStr(task) << endl;
                }
            }
        }        
        otaskFile << endl;
    }
    otaskFile.close();
}

// Добавление группы
bool addGroup(vector<Group>* groups) {
    string groupName; // выбранная группа
    cout << "Введите название группы: ";
    groupName = "";
    SetConsoleCP(1251);
    getline(cin, groupName);
    SetConsoleCP(866);
    bool unique = true;
    for (int i = 0; i < (*groups).size(); i++) {
        if (groups->at(i).name == groupName) {
            cout << "Такое название уже используется\n";
            unique = false;
            break;
        }
    }
    if (unique) {
        (*groups).push_back(Group(groupName));
        return true;        
    }
    else {
        return false;
    }
}

//возвращает id группы по данному имени группы
int chooseGroup(vector<Group>* groups) {
    string groupName; // выбранная группа
    int groupId; //id выбранной группы
    cout << "Выбирете группу: ";
    groupName = "";
    SetConsoleCP(1251);
    getline(cin, groupName);
    SetConsoleCP(866);
    bool finded = false;
    for (int i = 0; i < (*groups).size(); i++) {
        if (groups->at(i).name == groupName) {
            groupId = i;
            finded = true;
            break;
        }
    }
    if (!finded) {
        groupId = -1;
        cout << "Группа не найдена\n";
    }
    else {
        cout << "Группа выбрана\n\n";
    }
    return groupId;
}

// проверка даты
bool checkDate(string data) {
    bool isdigits = true;
    string day = data.substr(0, 2);
    string month = data.substr(3, 2);
    string year = data.substr(6, 4);
    if (data.length() != 10) {
        return false;
    }
    else {
        // Проверка на то что дата состоит из чисел
        for (char ch : day) {
            if (!isdigit(ch)) {
                isdigits = false;
                break;
            }
        }
        for (char ch : month) {
            if (!isdigit(ch)) {
                isdigits = false;
                break;
            }
        }
        for (char ch : year) {
            if (!isdigit(ch)) {
                isdigits = false;
                break;
            }
        }
        if (!isdigits) {
            return false;
        }
        else {
            if (month == "01" || month == "03" || month == "05" || month == "07" || month == "08" || month == "10" || month == "12") {
                if (!(0 < stoi(day) && stoi(day) <= 31)) {
                    return false;
                }
            }
            else if (month == "04" || month == "06" || month == "09" || month == "01") {
                if (!(0 < stoi(day) && stoi(day) <= 30)) {
                    return false;
                }
            }
            else if (month == "02") {
                if (stoi(year) % 4 == 0 && (stoi(year) % 100 != 0 || stoi(year) % 400 == 0)) {
                    if (!(0 < stoi(day) && stoi(day) <= 29)) {
                        return false;
                    }
                }
                else {
                    if (!(0 < stoi(day) && stoi(month) <= 28)) {
                        return false;
                    }
                }
            }
            else {
                return false;
            }
        }
    }
    return true;
}

// Добавление задачи
bool addTask(vector<Group>* groups, int groupId) {
    string task;
    string status;
    bool bStatus;
    string priority;
    string date;
    if (groupId == -1) {
        groupId = chooseGroup(groups);
    }
    if (groupId == -1) {
        return false;
    }
    cout << "Введите описание задачи: ";
    SetConsoleCP(1251);
    getline(cin, task);
    SetConsoleCP(866);
    cout << "Выбирете приоритет (высокий, средний, низкий): ";
    SetConsoleCP(1251);
    getline(cin, priority);
    SetConsoleCP(866);
    if (priority != "высокий" && priority != "средний" && priority != "низкий") {
        cout << "Неправильно введены данные\n";
        return false;
    }
    cout << "укажите статус задачи (выполнена, не выполнена): ";
    SetConsoleCP(1251);
    getline(cin, status);
    SetConsoleCP(866);
    if (status != "выполнена" && status != "не выполнена") {
        cout << "Неправильно введены данные\n";
        return false;
    }
    if (status == "выполнена") {
        bStatus = true;
    }
    else {
        bStatus = false;
    }
    cout << "Укажите срок выполнения задачи (dd.mm.yyyy): ";
    SetConsoleCP(1251);
    getline(cin, date);
    SetConsoleCP(866);
    if (!checkDate(date)) {
        cout << "Неверный формат даты\n";
        return false;
    }
    groups->at(groupId).tasks.push_back(Group::Task(task, priority, bStatus, date));
    return true;
}

// Удаление задачи
bool deleteTask(vector<Group>* groups, int groupId) {
    if (groupId == -1) {
        groupId = chooseGroup(groups);
    }
    if (groupId == -1) {
        cout << "Ошибка добавления задачи\n";
        return false;
    }
    cout << "введите номер задачи для удаления: ";
    int taskId;
    string staskId;
    SetConsoleCP(1251);
    getline(cin, staskId);
    SetConsoleCP(866);
    taskId = stoi(staskId);
    taskId--;
    if ((groups->at(groupId).tasks.size() - 1) >= taskId && taskId > -1) {
        groups->at(groupId).tasks.erase(groups->at(groupId).tasks.begin() + taskId);
        return true;
    }
    else {
        return false;
    }
}

// Изменение данных
bool changeTask(vector<Group>* groups, int groupId) {

    if (groupId == -1) {
        groupId = chooseGroup(groups);
    }
    if (groupId == -1) {
        return false;
    }
    cout << "введите номер задачи для изменения: ";
    int taskId;
    string staskId;
    SetConsoleCP(1251);
    getline(cin, staskId);
    SetConsoleCP(866);
    taskId = stoi(staskId);
    taskId--;
    if (!((groups->at(groupId).tasks.size() - 1) >= taskId && taskId > -1)) {
        cout << "Такого номера нет\n";
        return false;
    }

    cout << "Выбирете что вы хотите изменить в задаче? (описание, статус, приоритет, срок)\n";
    string option; // выбор пользователя
    string data; // переменная для данных на которые будут заменены
    SetConsoleCP(1251);
    getline(cin, option);
    SetConsoleCP(866);
    if (option == "описание") {
        cout << "Введите новое описание задачи: ";
        getline(cin, data);
        groups->at(groupId).tasks[taskId].task = data;
    }
    else if (option == "приоритет") {
        cout << "Введите новый приоритет(высокий, средний, низкий): ";
        SetConsoleCP(1251);
        getline(cin, data);
        SetConsoleCP(866);
        if (data != "высокий" && data != "средний" && data != "низкий") {
            cout << "Неправильно введены данные\n";
            return false;
        }
        groups->at(groupId).tasks[taskId].priority = data;
    }
    else if (option == "статус") {
        cout << "Введите новый статус(выполнена, не выполнена): ";
        SetConsoleCP(1251);
        getline(cin, data);
        SetConsoleCP(866);
        if (data != "выполнена" && data != "не выполнена") {
            cout << "Неправильно введены данные\n";
            return false;
        }
        bool bStatus = data == "выполнена" ? true : false;
        groups->at(groupId).tasks[taskId].newStatus(bStatus);
    }
    else if (option == "срок") {
        cout << "Введите новую дату(dd.mm.yyyy): ";
        SetConsoleCP(1251);
        getline(cin, data);
        SetConsoleCP(866);
        // проверка даты
        if (!checkDate(data)) {
            cout << "Неверный формат даты\n";
            return false;
        }
        groups->at(groupId).tasks[taskId].newDate(data);
    }
    else {
        cout << "Неизвестная команда\n";
        return false;
    }
    return true;
}

// Удаление группы
bool deleteGroup(vector<Group>* groups) {
    int groupId = chooseGroup(groups);
    if (groupId == -1) {
        return false;
    }
    (*groups).erase((*groups).begin() + groupId);
    return true;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    string tasksFileName = "tasks.txt"; // имя файла
    vector<Group> groups; // Массив групп
    // создание массива групп и задач из файла
    ifstream tasksFile(tasksFileName);
    int id = -1;
    //создание файла если его нет
    ofstream otaskFile(tasksFileName, ios::app);
    otaskFile.close();
    while (!tasksFile.eof()) {
        string line;
        getline(tasksFile, line);
        if (line.rfind(':') != string::npos) {
            groups.push_back(Group(line.substr(8, line.rfind('\"') - 8)));
            id++;
        }
        else {
            break;
        }
        getline(tasksFile, line);
        while (line.find(':') != string::npos) {
            string priority = line.substr(2, line.rfind(' ') - 2);
            while (true) {
                getline(tasksFile, line);
                string task;
                string status;
                bool bStatus;
                string date;
                if (line.find(')') != string::npos) {
                    task = line.substr(line.find('-') + 2, line.find(';') - (line.find('-') + 2));
                    status = line.substr(line.find('[') + 1, line.rfind(']') - (line.find('[') + 1));
                    date = line.substr(line.find(':') + 2, 10);
                    if (status == "выполнена") {
                        bStatus = true;
                    }
                    else {
                        bStatus = false;
                    }
                    groups[id].tasks.push_back(Group::Task(task, priority, bStatus, date));
                }
                else {
                    break;
                }
            }
        }
    }
    sortGroup(&groups);
    tasksFile.close();
    //   
    int groupId = -1; //id выбранной группы
    string option; // выбранная команда
    int sort = 0; // способ сортировки
    // список команд
    string help = R"(Доступные команды:
"помощь" - список команд;
"добавить группу" - создание группы;
"выбрать группу" - сфокусироваться на одной группе;
"группы" - вывести на экран все группы;
"добавить задачу" - добавление задачи;
"удалить задачу" - удаление задачи;
"удалить группу" - удаление группы;
"изменить" - изменить данные в задаче;
"сортировка" - остортировать задачи;
"выход" - выход из программы;)";
    cout << help << endl << endl;
    printGroups(&groups); // вывод всех групп и задач
    while (true) {
        cout << "Введите команду: ";
        SetConsoleCP(1251);
        getline(cin, option);
        SetConsoleCP(866);
        if (option == "помощь") {
            cout << help << endl;
        }
        else if (option == "добавить группу") {
            // Добавление группы
            if (addGroup(&groups)) {
                cout << "Группа создана\n\n";
                printGroups(&groups, sort);
            }
            else {
                cout << "Группа не создана\n\n";
            }
            //
        }

        else if (option == "выбрать группу") {
            //сфокусироваться на группе
            groupId = chooseGroup(&groups);
            if (groupId != -1) {
                printTasks(&groups, groupId, sort);
            }
            //
        }

        else if (option == "добавить задачу") {
            // Добавление задачи
            if (addTask(&groups, groupId)) {
                cout << "Задача добавлена\n\n";
                if (groupId == -1) {
                    printGroups(&groups, sort);
                }
                else {
                    printTasks(&groups, groupId, sort);
                }
            }
            else {
                cout << "Задача не добавлена\n\n";
            }
        }

        else if (option == "удалить задачу") {
            // Удаление задачи
            if (deleteTask(&groups, groupId)) {
                cout << "Задача удалена\n\n";
                if (groupId == -1) {
                    printGroups(&groups, sort);
                }
                else {
                    printTasks(&groups, groupId, sort);
                }
            }
            else {
                cout << "такого номера нет\n";

            }
        }
        else if (option == "удалить группу") {
            // Удаление группы
            if (deleteGroup(&groups)) {
                cout << "группа удалена\n";
                printGroups(&groups, sort);
            }
            else {
                cout << "группа не удалена\n";
            }
        }
        else if (option == "изменить") {
            if (changeTask(&groups, groupId)) {
                cout << "Данные изменены\n\n";
                if (groupId == -1) {
                    printGroups(&groups, sort);
                }
                else {
                    printTasks(&groups, groupId, sort);
                }
            }
        }
        else if (option == "выход") {
            return 1;
        }
        else if (option == "группы") {
            groupId = -1;
            printGroups(&groups, sort);
        }
        else if (option == "сортировка") {
            cout << "выбирете способ сортировки по приоритетности(1 - по возрастанию, 0 по убыванию)\n";
            cin >> sort;
            getline(cin, option);
            sortGroup(&groups, sort);
            if (groupId == -1) {
                printGroups(&groups, sort);
            }
            else {
                printTasks(&groups, groupId, sort);
            }
        }
        else {
            cout << "Неизвестная команда\n";
        }
        // Запись в файл
        toFile(&groups, tasksFileName);
    }
    return 0;
}
