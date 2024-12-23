﻿#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <locale>      // Для установки локали
#include <codecvt>     // Для конвертации строк
#include <queue>
#include <fstream>
#include <iomanip>     // Для форматированного вывода
#include <cmath>       // Для функций ceil и log2
#include <sstream>     // Для stringstream
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

using namespace std;

const int LEN = 25;

// Структура для узла дерева Хаффмана
struct HuffmanNode {
    wchar_t ch; // Символ
    int freq;   // Частота
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(wchar_t character, int frequency) {
        ch = character;
        freq = frequency;
        left = right = nullptr;
    }
};

// Компаратор для приоритетной очереди
struct Compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        // Более высокий приоритет имеет узел с меньшей частотой
        return l->freq > r->freq;
    }
};

// Функция для присвоения кодов символам путем обхода дерева Хаффмана
void assignCodes(HuffmanNode* root, wstring str, map<wchar_t, wstring>& huffmanCode) {
    if (!root)
        return;

    // Листовой узел
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    assignCodes(root->left, str + L"0", huffmanCode);
    assignCodes(root->right, str + L"1", huffmanCode);
}

// Функция для вывода дерева Хаффмана (для визуализации)
void printHuffmanTree(HuffmanNode* root, int indent = 0) {
    if (root != nullptr) {
        printHuffmanTree(root->right, indent + 4);
        if (indent) {
            wcout << std::setw(indent) << ' ';
        }
        if (root->ch != L'\0') {
            wcout << root->ch << L"(" << root->freq << "," << float(root->freq)/float(LEN) << L")\n";
        }
        else {
            wcout << L"*" << L"(" << root->freq << L")\n";
        }
        printHuffmanTree(root->left, indent + 4);
    }
}

// Функция для выполнения кодирования Хаффмана на заданном тексте
void HuffmanCoding(wstring text) {
    // Шаг 1: Построение таблицы частот
    map<wchar_t, int> freq;
    for (wchar_t ch : text) {
        freq[ch]++;
    }

    // Шаг 2: Сортировка алфавита в порядке убывания частот
    // Преобразуем map в vector для сортировки
    vector<pair<wchar_t, int>> freq_vector(freq.begin(), freq.end());
    sort(freq_vector.begin(), freq_vector.end(), [](const pair<wchar_t, int>& a, const pair<wchar_t, int>& b) {
        return a.second > b.second; // Порядок убывания
        });

    // Шаг 3: Построение дерева кодирования Хаффмана
    // Создаем приоритетную очередь для хранения узлов
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

    // Создаем листовые узлы для каждого символа и добавляем в очередь
    for (auto pair : freq) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // Строим дерево Хаффмана
    while (pq.size() != 1) {
        // Удаляем два узла с наивысшим приоритетом (наименьшей частотой)
        HuffmanNode* left = pq.top();
        pq.pop();

        HuffmanNode* right = pq.top();
        pq.pop();

        // Создаем новый внутренний узел с этими двумя узлами в качестве потомков
        // и частотой, равной сумме частот
        HuffmanNode* node = new HuffmanNode(L'\0', left->freq + right->freq);
        node->left = left;
        node->right = right;

        // Добавляем новый узел в очередь
        pq.push(node);
    }

    // Оставшийся узел является корнем дерева
    HuffmanNode* root = pq.top();

    // Шаги 5 и 6: Присвоение кодов символам
    map<wchar_t, wstring> huffmanCode;
    assignCodes(root, L"", huffmanCode);

    // Вывод результатов
    int total_chars = text.length();

    // Шаг 1: вывод таблицы частот и вероятностей
    wcout << L"Символ\tЧастота\tВероятность\n";
    for (auto pair : freq_vector) {
        wchar_t ch = pair.first;
        int frequency = pair.second;
        wcout << ch << L"\t" << frequency << L"\t" << fixed << setprecision(4) << (double)frequency / total_chars << endl;
    }

    // Шаг 2: Отсортированный алфавит (уже выведен выше)

    // Шаг 3: Вывод дерева Хаффмана
    wcout << L"\nДерево Хаффмана:\n";
    printHuffmanTree(root);

    // Шаги 5 и 6: вывод кодов символов
    wcout << L"\nКоды символов:\n";
    for (auto pair : huffmanCode) {
        wcout << pair.first << L"\t" << pair.second << endl;
    }

    // Шаг 7: Кодирование исходной строки
    wstring encodedString = L"";
    for (wchar_t ch : text) {
        encodedString += huffmanCode[ch];
    }

    wcout << L"\nЗакодированная строка:\n" << encodedString << endl;

    // Шаг 8: Расчет коэффициентов сжатия
    // Исходный размер в битах (предполагая кодировку ASCII или UTF-16)
    int originalBits = total_chars * sizeof(wchar_t) * 8; // Размер в битах исходной строки

    // Размер сжатой строки в битах
    int compressedBits = encodedString.length(); // Т.к. каждый символ '0' или '1' занимает 1 бит

    // Длина равномерного кода (log2(N)), где N - количество уникальных символов
    int uniqueSymbols = freq.size();
    int uniformCodeLength = ceil(log2(uniqueSymbols));
    int uniformBits = total_chars * uniformCodeLength;

    // Коэффициенты сжатия
    double compressionRatioOriginal = (double)originalBits / compressedBits;
    double compressionRatioUniform = (double)uniformBits / compressedBits;

    wcout << L"\nКоэффициент сжатия относительно исходной кодировки: " << originalBits << " / " << compressedBits << " = " << compressionRatioOriginal << endl;
    wcout << L"Коэффициент сжатия относительно равномерного кода: " << uniformBits << " / " << compressedBits << " = " << compressionRatioUniform << endl;

    // Шаг 9: Расчет средней длины кода и дисперсии
    double averageCodeLength = 0.0;
    for (auto pair : freq) {
        wchar_t ch = pair.first;
        int frequency = pair.second;
        double probability = (double)frequency / total_chars;
        averageCodeLength += probability * huffmanCode[ch].length();
    }

    double variance = 0.0;
    for (auto pair : freq) {
        wchar_t ch = pair.first;
        int frequency = pair.second;
        double probability = (double)frequency / total_chars;
        double length = huffmanCode[ch].length();
        variance += probability * pow(length - averageCodeLength, 2);
    }

    wcout << L"\nСредняя длина кода: " << averageCodeLength << endl;
    wcout << L"Дисперсия: " << variance << endl;
}

