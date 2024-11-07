#include <iostream>
#include <math.h>
using namespace std;

int hashfunc(string word)
{
	int sum = 0;
	for (int i = 0; i < word.length(); i++ )
	{
		sum += word[i] * pow(11, word.length() - i - 1);
	}
	return sum;
}

int RK_func(string text, string sub)
{
	string iter = "";
	int hsub = hashfunc(sub), htext;
	int start = 0, stop = sub.length();
	while (start != text.length() - sub.length())
	{
		for (int i = start; i < stop; i++)
		{
			iter += text[i];
		}
		htext = hashfunc(iter);
		cout << iter << "  " << htext << "\n";
		if (hsub == htext)
		{
			if (iter == sub)
			{
				return start;
			}
		}
		start++;
		stop++;
		iter = "";
	}
	return -1;
	
}
int main()
{
	setlocale(0, "");
	string sub;
	string text = "There is something incredibly special about the arrival of summer. The air becomes warmer, \n and the days grow longer, filling us with a sense of energy and vitality. \nAs the world awakens from its winter slumber, we can’t help but feel a sense of\n excitement and possibility. One of the best things about summer is the opportunity\n it provides to enjoy the great outdoors.Whether it’s swimming, playing sports, or\n simply soaking up the sun at the beach, summer is the perfect time to get outside and enjoy \nthe beauty of nature. In addition to outdoor activities, summer is also a time for relaxation and\n leisure.It is a time to kick back and enjoy the simple pleasures of life, whether it’s reading a good \nbook, spending time with friends and family, or traveling to \nnew and exciting places.";
	cout << text << "\n";
	cout << "Введите образец для нахождения: ";
	getline(cin,sub);
	if (RK_func(text, sub) == -1)
	{
		cout << "Строка не найдена";
	}
	else
	{
		cout << "Строка найдена. Индекс первой буквы: " << RK_func(text, sub);
	}
}
