//#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <ctime>
#include <map>
using namespace std;

// ����� ������ �����
class Group
{
public:
    string name; //�������� ������
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
        // �������� ��������������
        bool isOverdue(string Fdate) {
            // Fdate - ���� �� �����
            int Fday = stoi(Fdate.substr(0, 2));
            int Fmonth = stoi(Fdate.substr(3, 2));
            int Fyear = stoi(Fdate.substr(6, 4));

            // ��������� ������� ����
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

// �������������� ������ � ������
string taskToStr(Group::Task Task) {
    string taskStr;
    taskStr = " - " + Task.task + "; [";
    if (Task.status) {
        taskStr += "���������";
    }
    else {
        taskStr += "�� ���������";
    }
    taskStr += "] ����: " + Task.date;
    if (Task.overdue) {
        taskStr += " {����������}";
    }
    return taskStr;
}

//����������
void sortGroup(vector<Group>* groups, int sort = 0) {
    vector<Group> temp = *groups;
    string priorities[3];
    if (sort) {
        priorities[0] = "������";
        priorities[1] = "�������";
        priorities[2] = "�������";
    }
    else {
        priorities[0] = "�������";
        priorities[1] = "�������";
        priorities[2] = "������";
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

// ����� � ������� ��� ������ ����� ������
void printTasks(vector<Group>* groups, int i, int sort = 0) {
    int id = 1;
    cout << "������ \"" << groups->at(i).name << "\":" << endl;
    string priorities[3];
    if (sort) {
        priorities[0] = "������";
        priorities[1] = "�������";
        priorities[2] = "�������";
    }
    else {
        priorities[0] = "�������";
        priorities[1] = "�������";
        priorities[2] = "������";
    }
    for (string priority : priorities) {
        cout << "  " << priority << " ���������:" << endl;
        for (int j = 0; j < groups->at(i).tasks.size(); j++) {
            Group::Task task = groups->at(i).tasks[j];
            if (task.priority == priority) {
                cout << "    " << id++ << ")" << taskToStr(task) << endl;
            }
        }
    }
    cout << endl;
}

// ����� � ������� ��� ������ � ������
void printGroups(vector<Group>* groups, int sort = 0) {
    for (int i = 0; i < (*groups).size(); i++) {
        printTasks(groups, i, sort);
    }
}

// ������ � ����
void toFile(vector<Group>* groups, string tasksFileName) {
    ofstream otaskFile(tasksFileName);
    string priorities[]{ "�������", "�������", "������" };
    for (int i = 0; i < (*groups).size(); i++) {
        int id = 1;
        otaskFile << "������ \"" << groups->at(i).name << "\":" << endl;
        for (string priority : priorities) {
            otaskFile << "  " << priority << " ���������:" << endl;
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

// ���������� ������
bool addGroup(vector<Group>* groups) {
    string groupName; // ��������� ������
    cout << "������� �������� ������: ";
    groupName = "";
    SetConsoleCP(1251);
    getline(cin, groupName);
    SetConsoleCP(866);
    bool unique = true;
    for (int i = 0; i < (*groups).size(); i++) {
        if (groups->at(i).name == groupName) {
            cout << "����� �������� ��� ������������\n";
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

//���������� id ������ �� ������� ����� ������
int chooseGroup(vector<Group>* groups) {
    string groupName; // ��������� ������
    int groupId; //id ��������� ������
    cout << "�������� ������: ";
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
        cout << "������ �� �������\n";
    }
    else {
        cout << "������ �������\n\n";
    }
    return groupId;
}

// �������� ����
bool checkDate(string data) {
    bool isdigits = true;
    string day = data.substr(0, 2);
    string month = data.substr(3, 2);
    string year = data.substr(6, 4);
    if (data.length() != 10) {
        return false;
    }
    else {
        // �������� �� �� ��� ���� ������� �� �����
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

// ���������� ������
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
    cout << "������� �������� ������: ";
    SetConsoleCP(1251);
    getline(cin, task);
    SetConsoleCP(866);
    cout << "�������� ��������� (�������, �������, ������): ";
    SetConsoleCP(1251);
    getline(cin, priority);
    SetConsoleCP(866);
    if (priority != "�������" && priority != "�������" && priority != "������") {
        cout << "����������� ������� ������\n";
        return false;
    }
    cout << "������� ������ ������ (���������, �� ���������): ";
    SetConsoleCP(1251);
    getline(cin, status);
    SetConsoleCP(866);
    if (status != "���������" && status != "�� ���������") {
        cout << "����������� ������� ������\n";
        return false;
    }
    if (status == "���������") {
        bStatus = true;
    }
    else {
        bStatus = false;
    }
    cout << "������� ���� ���������� ������ (dd.mm.yyyy): ";
    SetConsoleCP(1251);
    getline(cin, date);
    SetConsoleCP(866);
    if (!checkDate(date)) {
        cout << "�������� ������ ����\n";
        return false;
    }
    groups->at(groupId).tasks.push_back(Group::Task(task, priority, bStatus, date));
    return true;
}

// �������� ������
bool deleteTask(vector<Group>* groups, int groupId) {
    if (groupId == -1) {
        groupId = chooseGroup(groups);
    }
    if (groupId == -1) {
        cout << "������ ���������� ������\n";
        return false;
    }
    cout << "������� ����� ������ ��� ��������: ";
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

// ��������� ������
bool changeTask(vector<Group>* groups, int groupId) {

    if (groupId == -1) {
        groupId = chooseGroup(groups);
    }
    if (groupId == -1) {
        return false;
    }
    cout << "������� ����� ������ ��� ���������: ";
    int taskId;
    string staskId;
    SetConsoleCP(1251);
    getline(cin, staskId);
    SetConsoleCP(866);
    taskId = stoi(staskId);
    taskId--;
    if (!((groups->at(groupId).tasks.size() - 1) >= taskId && taskId > -1)) {
        cout << "������ ������ ���\n";
        return false;
    }

    cout << "�������� ��� �� ������ �������� � ������? (��������, ������, ���������, ����)\n";
    string option; // ����� ������������
    string data; // ���������� ��� ������ �� ������� ����� ��������
    SetConsoleCP(1251);
    getline(cin, option);
    SetConsoleCP(866);
    if (option == "��������") {
        cout << "������� ����� �������� ������: ";
        getline(cin, data);
        groups->at(groupId).tasks[taskId].task = data;
    }
    else if (option == "���������") {
        cout << "������� ����� ���������(�������, �������, ������): ";
        SetConsoleCP(1251);
        getline(cin, data);
        SetConsoleCP(866);
        if (data != "�������" && data != "�������" && data != "������") {
            cout << "����������� ������� ������\n";
            return false;
        }
        groups->at(groupId).tasks[taskId].priority = data;
    }
    else if (option == "������") {
        cout << "������� ����� ������(���������, �� ���������): ";
        SetConsoleCP(1251);
        getline(cin, data);
        SetConsoleCP(866);
        if (data != "���������" && data != "�� ���������") {
            cout << "����������� ������� ������\n";
            return false;
        }
        bool bStatus = data == "���������" ? true : false;
        groups->at(groupId).tasks[taskId].newStatus(bStatus);
    }
    else if (option == "����") {
        cout << "������� ����� ����(dd.mm.yyyy): ";
        SetConsoleCP(1251);
        getline(cin, data);
        SetConsoleCP(866);
        // �������� ����
        if (!checkDate(data)) {
            cout << "�������� ������ ����\n";
            return false;
        }
        groups->at(groupId).tasks[taskId].newDate(data);
    }
    else {
        cout << "����������� �������\n";
        return false;
    }
    return true;
}

// �������� ������
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
    string tasksFileName = "tasks.txt"; // ��� �����
    vector<Group> groups; // ������ �����
    // �������� ������� ����� � ����� �� �����
    ifstream tasksFile(tasksFileName);
    int id = -1;
    //�������� ����� ���� ��� ���
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
                    if (status == "���������") {
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
    int groupId = -1; //id ��������� ������
    string option; // ��������� �������
    int sort = 0; // ������ ����������
    // ������ ������
    string help = R"(��������� �������:
"������" - ������ ������;
"�������� ������" - �������� ������;
"������� ������" - ��������������� �� ����� ������;
"������" - ������� �� ����� ��� ������;
"�������� ������" - ���������� ������;
"������� ������" - �������� ������;
"������� ������" - �������� ������;
"��������" - �������� ������ � ������;
"����������" - ������������� ������;
"�����" - ����� �� ���������;)";
    cout << help << endl << endl;
    printGroups(&groups); // ����� ���� ����� � �����
    while (true) {
        cout << "������� �������: ";
        SetConsoleCP(1251);
        getline(cin, option);
        SetConsoleCP(866);
        if (option == "������") {
            cout << help << endl;
        }
        else if (option == "�������� ������") {
            // ���������� ������
            if (addGroup(&groups)) {
                cout << "������ �������\n\n";
                printGroups(&groups, sort);
            }
            else {
                cout << "������ �� �������\n\n";
            }
            //
        }

        else if (option == "������� ������") {
            //��������������� �� ������
            groupId = chooseGroup(&groups);
            if (groupId != -1) {
                printTasks(&groups, groupId, sort);
            }
            //
        }

        else if (option == "�������� ������") {
            // ���������� ������
            if (addTask(&groups, groupId)) {
                cout << "������ ���������\n\n";
                if (groupId == -1) {
                    printGroups(&groups, sort);
                }
                else {
                    printTasks(&groups, groupId, sort);
                }
            }
            else {
                cout << "������ �� ���������\n\n";
            }
        }

        else if (option == "������� ������") {
            // �������� ������
            if (deleteTask(&groups, groupId)) {
                cout << "������ �������\n\n";
                if (groupId == -1) {
                    printGroups(&groups, sort);
                }
                else {
                    printTasks(&groups, groupId, sort);
                }
            }
            else {
                cout << "������ ������ ���\n";

            }
        }
        else if (option == "������� ������") {
            // �������� ������
            if (deleteGroup(&groups)) {
                cout << "������ �������\n";
                printGroups(&groups, sort);
            }
            else {
                cout << "������ �� �������\n";
            }
        }
        else if (option == "��������") {
            if (changeTask(&groups, groupId)) {
                cout << "������ ��������\n\n";
                if (groupId == -1) {
                    printGroups(&groups, sort);
                }
                else {
                    printTasks(&groups, groupId, sort);
                }
            }
        }
        else if (option == "�����") {
            return 1;
        }
        else if (option == "������") {
            groupId = -1;
            printGroups(&groups, sort);
        }
        else if (option == "����������") {
            cout << "�������� ������ ���������� �� ��������������(1 - �� �����������, 0 �� ��������)\n";
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
            cout << "����������� �������\n";
        }
        // ������ � ����
        toFile(&groups, tasksFileName);
    }
    return 0;
}