// Функция для сжатия текстового файла с помощью кодирования Хаффмана
void CompressFile(const string& inputFileName, const string& outputFileName) {
    // Открываем входной файл
    wifstream inFile(inputFileName);
    inFile.imbue(locale(""));

    // Проверяем, открыт ли файл
    if (!inFile.is_open()) {
        wcerr << L"Не удалось открыть файл для чтения: " << inputFileName.c_str() << endl;
        return;
    }

    // Читаем содержимое файла
    wstring content((istreambuf_iterator<wchar_t>(inFile)), istreambuf_iterator<wchar_t>());
    inFile.close();

    if (content.empty()) {
        wcerr << L"Файл пуст или содержит недопустимые символы: " << inputFileName.c_str() << endl;
        return;
    }

    // Построение таблицы частот
    map<wchar_t, int> freq;
    for (wchar_t ch : content) {
        freq[ch]++;
    }

    // Построение дерева Хаффмана
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;
    for (auto pair : freq) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }
    while (pq.size() != 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        HuffmanNode* node = new HuffmanNode(L'\0', left->freq + right->freq);
        node->left = left;
        node->right = right;
        pq.push(node);
    }
    HuffmanNode* root = pq.top();

    // Присвоение кодов
    map<wchar_t, wstring> huffmanCode;
    assignCodes(root, L"", huffmanCode);

    // Кодирование содержимого
    wstring encodedContent = L"";
    for (wchar_t ch : content) {
        encodedContent += huffmanCode[ch];
    }

    // Запись закодированного содержимого в выходной файл
    ofstream outFile(outputFileName, ios::binary);
    if (!outFile.is_open()) {
        wcerr << L"Не удалось открыть файл для записи: " << outputFileName.c_str() << endl;
        return;
    }

    // Записываем биты в файл
    // Преобразуем последовательность '0' и '1' в байты
    vector<unsigned char> bytes;
    unsigned char byte = 0;
    int bit_count = 0;
    for (wchar_t bit_char : encodedContent) {
        byte = byte << 1;
        if (bit_char == L'1') {
            byte |= 1;
        }
        bit_count++;
        if (bit_count == 8) {
            bytes.push_back(byte);
            byte = 0;
            bit_count = 0;
        }
    }

    // Добавляем остаточные биты
    if (bit_count > 0) {
        byte <<= (8 - bit_count);
        bytes.push_back(byte);
    }

    // Записываем байты в файл
    outFile.write(reinterpret_cast<const char*>(&bytes[0]), bytes.size());
    outFile.close();

    // Для сравнения выводим размеры файлов
    ifstream inFileSizeStream(inputFileName, ifstream::ate | ifstream::binary);
    ifstream outFileSizeStream(outputFileName, ifstream::ate | ifstream::binary);
    streamsize inFileSize = inFileSizeStream.tellg();
    streamsize outFileSize = outFileSizeStream.tellg();

    double compressionRatio = (double)inFileSize / outFileSize;
    cout << "\nРазмер исходного файла: " << inFileSize << " байт" << endl;
    cout << "Размер сжатого файла: " << outFileSize << " байт" << endl;
    cout << "Коэффициент сжатия: " << compressionRatio << endl;

    // Здесь вы можете использовать стандартный архиватор для сжатия того же файла и сравнить результаты
}

int main() {
    // Устанавливаем локаль для поддержки русских символов
    setlocale(LC_ALL, "");

#ifdef _WIN32
    // Для корректного отображения в консоли Windows
    // Устанавливаем кодировку вывода на UTF-16
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
#endif

    // Запрашиваем у пользователя ввод Фамилии Имени Отчества
    wcout << L"Введите вашу Фамилию Имя Отчество: ";
    wstring inputText;
    getline(wcin, inputText);

    if (inputText.empty()) {
        wcerr << L"Ввод не должен быть пустым!" << endl;
        return 1;
    }

    // Выполняем кодирование Хаффмана на введенном тексте
    HuffmanCoding(inputText);

    // Шаг 10: Сжимаем текстовый файл
    // Для демонстрации сжимаем "input.txt" и выводим в "output.huff"
    // Убедитесь, что файл "input.txt" существует и содержит текст
    string inputFileName = "input.txt";
    string outputFileName = "output.huff";

    CompressFile(inputFileName, outputFileName);

    return 0;
}