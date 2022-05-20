#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>  // SetConsoleOutputCP, SetConsoleCP
#include <stdio.h>    //printf , fgets
#include <conio.h>    // _getch
#include <string.h>   // strcpy, strncmp, strchr
//---------------------------------------------------------------------------
struct TInfo {
	int Val;
};
struct TElem {  // моделирование стека на основе односвязного списка
	TInfo Info;   //  или struct TInfo Info;
	TElem* Next;  // или struct TElem *Next; здесь и далее
};
//-----------------------прототипы-------------------------------------------
TElem* PushStack(TElem* St, TInfo Info); // добавить элемент в стек
TInfo PopStack(TElem** PSt);  //извлечь элемент из стека
// дополнительная процедура перекладывания элемента из одного стека (StTop) в другой(Dop)
void TopToTop(TElem** PSt1, TElem** PSt2); // без изменения адресов элементов, только связи
//----------первая часть: создание стека из текстового файла
TElem* CreateStack(TElem* St, bool TypeFile);
//----------вторая часть: вывод стека на экран ------
void OutputStack(TElem*& St);
//----------третья часть: решение задачи -------------
TElem* Decide(TElem** PSt1, TElem* St2);
//----------четвертая часть: освобождение памяти -----
TElem* FreeStack(TElem* St);
//---------------------------------------------------------------------------
int main()
{
	TElem* StackTop1 = NULL, * StackTop2 = NULL;
	char ch;
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	do {

		printf("\nN - создать новый стек; V - вывод; D - решение; F - освободить; E - конец.\nВаш выбор?");
		ch = _getch();   fflush(stdin);
		ch = toupper(ch);
		switch (ch) {
			//----------первая часть: создание стека из текстового
		case 'N':
			if (StackTop1) {
				printf("Error: сначала надо освободить память!"); break;
			}
			printf("\nДля ввода из нестандартного текстового файла нажмите 1, иначе 2"); // добавили ввод из нестандартного текстового файла
			ch = _getch();
			printf("\n");
			switch (ch) {
			case '1': // ввод из нестандартного
				StackTop1 = CreateStack(StackTop1, false);
				break;
			case '2':
				StackTop1 = CreateStack(StackTop1, true);
				break;
			}
			
				//----------вторая часть: вывод стеков на экран ------
		case 'V': printf("\nСтек:\n"); OutputStack(StackTop1);
			break;
			//----------третья часть: решение задачи -------------
		case 'D': StackTop2 = Decide(&StackTop1, StackTop2);
			break;
			//----------четвертая часть: освобождение памяти -----
		case 'F': StackTop1 = FreeStack(StackTop1);
			StackTop2 = FreeStack(StackTop2);
			printf("Вся память под стеки особождена\n");
			break;
			//-----------------------выход------------------------
		case 'E': return 0;
		default:
			printf("Нет такой команды\nPress any key\n");
			_getch();
		}

	} while (ch != 'E');

	return 0;
}
//---------------------------------------------------------------------------
TElem* PushStack(TElem* St, TInfo Info) { // добавить элемент в стек
	TElem* Elem = new TElem;
	Elem->Info.Val = Info.Val;
	// strcpy(Elem->Info.FamIO, Info.FamIO);
	Elem->Next = St;
	return Elem; // Адрес новой вершины
}
TInfo PopStack(TElem** PSt) { //извлечь элемент из стека
	TElem* Elem = *PSt;
	TInfo Info = Elem->Info;
	*PSt = Elem->Next;
	delete Elem;
	return Info;
}
// дополнительная процедура перекладывания элемента из одного стека (StTop) в другой(Dop)
void TopToTop(TElem** PSt1, TElem** PSt2) {
	TElem* Elem, * StTop = *PSt1, * Dop = *PSt2;
	Elem = StTop;
	StTop = StTop->Next;  // или StTop=(*StTop).Next;
	Elem->Next = Dop;
	Dop = Elem;
	*PSt1 = StTop; *PSt2 = Dop;  // сохранить новые адреса вершин по адресам PSt1 и PSt2
	return;
}
//----------первая часть: создание стека из текстового
TElem* CreateStack(TElem* St, bool TypeFile) { // добавили маркер который true если файл стандартный и false если нестандартный
	int kol = 0;
	// char* n, * r;
	TInfo Info;
	char Vals[5];

	while (TypeFile) {               // здесь ввод с консоли (как в примере)
		// условие окончания ввода - два первых символа **
		printf("Значение (или **) =?");
		fgets(Vals, 4, stdin); fflush(stdin);   // считываем входные данные как строку
		if (strncmp(Vals, "**", 2) == 0) break; // если там ** то заканчиваем ввод
		Info.Val = atoi(Vals);                  // если там число а не ** то строку превращаем в int

		St = PushStack(St, Info);               // и ставим наверх в стек
		kol++;
	}
	
	if (!TypeFile) {
		FILE* ft = fopen("input.txt", "rt"); // открываем файл
		if (ft == NULL) { // проверяем что файл не пустой
			printf("Error: не удалось открыть файл с исходными данными %s\n");
			printf("Press any key "); _getch();
			return St;
		}
		while (1) {               // здесь ввод из файла
			// условие окончания ввода - два первых символа **
			fgets(Vals, 4, ft); // fflush(ft);
			if (strncmp(Vals, "**", 2) == 0) break;        // вылили воду из чайника свели задачу к предыдущей
			Info.Val = atoi(Vals);

			St = PushStack(St, Info);
			kol++;
		}
	}

	printf("Создан стек из %d элементов\n", kol);
	printf("Press any key to continue");
	_getch();
	return St;
}
//----------вторая часть: вывод стека на экран ------
void OutputStack(TElem*& St) {
	TElem* Dop = NULL;
	TInfo inf;
	if (!St) printf("пустой\n");
	while (St) {
		inf = PopStack(&St);
		printf("%30d\n", inf.Val);
		Dop = PushStack(Dop, inf);
	}
	while (Dop) TopToTop(&Dop, &St);

	printf("Press any key to continue");
	_getch();
	return;
}
//----------третья часть: решение задачи -------------
TElem* Decide(TElem** PSt1, TElem* St2) {
	TElem * St1 = *PSt1, * Dop = NULL;
	St2 = FreeStack(St2);

	int max = 0, N = 0; // MAX может быть меньше нуля
	while (St1) { // проход по стеку (снимаем сверху элемент и обрабатываем)
		if (max < (St1->Info.Val)) // находим максимум
			max = St1->Info.Val;
		N++; // счетчик
		
		TopToTop(&St1, &Dop); // переместил верхний элемент с главного стека в дополнительный
	}
	while (Dop) { // обработка дополнительного стека
		if (Dop->Info.Val != max) // если текущий верхний элемент дополнительного стека не равен max
			TopToTop(&Dop, &St1); // то возвращаем его в основной стек
		else
			PopStack(&Dop); // иначе удаляем
	};

	if (N) {
		if (St1)
			printf("\nУдалены все элементы равные максимальному\n");
		else
			printf("\nВсе элементы в стеке имеют максимальное значение\n");
	} else
		printf("Стек пуст\n");

	printf("Press any key to continue");
	_getch();
	*PSt1 = St1; // новый адрес через параметр (изменение по адресу)
	return St2;  // новый адрес через результат функции вернется
}
//----------четвертая часть: освобождение памяти -----
TElem* FreeStack(TElem* St) {
	TInfo Info;
	while (St) {
		Info = PopStack(&St);
	}
	return St;
}
//---------------------------------------------------------------------------
