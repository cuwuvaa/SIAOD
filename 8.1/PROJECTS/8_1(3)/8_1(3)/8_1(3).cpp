#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;
int main() {
    string input = "comconcomconacom";
    unordered_map<string, int> dictionary;
    vector<pair<int, char>> output;
    setlocale(0, "");
    int dictSize = 1; // Начинаем индекс словаря с 1
    size_t i = 0;

    while (i < input.size()) {
        string currentSubstring;
        int index = 0;

        // Находим максимально длинную подстроку, которая уже есть в словаре
        while (i < input.size() && dictionary.find(currentSubstring + input[i]) != dictionary.end()) {
            currentSubstring += input[i];
            index = dictionary[currentSubstring];
            ++i;
        }

        // Выводим считываемую подстроку
        cout << "Считываемая подстрока: \"" << currentSubstring << "\"\n";

        // Если достигли конца строки
        if (i == input.size()) {
            output.emplace_back(index, '\0'); // Используем '\0' для обозначения конца
            // Выводим текущее состояние словаря
            cout << "Текущий словарь:\n";
            for (const auto& entry : dictionary) {
                cout << "  \"" << entry.first << "\": " << entry.second << "\n";
            }
            cout << "----------------------------------------\n";
            break;
        }

        // Добавляем новую подстроку в словарь
        currentSubstring += input[i];
        dictionary[currentSubstring] = dictSize++;
        char nextChar = input[i];
        ++i;

        // Записываем пару (индекс, символ)
        output.emplace_back(index, nextChar);

        // Выводим текущее состояние словаря
        cout << "Текущий словарь:\n";
        for (const auto& entry : dictionary) {
            cout << "  \"" << entry.first << "\": " << entry.second << "\n";
        }
        cout << "----------------------------------------\n";
    }

    // Выводим результаты
    cout << "Сжатые данные (пары индекс-символ):\n";
    for (const auto& pair : output) {
        cout << "(" << pair.first << ", " << (pair.second == '\0' ? "EOF" : string(1, pair.second)) << ")\n";
    }

    return 0;
}