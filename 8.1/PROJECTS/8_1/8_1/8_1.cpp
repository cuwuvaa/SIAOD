#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct literal
{
	char liter;
	int count = 0;
	string code = "";
	literal(char liter, int count)
	{
		this->liter = liter;
		this->count = count;
	}
};

bool isFound(vector <literal> ex, char toFound)
{
	for (int i = 0; i < ex.size(); i++)
	{
		if (ex[i].liter == toFound)
		{
			return 1;
		}
	}
	return 0;
}

vector <literal> alph(string text)
{
	vector<literal> iter;
	for (int i = 0; i < text.length(); i++)
	{
		if (!isFound(iter, text[i]))	//если новая буква в алфавите
		{
			literal newLit(text[i], 1);
			iter.push_back(newLit);
		}
		else					//если уже есть подобная буква, увеличиваем счетчик ее повторов
		{
			for (int j = 0; j < iter.size(); j++)
			{
				if (iter[j].liter == text[i])
				{
					iter[j].count++;
				}
			}
		}
	}
	return iter;
}

void sortingVector(vector <literal>& alphabet)
{
	int n = alphabet.size();
	for (int i = 0; i < n - 1; i++) {
		// Переменная для оптимизации (отслеживание обменов)
		bool swapped = false;
		// Последние i элементов уже стоят на своих местах
		for (int j = 0; j < n - i - 1; j++) {
			if (alphabet[j].count < alphabet[j + 1].count) {
				// Обмен элементов
				literal temp = alphabet[j];
				alphabet[j] = alphabet[j + 1];
				alphabet[j + 1] = temp;
				swapped = true;
			}
		}
		// Если не было обменов во внутреннем цикле, массив уже отсортирован
		if (!swapped)
			break;
	}
}

int findSplitPoint(int left, int right, const std::vector<literal>& symbols) {
	// Вычисляем общую сумму частот в текущей группе символов
	double total = 0.0;
	for (int i = left; i <= right; ++i) {
		total += symbols[i].count;
	}

	// Цель - найти точку, где накопленная сумма приближается к половине общей суммы
	double halfTotal = total / 2.0;
	double accum = 0.0;
	int split = left;

	for (int i = left; i <= right; ++i) {
		accum += symbols[i].count;
		if (accum >= halfTotal) {
			// Проверяем, было ли предыдущее значение накопленной суммы ближе к половине
			if (i == left) {
				// Если первый же символ превосходит половину, то точка разделения - первый символ
				split = i;
			}
			else {
				double diff1 = std::abs(accum - halfTotal);
				double diff2 = std::abs((accum - symbols[i].count) - halfTotal);
				if (diff2 < diff1) {
					split = i - 1;
				}
				else {
					split = i;
				}
			}
			break;
		}
	}

	return split;
}


void shannonFano(int left, int right, std::vector<literal>& symbols) {
	if (left >= right)
		return;

	// Ищем точку разделения
	int split = findSplitPoint(left, right, symbols);

	// Присваиваем биты '0' и '1'
	for (int i = left; i <= split; ++i)
		symbols[i].code += "0";
	for (int i = split + 1; i <= right; ++i)
		symbols[i].code += "1";

	// Рекурсивные вызовы для подгрупп
	shannonFano(left, split, symbols);
	shannonFano(split + 1, right, symbols);
}

void codeText(string text)
{
	vector<literal>alphabet = alph(text);
	sortingVector(alphabet);
	cout << "alphabet: \n";
	cout << "------------------------------\n";
	for (int i = 0; i < alphabet.size(); i++)
	{
		cout << alphabet[i].liter << ": " << alphabet[i].count << "\n";
	}
	cout << "------------------------------\n";
	cout << "Размер алфавита: " << alphabet.size() << "\n";
	shannonFano(0, alphabet.size() - 1, alphabet);
	int shrinkSize = 0;
	cout << "\n--------Таблица сжатия--------\n";
	for (int i = 0; i < alphabet.size(); i++)
	{
		shrinkSize += alphabet[i].code.length() * alphabet[i].count;
		cout << alphabet[i].liter << ": " << alphabet[i].code << "\n";
	}
	cout << "\nДо сжатия вес в битах: " << text.length() * 8 << "  (в байтах): " << text.length() << "\n";
	cout << "После сжатия вес в битах: " << shrinkSize << " (в байтах): " << shrinkSize / 8 << "\n";
	cout << "\n--------Закодированный текст--------\n";
	for (int i = 0; i < text.length(); i++)
	{
		if (i % 20 == 0 && i != 0)
		{
			cout << "<<\n";
		}
		for (int j = 0; j < alphabet.size(); j++)
		{
			if (text[i] == alphabet[j].liter)
			{
				cout << alphabet[j].code << " ";
			}
		}
	}
}


int main()
{
	setlocale(0, "");
	string text = "Перводан, другодан,На колоде барабан;Свистель, коростель,Пятерка, шестерка, утюг.";
	codeText(text);
}