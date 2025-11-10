/*Первое задание
Создайте игру «Морской бой».
Главные особенности реализации игры:
■ Режимы игры:
• Человек – компьютер;
• Компьютер – компьютер.
■ Расстановка кораблей:
• Игрок расставляет вручную;
• Компьютер расставляет за игрока.
■ Компьютер может играть в двух режимах:
• Случайный выстрел;
• Интеллектуальная игра. Компьютер строит стратегию игры, а не стреляет случайно.
■ Отображать текущее состояние игрового поля игрока и компьютера.
■ Возможность остановки, приостановки, нового запуска игры.*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#define DEBUG 1

using namespace std;

const int SIZE_FIELD = 12;
const int INDEX_GAME = 5;

struct ship {
	int indexRows[INDEX_GAME - 1]{ 0 };
	int indexCols[INDEX_GAME - 1]{ 0 };
	int decks{ 0 };
};

void PrintTitle(int);
void PrintUpLine();
void PrintMiddle_1_Line();
void PrintMiddle_2_Line();
void PrintDownLine();
void Print_USER_COMP_Fields(int fField1[][SIZE_FIELD], int fField2[][SIZE_FIELD]);
void Print_COMP_COMP_Fields(int fField1[][SIZE_FIELD], int fField2[][SIZE_FIELD]);
void FillFields(int[][SIZE_FIELD], int[][SIZE_FIELD], ship*, ship*, int);
void AutoFillFields(int[][SIZE_FIELD], ship*, int, int, int);
void ManualFillFields(int[][SIZE_FIELD], ship*, int, int, int);
void InputData(int[][SIZE_FIELD], ship*, int, int, int, int, int);
bool CheckPosition(int[][SIZE_FIELD], int, int, int, int);
void Game(int[][SIZE_FIELD], int[][SIZE_FIELD], ship*, ship*, ship*, ship*, int);
void PrintCount(int, int);
bool ShotPlayer(int[][SIZE_FIELD], ship*, ship*, int&);
bool ShotComputer(int[][SIZE_FIELD], ship*, ship*, int&);
bool ShotSmartComputer(int[][SIZE_FIELD], ship*, ship*, int&);
bool CheckShot(int[][SIZE_FIELD], ship*, ship*, int, int, int&);

int main()
{
	system("title МОРСКОЙ БОЙ");
	setlocale(LC_ALL, "");
	srand(unsigned(time(NULL)));
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, 15);

	int field1[SIZE_FIELD][SIZE_FIELD]{ 0 }, field2[SIZE_FIELD][SIZE_FIELD]{ 0 };
	ship* shipsField1 = new ship[INDEX_GAME * 2];
	if (!shipsField1) { cerr << "NO MEMORY"; return 1; }
	ship* shipsField2 = new ship[INDEX_GAME * 2];
	if (!shipsField2) { cerr << "NO MEMORY"; return 1; }
	ship* copySF1 = new ship[INDEX_GAME * 2];
	if (!copySF1) { cerr << "NO MEMORY"; return 1; }
	ship* copySF2 = new ship[INDEX_GAME * 2];
	if (!copySF2) { cerr << "NO MEMORY"; return 1; }

	cout << "\nВыберите режим игры"
		<< "\n1 - Человек-Компьютер"
		<< "\n2 - Комьютер-Компьютер"
		<< "\n3 - Человек-СмартКомпьютер"
		<< "\n4 - СмартКомпьютер-СмартКомпьютер";
	int choice(0);
	do {
		cout << "\nСделайте выбор(от 1 до 4): ";
		cin >> choice;
		if (cin.fail()) { cin.clear(); while (cin.get() != '\n'); }
	} while (choice < 1 || choice > 4);

	int arrangement(2);
	if (choice % 2) {
		cout << "\nХотите расставить свои корабли вручную('y' или 'Y' для да)?: \n";
		char ch;
		cin >> ch;
		if ('Y' == toupper(ch))	arrangement = 1;
	}
	FillFields(field1, field2, shipsField1, shipsField2, arrangement);

	for (int i = 0; i < INDEX_GAME * 2; ++i) {
		copySF1[i] = shipsField1[i];
		copySF2[i] = shipsField2[i];
	}

	Game(field1, field2, shipsField1, shipsField2, copySF1, copySF2, choice);

	delete[] shipsField1;
	delete[] shipsField2;
	delete[] copySF1;
	delete[] copySF2;
	system("pause");
	return 0;
}

//Функция вывода заголовка
void PrintTitle(int fChoice)
{
	setlocale(LC_ALL, "C");
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, 11);
	switch (fChoice)
	{
	case 1:
		PrintUpLine();
		PrintMiddle_1_Line();
		PrintDownLine();
		break;
	case 2:
		PrintUpLine();
		PrintMiddle_2_Line();
		PrintDownLine();
		break;
	case 3:
		PrintUpLine();
		PrintMiddle_1_Line();
		PrintDownLine();
		break;
	case 4:
		PrintUpLine();
		PrintMiddle_2_Line();
		PrintDownLine();
		break;
	}
	SetConsoleTextAttribute(hStdOut, 15);
	setlocale(LC_ALL, "");
}
//вывод верхней линии заголовка
void PrintUpLine()
{
	cout << "     " << char(201);
	for (int i = 0; i < SIZE_FIELD; ++i) { cout << char(205); }
	cout << char(187);

	for (int i = 0; i < INDEX_GAME; ++i) { cout << "     "; }

	cout << "    " << char(201);
	for (int i = 0; i < SIZE_FIELD; ++i) { cout << char(205); }
	cout << char(187);
	cout << "\n";
}
//вывод средней линии заголовка
void PrintMiddle_1_Line()
{
	cout << "     " << char(186);
	cout << setw(12) << "   Player   " << char(186);
	for (int i = 0; i < INDEX_GAME; ++i) { cout << "     "; }
	cout << "    " << char(186);
	cout << setw(12) << "  Computer  " << char(186);
}
void PrintMiddle_2_Line()
{
	cout << "     " << char(186);
	cout << setw(12) << " Computer 1 " << char(186);
	for (int i = 0; i < INDEX_GAME; ++i) { cout << "     "; }
	cout << "    " << char(186);
	cout << setw(12) << " Computer 2 " << char(186);
}
//вывод нижней линии заголовка
void PrintDownLine()
{
	cout << "\n     " << char(200);
	for (int i = 0; i < SIZE_FIELD; ++i) { cout << char(205); }
	cout << char(188);

	for (int i = 0; i < INDEX_GAME; ++i) { cout << "     "; }

	cout << "    " << char(200);
	for (int i = 0; i < SIZE_FIELD; ++i) { cout << char(205); }
	cout << char(188);
	cout << "\n";
}

//отображение полей в режиме пользователь-компьютер
void Print_USER_COMP_Fields(int fField1[][SIZE_FIELD], int fField2[][SIZE_FIELD])
{
	setlocale(LC_ALL, "C");
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, 11);

	char letters[SIZE_FIELD]{ '0', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', '0' };
	int x = SIZE_FIELD - 1;

	//буквы первого поля
	cout << "   ";
	for (int i = 1; i < x; ++i) {
		cout << letters[i] << " ";
	}

	//промежуток между полями
	cout.width(21); cout << " ";

	//буквы второго поля
	cout << "   ";
	for (int i = 1; i < x; ++i) {
		cout << letters[i] << " ";
	}
	cout << "\n";
	//прорисовка полей
	for (int i = 1; i < x; ++i) {
		//первое поле
		cout.width(2);
		cout << i << "|";
		for (int j = 1; j < x; ++j) {
			switch (fField1[i][j]) {
			case  1:
				SetConsoleTextAttribute(hStdOut, 10);
				cout << char(254) << " "; break;
			case  0:
				SetConsoleTextAttribute(hStdOut, 11);
				cout << char(126) << " "; break;
			case -1:
				SetConsoleTextAttribute(hStdOut, 11);
				cout << char(88) << " "; break;
			case -2:
				SetConsoleTextAttribute(hStdOut, 12);
				cout << char(177) << " ";
			}
		}
		//промежуток
		SetConsoleTextAttribute(hStdOut, 15);
		cout.width(21); cout << " ";
		//второе поле
		SetConsoleTextAttribute(hStdOut, 11);
		cout.width(2);
		cout << i << "|";
		for (int j = 1; j < x; ++j) {
			switch (fField2[i][j]) {
			case  1:
			case  0:
				SetConsoleTextAttribute(hStdOut, 11);
				cout << char(126) << " "; break;
			case -1:
				SetConsoleTextAttribute(hStdOut, 11);
				cout << char(88) << " "; break;
			case -2:
				SetConsoleTextAttribute(hStdOut, 12);
				cout << char(177) << " ";
			}
		}
		cout << "\n";
	}
	SetConsoleTextAttribute(hStdOut, 15);
	setlocale(LC_ALL, "");
}
//отображение полей в режиме компьютер-компьютер
void Print_COMP_COMP_Fields(int fField1[][SIZE_FIELD], int fField2[][SIZE_FIELD])
{
	setlocale(LC_ALL, "C");
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, 11);

	char letters[SIZE_FIELD]{ '0', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', '0' };
	int x = SIZE_FIELD - 1;
	//буквы первого поля
	cout << "   ";
	for (int i = 1; i < x; ++i) {
		cout << letters[i] << " ";
	}
	//промежуток между полями
	cout.width(21); cout << " ";
	//буквы второго поля
	cout << "   ";
	for (int i = 1; i < x; ++i) {
		cout << letters[i] << " ";
	}
	cout << "\n";

	//прорисовка полей
	for (int i = 1; i < x; ++i) {
		//первое поле
		cout.width(2);
		cout << i << "|";
		for (int j = 1; j < x; ++j) {
			switch (fField1[i][j]) {
			case  1:
				SetConsoleTextAttribute(hStdOut, 10);
				cout << char(254) << " "; break;
			case  0:
				SetConsoleTextAttribute(hStdOut, 11);
				cout << char(126) << " "; break;
			case -1:
				SetConsoleTextAttribute(hStdOut, 11);
				cout << char(88) << " "; break;
			case -2:
				SetConsoleTextAttribute(hStdOut, 12);
				cout << char(177) << " ";
			}
		}
		//промежуток
		cout.width(21); cout << " ";
		//второе поле
		cout.width(2);
		cout << i << "|";
		for (int j = 1; j < x; ++j) {
			switch (fField2[i][j]) {
			case  1:
				SetConsoleTextAttribute(hStdOut, 10);
				cout << char(254) << " "; break;
			case  0:
				SetConsoleTextAttribute(hStdOut, 11);
				cout << char(126) << " "; break;
			case -1:
				SetConsoleTextAttribute(hStdOut, 11);
				cout << char(88) << " "; break;
			case -2:
				SetConsoleTextAttribute(hStdOut, 12);
				cout << char(177) << " ";
			}
		}
		cout << "\n";
		SetConsoleTextAttribute(hStdOut, 11);
	}
	SetConsoleTextAttribute(hStdOut, 15);
	setlocale(LC_ALL, "");
}

//функция заполнения полей
void FillFields(int fField1[][SIZE_FIELD], int fField2[][SIZE_FIELD], ship* fShipsField1, ship* fShipsField2,
	int fArrangement)
{
	int counter(0);
	void (*ptrFillFunct[2])(int[][SIZE_FIELD], ship*, int, int, int) {
		ManualFillFields,
			AutoFillFields
	};

	switch (fArrangement)
	{
	case 1:
		for (int i = 1; i < INDEX_GAME; ++i)
		{
			system("cls");
			Print_USER_COMP_Fields(fField1, fField2);
			cout << "\nПоставте " << i << " " << INDEX_GAME - i << "-х палубных корабля"
				<< " в первом поле\n";
			ptrFillFunct[0](fField1, fShipsField1, i, INDEX_GAME - i, counter);
			system("cls");
			Print_USER_COMP_Fields(fField1, fField2);
			cout << "\nКомпьютер расставляет " << i << " " << INDEX_GAME - i << "-х палубных корабля"
				<< " во втором поле\n";
			ptrFillFunct[1](fField2, fShipsField2, i, INDEX_GAME - i, counter);
			system("cls");
			Print_USER_COMP_Fields(fField1, fField2);
			counter += i;
		}
		break;
	case 2:
		for (int i = 1; i < INDEX_GAME; ++i)
		{
			ptrFillFunct[1](fField1, fShipsField1, i, INDEX_GAME - i, counter);
			ptrFillFunct[1](fField2, fShipsField2, i, INDEX_GAME - i, counter);
			counter += i;
		}
	}
}

//функция ручного заполнения поля
void ManualFillFields(int fField[][SIZE_FIELD], ship* fShipsField, int fAmountShips, int fAmountDecks,
	int fCounter)
{
	int fRow(0), fCol(0), direction, x(0);
	char tempCh;

	while (fAmountShips > 0) {
		++x;
		do {
			cout << "\nКорабль " << x;
			do {
				cout << "\nСтрока(от 1 до 10): ";
				cin >> fRow;
				if (cin.fail()) { cin.clear(); while (cin.get() != '\n'); }
			} while ((fRow < 1 || fRow > 10));
			do {
				cout << "\nСтолбец(от A до J): ";
				cin >> tempCh;
				while (cin.get() != '\n');
				fCol = int(toupper(tempCh)) - 64;;
			} while (fCol < 1 || fCol > 10);

			cout << "Введите направление прорисовки"
				<< "\n1 - Вверх"
				<< "\n2 - Вправо"
				<< "\n3 - Вниз"
				<< "\n4 - Влево";
			do {
				cout << "\nСделайте выбор(от 1 до 4): ";
				cin >> direction;
				if (cin.fail()) { cin.clear(); while (cin.get() != '\n'); }
			} while (direction < 1 || direction > 4);

		} while (!CheckPosition(fField, fRow, fCol, direction, fAmountDecks));
		InputData(fField, fShipsField, fRow, fCol, direction, fAmountDecks, fCounter++);
		--fAmountShips;
	}
}

//функция автоматического заполнения поля
void AutoFillFields(int fField[][SIZE_FIELD], ship* fShipsField, int fAmountShips, int fAmountDecks,
	int fCounter)
{
	int fRow, fCol, direction;
	while (fAmountShips > 0) {
		do
		{
			fRow = rand() % 10 + 1;
			fCol = rand() % 10 + 1;
			direction = rand() % 4 + 1;
		} while (!CheckPosition(fField, fRow, fCol, direction, fAmountDecks));
		InputData(fField, fShipsField, fRow, fCol, direction, fAmountDecks, fCounter++);
		--fAmountShips;
	}
}

//ввод данных в поля и структуры
void InputData(int fField[][SIZE_FIELD], ship* fShipsField, int fRow, int fCol, int fDirection,
	int fAmountDecks, int fCounter)
{
	int n(0);
	switch (fDirection)
	{
	case 1:
		for (int i = fRow; i > fRow - fAmountDecks; --i) {
			fField[i][fCol] = 1;
			fShipsField[fCounter].indexRows[n] = i;
			fShipsField[fCounter].indexCols[n++] = fCol;
			++(fShipsField[fCounter].decks);
		}
		break;
	case 2:
		for (int i = fCol; i < fCol + fAmountDecks; ++i) {
			fField[fRow][i] = 1;
			fShipsField[fCounter].indexRows[n] = fRow;
			fShipsField[fCounter].indexCols[n++] = i;
			++(fShipsField[fCounter].decks);
		}
		break;
	case 3:
		for (int i = fRow; i < fRow + fAmountDecks; ++i) {
			fField[i][fCol] = 1;
			fShipsField[fCounter].indexRows[n] = i;
			fShipsField[fCounter].indexCols[n++] = fCol;
			++(fShipsField[fCounter].decks);
		}
		break;
	case 4:
		for (int i = fCol; i > fCol - fAmountDecks; --i) {
			fField[fRow][i] = 1;
			fShipsField[fCounter].indexRows[n] = fRow;
			fShipsField[fCounter].indexCols[n++] = i;
			++(fShipsField[fCounter].decks);
		}
	}
}

//проверка сгенерированой точки и направления прорисовки
bool CheckPosition(int fField[][SIZE_FIELD], int fRow, int fCol, int fDirection, int fAmountDecks)
{
	switch (fDirection)
	{
	case 1:
		if (fRow - (fAmountDecks - 1) < 1) return false;
		for (int i = fRow + 1; i >= fRow - fAmountDecks; --i) {
			for (int j = fCol - 1; j <= fCol + 1; ++j) {
				if (fField[i][j]) return false;
			}
		}
		break;
	case 2:
		if (fCol + (fAmountDecks - 1) > SIZE_FIELD - 2) return false;
		for (int i = fRow - 1; i <= fRow + 1; ++i) {
			for (int j = fCol - 1; j <= fCol + fAmountDecks; ++j) {
				if (fField[i][j]) return false;
			}
		}
		break;
	case 3:
		if (fRow + (fAmountDecks - 1) > SIZE_FIELD - 2) return false;
		for (int i = fRow - 1; i <= fRow + fAmountDecks; ++i) {
			for (int j = fCol - 1; j <= fCol + 1; ++j) {
				if (fField[i][j]) return false;
			}
		}
		break;
	case 4:
		if (fCol - (fAmountDecks - 1) < 1) return false;
		for (int i = fRow - 1; i <= fRow + 1; ++i) {
			for (int j = fCol + 1; j >= fCol - fAmountDecks; --j) {
				if (fField[i][j]) return false;
			}
		}
	}
	return true;
}

//---------------------------------функция игры---------------------------------------\\

void Game(int fField1[][SIZE_FIELD], int fField2[][SIZE_FIELD], ship* fShipsField1, ship* fShipsField2,
	ship* CopySF1, ship* CopySF2, int fChoice)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	bool flag{ false };
	int AmountShipsField1(INDEX_GAME * 2), AmountShipsField2(INDEX_GAME * 2);
	int tempVal(AmountShipsField2), countShot;
	bool (*ptr_TypeGame[3])(int[][SIZE_FIELD], ship*, ship*, int&) {
		ShotPlayer,
			ShotComputer,
			ShotSmartComputer
	};

	switch (fChoice)
	{
	case 1:
		while (AmountShipsField1 > 0 && AmountShipsField2 > 0) {
			system("cls");
			PrintTitle(fChoice);
			PrintCount(AmountShipsField1, AmountShipsField2);
			Print_USER_COMP_Fields(fField1, fField2);
			countShot = 0;
			cout << "\nИгрок стреляет\n";
			do {
				flag = ptr_TypeGame[0](fField2, fShipsField2, CopySF2, AmountShipsField2);
				system("cls");
				PrintTitle(fChoice);
				PrintCount(AmountShipsField1, AmountShipsField2);
				Print_USER_COMP_Fields(fField1, fField2);
				SetConsoleTextAttribute(hStdOut, 12);
				if (tempVal > AmountShipsField2) {
					cout << "\nПотоплен...Враг уходит на дно...\n";
					--tempVal;
				}
				if (flag) {
					cout << "\nПрекрасное попадание\n";
					++countShot;
				}
				switch (countShot)
				{
				case 1: cout << "Выцеливаем дальше\n"; break;
				case 2: cout << "!!!Double kill!!!\n"; break;
				case 3: cout << "!!!Triple kill!!!\n"; break;
				case 4: cout << "!!!...Просто БУЙСТВО...!!!\n"; break;
				case 5: cout << "!!!...НЕУДЕРЖИМЫЙ КИЛЛЕР...!!!\n";
				}
				SetConsoleTextAttribute(hStdOut, 15);
			} while (flag && AmountShipsField2 > 0);

			cout << "/nКомпьютер стреляет\n";
			do {
				flag = ptr_TypeGame[1](fField1, fShipsField1, CopySF1, AmountShipsField1);
				system("cls");
				PrintTitle(fChoice);
				PrintCount(AmountShipsField1, AmountShipsField2);
				Print_USER_COMP_Fields(fField1, fField2);
			} while (flag && AmountShipsField1 > 0);
		}
		break;
	case 2:
		while (AmountShipsField1 > 0 && AmountShipsField2 > 0) {
			do {
				flag = ptr_TypeGame[1](fField2, fShipsField2, CopySF2, AmountShipsField2);
				system("cls");
				PrintTitle(fChoice);
				PrintCount(AmountShipsField1, AmountShipsField2);
				Print_COMP_COMP_Fields(fField1, fField2);
			} while (flag && AmountShipsField2 > 0);

			do {
				flag = ptr_TypeGame[1](fField1, fShipsField1, CopySF1, AmountShipsField1);
				system("cls");
				PrintTitle(fChoice);
				PrintCount(AmountShipsField1, AmountShipsField2);
				Print_COMP_COMP_Fields(fField1, fField2);
			} while (flag && AmountShipsField1 > 0);
		}
		break;
	case 3:
		while (AmountShipsField1 > 0 && AmountShipsField2 > 0) {
			countShot = 0;
			system("cls");
			PrintTitle(fChoice);
			PrintCount(AmountShipsField1, AmountShipsField2);
			Print_USER_COMP_Fields(fField1, fField2);
			cout << "\nИгрок стреляет\n";
			do {
				flag = ptr_TypeGame[0](fField2, fShipsField2, CopySF2, AmountShipsField2);
				system("cls");
				PrintTitle(fChoice);
				PrintCount(AmountShipsField1, AmountShipsField2);
				Print_USER_COMP_Fields(fField1, fField2);
				SetConsoleTextAttribute(hStdOut, 12);
				if (tempVal > AmountShipsField2) {
					cout << "\nПотоплен...Враг уходит на дно...\n";
					--tempVal;
				}
				if (flag) {
					cout << "\nПрекрасное попадание\n";
					++countShot;
				}
				switch (countShot)
				{
				case 1: cout << "Выцеливаем дальше\n"; break;
				case 2: cout << "!!!Double kill!!!\n"; break;
				case 3: cout << "!!!Triple kill!!!\n"; break;
				case 4: cout << "!!!...Просто БУЙСТВО...!!!\n"; break;
				case 5: cout << "!!!...НЕУДЕРЖИМЫЙ КИЛЛЕР...!!!\n";
				}
				SetConsoleTextAttribute(hStdOut, 15);
			} while (flag && AmountShipsField2 > 0);

			cout << "\nКомпьютер стреляет\n";
			do {
				flag = ptr_TypeGame[2](fField1, fShipsField1, CopySF1, AmountShipsField1);
				system("cls");
				PrintTitle(fChoice);
				PrintCount(AmountShipsField1, AmountShipsField2);
				Print_USER_COMP_Fields(fField1, fField2);
			} while (flag && AmountShipsField1 > 0);
		}
		break;
	case 4:
		while (AmountShipsField1 > 0 && AmountShipsField2 > 0) {
			do {
				flag = ptr_TypeGame[2](fField2, fShipsField2, CopySF2, AmountShipsField2);
				system("cls");
				PrintTitle(fChoice);
				PrintCount(AmountShipsField1, AmountShipsField2);
				Print_COMP_COMP_Fields(fField1, fField2);
			} while (flag && AmountShipsField2 > 0);

			do {
				flag = ptr_TypeGame[2](fField1, fShipsField1, CopySF1, AmountShipsField1);
				system("cls");
				PrintTitle(fChoice);
				PrintCount(AmountShipsField1, AmountShipsField2);
				Print_COMP_COMP_Fields(fField1, fField2);
			} while (flag && AmountShipsField1 > 0);
		}
	}
	if (!AmountShipsField2) cout << "Во втором поле все корабли потоплены. ПОБЕДА\n";
	else cout << "В первом поле все корабли потоплены. ПОБЕДА\n";
}

//счёт в игре
void PrintCount(int fAmountShipsField1, int fAmountShipsField2)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, 10);

	cout.width(INDEX_GAME * 2); cout << "";
	cout.width(3); cout << fAmountShipsField1;
	cout.width(INDEX_GAME * 8); cout << "";
	cout.width(4); cout << fAmountShipsField2;
	cout << "\n";

	SetConsoleTextAttribute(hStdOut, 15);
}

//функция выстрела игрока
bool ShotPlayer(int fField[][SIZE_FIELD], ship* fShipField, ship* fCopySF, int& fAmountShipsField)
{
	int fRow(0), fCol(0);
	char tempCh;
	do {
		if (fField[fRow][fCol] < 0) cout << "Вы сюда уже стреляли...Стреляйте внимательней.";
		do {
			cout << "\nСтрока(от 1 до 10): ";
			cin >> fRow;
			if (cin.fail()) { cin.clear(); while(cin.get() != '\n'); }
		} while (fRow < 1 || fRow > 10);
		do {
			cout << "\nСтолбец(от A до J): ";
			cin >> tempCh;
			while (cin.get() != '\n');
			fCol = int(toupper(tempCh)) - 64;;
		} while (fCol < 1 || fCol > 10);
		
	} while (fField[fRow][fCol] < 0);
	return CheckShot(fField, fShipField, fCopySF, fRow, fCol, fAmountShipsField);
}

//функция выстрела компьютера
bool ShotComputer(int fField[][SIZE_FIELD], ship* fShipField, ship* fCopySF, int& fAmountShipsField)
{
	int fRow(0), fCol(0);

	do {
		fRow = rand() % 10 + 1;
		fCol = rand() % 10 + 1;
	} while (fField[fRow][fCol] < 0);
	return CheckShot(fField, fShipField, fCopySF, fRow, fCol, fAmountShipsField);
}

//функция выстрела умного компьютера
bool ShotSmartComputer(int fField[][SIZE_FIELD], ship* fShipField, ship* fCopySF, int& fAmountShipsField)
{
	bool flag = false;
	int fRow(0), fCol(0), tempRow(0), tempCol(0);
	do {
		if (flag) {
			if (-1 != fField[fRow - 1][fCol] && fRow - 1 > 0) --fRow;
			if (-1 != fField[fRow][fCol + 1] && fCol + 1 < SIZE_FIELD - 1) ++fCol;;
			if (-1 != fField[fRow + 1][fCol] && fRow + 1 < SIZE_FIELD - 1) ++fRow;
			if (-1 != fField[fRow][fCol - 1] && fCol - 1 > 0) --fCol;;
		}
		else {
			do {
				fRow = rand() % 10 + 1;
				fCol = rand() % 10 + 1;
			} while (fField[fRow][fCol] < 0);
		}
		flag = CheckShot(fField, fShipField, fCopySF, fRow, fCol, fAmountShipsField);
	} while (flag);
	return false;
}

//функция проверки результатов выстрела
bool CheckShot(int fField[][SIZE_FIELD], ship* fShipField, ship* fCopySF, int fRow, int fCol,
	int& fAmountShipsField)
{
	bool flag{ false };

	switch (fField[fRow][fCol])
	{
	case  1:
		fField[fRow][fCol] = -2;
		for (int i = 0; i < INDEX_GAME * 2; ++i) {
			for (int j = 0; j < INDEX_GAME - 1; ++j) {
				if (fRow == fShipField[i].indexRows[j] && fCol == fShipField[i].indexCols[j]) {
					fShipField[i].indexRows[j] = -2;
					fShipField[i].indexCols[j] = -2;
					--fShipField[i].decks;
					flag = true;
					break;
				}
			}
			if (flag) {
				if (!fShipField[i].decks) {
					--fAmountShipsField;
					for (int n = 0; n < fCopySF[i].decks; ++n) {
						if (!fField[fCopySF[i].indexRows[n] - 1][fCopySF[i].indexCols[n] - 1])
							fField[fCopySF[i].indexRows[n] - 1][fCopySF[i].indexCols[n] - 1] = -1;
						if (!fField[fCopySF[i].indexRows[n] - 1][fCopySF[i].indexCols[n]])
							fField[fCopySF[i].indexRows[n] - 1][fCopySF[i].indexCols[n]] = -1;
						if (!fField[fCopySF[i].indexRows[n] - 1][fCopySF[i].indexCols[n] + 1])
							fField[fCopySF[i].indexRows[n] - 1][fCopySF[i].indexCols[n] + 1] = -1;
						if (!fField[fCopySF[i].indexRows[n]][fCopySF[i].indexCols[n] + 1])
							fField[fCopySF[i].indexRows[n]][fCopySF[i].indexCols[n] + 1] = -1;
						if (!fField[fCopySF[i].indexRows[n] + 1][fCopySF[i].indexCols[n] + 1])
							fField[fCopySF[i].indexRows[n] + 1][fCopySF[i].indexCols[n] + 1] = -1;
						if (!fField[fCopySF[i].indexRows[n] + 1][fCopySF[i].indexCols[n]])
							fField[fCopySF[i].indexRows[n] + 1][fCopySF[i].indexCols[n]] = -1;
						if (!fField[fCopySF[i].indexRows[n] + 1][fCopySF[i].indexCols[n] - 1])
							fField[fCopySF[i].indexRows[n] + 1][fCopySF[i].indexCols[n] - 1] = -1;
						if (!fField[fCopySF[i].indexRows[n]][fCopySF[i].indexCols[n] - 1])
							fField[fCopySF[i].indexRows[n]][fCopySF[i].indexCols[n] - 1] = -1;
					}
				}
				break;
			}
		}
		break;
	case  0:
		fField[fRow][fCol] = -1;
	}
	return flag;
}