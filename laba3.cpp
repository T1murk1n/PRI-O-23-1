#include<iostream>
#include <vector>
#include <set>

using namespace std;

#define A 0
#define B 100

int main()
{
	setlocale(0, "");
	int size, k = 0, znach, ind, ind1, ind2, flag = 1, flag1 = 0;
	string otv;
	cout << "Введите размер массива: ";
	cin >> size;
	vector <int> massiv;
	set <int> index;

	cout << "Вы хотите ввести значения чисел самостоятельно? (yes, no) ";
	cin >> otv;


	if (otv == "yes")
	{
		while (k < size)
		{
			cin >> znach;
			massiv.push_back(znach);
			k++;
		}

		cout << "Заданный массив: ";

		for (int m : massiv)
		{
			cout << m << " ";
		}

		for (int pr : massiv)
		{
			if (pr >= A && pr <= B)
			{
				for (ind = 0; ind < size; ind++)
				{
					for (ind1 = 0; ind1 < size; ind1++)
					{
						for (ind2 = 0; ind2 < size; ind2++)
						{
							if ((ind != ind1 && ind != ind2 && ind1 != ind2) && (massiv[ind] == massiv[ind1] + massiv[ind2]))
							{
								index.insert(ind);
								flag1 = 1;
							}
						}
					}
				}
			}
			else flag = 0;
		}

		if (flag == 0)
		{
			cout << "Значения введены некорректно, попробуйте ещё раз.";
		}

		else
		{
			cout << "\nИндексы значений, удовлетворяющих условию: ";
			
			for (int n : index)
			{
				cout << n << " ";
			}

			if (flag1 == 0)
			{
				cout << "В данном массиве нет значений, удолетворяющих условию.\n";
			}
			cout << "\n";
		}

		

	}
	else
	{
		while (k < size)
		{
			znach = A + rand() % (B - A + 1);
			massiv.push_back(znach);
			k++;
		}

		cout << "Заданный массив: ";

		for (int n : massiv)
		{
			cout << n << " ";
		}

		cout << "\n";

		for (ind = 0; ind < size; ind++)
		{
			for (ind1 = 0; ind1 < size; ind1++)
			{
				for (ind2 = 0; ind2 < size; ind2++)
				{
					if ((ind != ind1 && ind != ind2 && ind1 != ind2) && (massiv[ind] == massiv[ind1] + massiv[ind2]))
					{
						index.insert(ind);
					}
					else flag1 = 0;
				}
			}
		}

		cout << "\nИндексы значений, удовлетворяющих условию: ";

		for (int n : index)
		{
			cout << n << " ";
		}
		
		if (flag1==0)
		{
			cout << "В данном массиве нет значений, удовлетворяющих условию\n";
		}
		cout<<"\n";
	}
}