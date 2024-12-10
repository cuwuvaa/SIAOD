#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>

void readRussianTxtFile(const std::string& filePath) {
    std::wifstream file(filePath);
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));

    if (!file.is_open()) {
        std::wcerr << L"Не удалось открыть файл." << std::endl;
        return;
    }

    std::wstring line;
    while (std::getline(file, line)) {
        std::wcout << line << std::endl;
    }

    file.close();
}

int main() {
    // Настройка локали для вывода в консоль
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());

    std::string filePath = "C:/Users/ivank/Desktop/СИАОД/8.1/text.txt";
    readRussianTxtFile(filePath);
    return 0;
}