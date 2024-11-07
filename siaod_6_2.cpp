#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct alphabet
{
	vector <char> used;
	vector <int> index;
};

string firstWord(string Text)	//для нахождения первого слова
{
	string output;
	for (char iter : Text)
	{
		if (iter == ' ' or iter == ',')
			return output;
		output += iter;
	}
}

string print_space(int count)
{
	string spaces = "";
	for (int i = 0; i < count; i++)
		spaces += " ";
	return spaces;
}

int index_from_alphabet(char ex, alphabet alph)
{
	for (int i = 0; i < alph.used.size(); i++)
	{
		if (ex == alph.used[i])
		{
			return alph.index[i];
		}
	}
	return alph.index[alph.index.size() - 1];
}

//БЛОК ДЛЯ СОСТАВЛЕНИЯ ТАБЛИЦЫ ИНДЕКСОВ

int found_count(char example, string ex)
{
	int count = 0;
	for (char iter : ex)
	{
		if (example == iter)
			count++;
	}
	return count;
}

bool found(char example, vector <char> ex)
{
	for (char iter : ex)
	{
		if (example == iter)
			return true;
	}
	return false;
}

alphabet table_index(string Word)
{
	alphabet ex;
	if (found_count(Word[Word.length() - 1], Word) == 1)	//если последняя буква уникальна, то присвоение индекса длины - 1
	{
		ex.used.push_back(Word[Word.length() - 1]);
		ex.index.push_back(Word.length() - 1);
	}
	for (int i = Word.length() - 2; i >= 1; i--)
	{
		if (!found(Word[i], ex.used))
		{
			ex.used.push_back(Word[i]);
			ex.index.push_back(Word.length() - 1 - i);
		}
	}

	ex.used.push_back(Word[0]);		//добавляем первую букву слова
	ex.index.push_back(Word.length() - 1);
	return ex;
}

void print_alphabet(alphabet example)
{
	for (char iter : example.used)
	{
		cout << iter << " ";
	}
	cout << endl;
	for (int iter : example.index)
	{
		cout << iter << " ";
	}
	cout << endl;
}

int find_index(string word, string text)
{
	int index = 0;
	int count = 0;
	bool flag = true;
	alphabet ex = table_index(word);
	while (index + word.length() < text.length())
	{
		
		int offset = 0;
		while (offset < word.length())
		{
			if (text[index + word.length() - 1 - offset] == word[word.length() - 1 - offset])
			{
				offset++;
				if (offset == word.length())
				{
					count++;
					index += word.length();
					cout << print_space(index) << word << "\n";
					break;
				}
			}
			else if (text[index + word.length() - 1 - offset] != word[word.length() - 1 - offset])
			{
				index += index_from_alphabet(text[index + word.length() - 1 - offset],ex);
				cout << print_space(index) << word << "\n";
				break;
			}
		}
	}
	return count;
}


int main()
{
	setlocale(0, "");
	string text = "buffalo, named Buffalo, is from Buffalo, buffalo, other buffaloes from Buffalo.";
	string first_word = firstWord(text);
	cout << text << "\n";
	cout << first_word << "\n";
	cout << find_index(first_word, text);
}