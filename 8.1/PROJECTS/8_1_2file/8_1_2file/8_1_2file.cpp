#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <map>
#include <queue>
#include <vector>

using namespace std;

struct Node {
    wchar_t ch;
    int freq;
    Node* left, * right;

    Node(wchar_t c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    Node(wchar_t c, int f, Node* l, Node* r) : ch(c), freq(f), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

Node* buildHuffmanTree(const map<wchar_t, int>& freqMap) {
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (auto pair : freqMap) {
        Node* node = new Node(pair.first, pair.second);
        pq.push(node);
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* parent = new Node(0, left->freq + right->freq, left, right);
        pq.push(parent);
    }

    return pq.top();
}

void generateCodes(Node* root, const wstring& prefix, map<wchar_t, wstring>& codes) {
    if (!root)
        return;

    if (!root->left && !root->right) {
        codes[root->ch] = prefix;
    }

    generateCodes(root->left, prefix + L"0", codes);
    generateCodes(root->right, prefix + L"1", codes);
}

void deleteTree(Node* root) {
    if (!root)
        return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

vector<uint8_t> packBits(const wstring& bitString) {
    vector<uint8_t> bytes;
    uint8_t byte = 0;
    int bitsFilled = 0;

    for (wchar_t bitChar : bitString) {
        byte <<= 1;
        if (bitChar == L'1') {
            byte |= 1;
        }
        bitsFilled++;

        if (bitsFilled == 8) {
            bytes.push_back(byte);
            byte = 0;
            bitsFilled = 0;
        }
    }

    if (bitsFilled > 0) {
        byte <<= (8 - bitsFilled);
        bytes.push_back(byte);
    }

    return bytes;
}

int main() {
    locale::global(locale("ru_RU.UTF-8"));
    locale locale("ru_RU.UTF-8");
    
    wifstream inFile("input.txt");
    inFile.imbue(locale);

    if (!inFile) {
        wcerr << L"Не удалось открыть входной файл.\n";
        return 1;
    }

    wstring text((istreambuf_iterator<wchar_t>(inFile)), istreambuf_iterator<wchar_t>());
    inFile.close();

    map<wchar_t, int> freq;
    for (wchar_t ch : text) {
        freq[ch]++;
    }

    Node* root = buildHuffmanTree(freq);

    map<wchar_t, wstring> codes;
    generateCodes(root, L"", codes);

    wstring encodedText;
    for (wchar_t ch : text) {
        encodedText += codes[ch];
    }

    vector<uint8_t> packedData = packBits(encodedText);

    wofstream outFile("output.huff", ios::binary);
    outFile.imbue(locale);

    if (!outFile) {
        wcerr << L"Не удалось открыть выходной файл.\n";
        return 1;
    }

    outFile << codes.size() << L'\n';

    for (const auto& pair : codes) {
        outFile << static_cast<int>(pair.first) << L' ' << pair.second << L'\n';
    }

    outFile << L"-----\n";

    outFile << encodedText.length() << L'\n';

    ofstream binaryOut("binary.bin", ios::binary);
    if (!binaryOut) {
        wcerr << L"Не удалось открыть бинарный выходной файл.\n";
        return 1;
    }
    
    binaryOut.write(reinterpret_cast<const char*>(packedData.data()), packedData.size());
    binaryOut.close();

    outFile.close();
  
    deleteTree(root);

    return 0;
}