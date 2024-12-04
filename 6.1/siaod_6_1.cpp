#include <iostream>
#include <vector>
#include <Windows.h>
using namespace std;

typedef long long int KEY;

struct item
{
	KEY isbn;
	string name;
	string author;
};

struct Hash_Table
{
	int length;
	int count;
	vector<item>* chains; // Массив векторов для цепочек
};

int hash_func(KEY key_, Hash_Table hash_table)
{
	return key_ % hash_table.length;
}

Hash_Table rehash_table(Hash_Table& hash_table)
{
	Hash_Table new_table;		//новая хеш-таблица
	new_table.chains = new vector<item>[hash_table.length*2];	//выделяем место для цепей в два раза больше
	new_table.count = hash_table.count;			//переносим данные с прошлой таблицы
	new_table.length = hash_table.length * 2;
	for (int i = 0; i < hash_table.length; i++)
	{
		for (auto& it : hash_table.chains[i])
		{
			int index = hash_func(it.isbn, new_table);	//пересчитываем индекс каждого элемента относительно новой длины
			new_table.chains[index].push_back(it);	
		}
	}
	
	delete[] hash_table.chains;		//освобождение озу
	return new_table;
}

void insertInHash(item example, Hash_Table& hash_table)
{
	int index = hash_func(example.isbn, hash_table);
	hash_table.chains[index].push_back(example);
	hash_table.count++;
	if (hash_table.count != 0)		//если таблица заполнена на 75% или больше, увеличиваем её
	{
		if ((float)(hash_table.count) / (float)(hash_table.length) >= 0.75)
		{
			hash_table = rehash_table(hash_table);
		}
	}

}

void printTable(Hash_Table& hash_table)
{
	for (int i = 0; i < hash_table.length; i++)
	{
		cout << "-------------------------------------------------------------------------\n";
		cout << "#" << i << ": ";
		for (auto& it : hash_table.chains[i])
		{
			cout << "|isbn: " << it.isbn << "|author: " << it.author << "|name: " << it.name << " ";
		}
		cout << endl;
		cout << "-------------------------------------------------------------------------\n";
	}
}

void find_data(Hash_Table& hash_table, KEY to_find)
{
	int index = hash_func(to_find, hash_table);		//применяем хеш функцию для нахождения индекса
	for (auto& it : hash_table.chains[index])	//перебор по цепочке индекса index
	{
		if (it.isbn == to_find)		//если нашли элемент, то выводим его данные
		{
			cout << "isbn: " << it.isbn << "   author: " << it.author << "    name: " << it.name << "\n";
			break;
		}
	}
	
	cout << "not found\n";
}

void delete_data(Hash_Table& hash_table, KEY to_delete)
{
	int index = hash_func(to_delete, hash_table);		//применяем хеш функцию для нахождения индекса
	for (auto it = hash_table.chains[index].begin(); it != hash_table.chains[index].end(); ++it)
	{										//перебор по цепочке индекса index
		if (it->isbn == to_delete)
		{
			hash_table.chains[index].erase(it);		//если нашли элемент, то стираем его и уменьшаем кол-во
			hash_table.count--;
			cout << "deleted\n";
			return;
		}
	}
	cout << "deleting failed\n";
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Hash_Table table;
	item ex;	//инициализированная структура, нужна для пользовательнской операции 
	item ex1, ex2, ex3, ex4, ex5, ex6, ex7;
	ex1.author = "А.С.Пушкин";	ex1.isbn = 499558691032;	ex1.name = "Капитанская дочка";			
	ex2.author = "М.Ю.Лермонтов"; ex2.isbn = 103583986392;	ex2.name = "Герой нашего времени";
	ex3.author = "Л.Н.Толстой"; ex3.isbn = 960303553069;	ex3.name = "Война и Мир";
	ex4.author = "С.Я.Маршак"; ex4.isbn = 869327484683;		ex4.name = "Кошкин дом";
	ex5.author = "А.А.Ахматова"; ex5.isbn = 683929395816;   ex5.name = "Реквием";			//готовые записи для операции auto
	ex6.author = "И.С.Тургенев"; ex6.isbn = 384885939945;   ex6.name = "Отцы и дети";
	table.length = 10;		//изначальные параметры для массива: вместимость 10, количество элементов 0
	table.count = 0;
	table.chains = new vector<item>[table.length]; // Выделяем память под массив векторов
	while (true)
	{
		cout << "0. auto\n1. add\n2. find\n3. print\n4. delete\n";
		int n;
		cin >> n;
		switch(n)
		{
		case 0:
			insertInHash(ex1, table);
			insertInHash(ex2, table);	//выполняется вставка в таблицу
			insertInHash(ex3, table);
			insertInHash(ex4, table);
			insertInHash(ex5, table);
			insertInHash(ex6, table);
			cout << "ready\n";
			break;
		case 1:
			cout << "isbn: ";
			cin >> ex.isbn;
			cout << "author: ";
			cin >> ex.author;
			cout << "name: ";
			cin >> ex.name;
			insertInHash(ex, table);	
			break;
		case 2:
			KEY key_to_find;
			cout << "isbn: ";		//создание ключа, по которому будет идти поиск
			cin >> key_to_find;
			find_data(table, key_to_find);
			break;
		case 3:
			printTable(table);		//вывод таблицы
			break;
		case 4:
			KEY key_to_delete;		//срздание ключа, по которому будет идти удаление
			cout << "isbn: ";
			cin >> key_to_delete;
			delete_data(table, key_to_delete);
			break;
		}
	}

	delete[] table.chains; //очищаем память
	return 0;
}
