import sys
import math
import heapq
from collections import defaultdict

# Установка корректной кодировки
import locale
locale.setlocale(locale.LC_ALL, '')

# Функция для подсчета частот символов в тексте
def calculate_frequencies(text):
    freq = defaultdict(int)
    for ch in text:
        freq[ch] += 1
    return freq

# Класс для узла дерева Хаффмана
class HuffmanNode:
    def __init__(self, freq, ch=None, left=None, right=None):
        self.freq = freq      # Частота
        self.ch = ch          # Символ
        self.left = left      # Левый потомок
        self.right = right    # Правый потомок

    # Метод для сравнения узлов (необходимо для приоритетной очереди)
    def __lt__(self, other):
        return self.freq < other.freq

# Построение дерева Хаффмана
def build_huffman_tree(freq):
    heap = []
    for ch, frequency in freq.items():
        node = HuffmanNode(frequency, ch)
        heapq.heappush(heap, node)

    while len(heap) > 1:
        # Извлекаем два узла с наименьшей частотой
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)

        # Создаем новый внутренний узел с суммарной частотой
        merged = HuffmanNode(left.freq + right.freq, left=left, right=right)
        heapq.heappush(heap, merged)

    return heap[0] if heap else None

# Функция для присвоения кодов символам путем обхода дерева
def assign_codes(node, code='', codes={}):
    if node is None:
        return

    # Листовой узел
    if node.ch is not None:
        codes[node.ch] = code
        return

    assign_codes(node.left, code + '0', codes)
    assign_codes(node.right, code + '1', codes)

# Функция для вывода дерева Хаффмана (опционально)
def print_huffman_tree(node, indent=0):
    if node is not None:
        print_huffman_tree(node.right, indent + 4)
        print(' ' * indent + f"{node.ch if node.ch is not None else '*'}({node.freq})")
        print_huffman_tree(node.left, indent + 4)

# Главная функция для выполнения всех шагов
def huffman_coding(text):
    total_chars = len(text)
    freq = calculate_frequencies(text)

    # Шаг 1: Вывод таблицы частот и вероятностей
    print("Шаг 1: Таблица частот и вероятностей появления символов:")
    print("Символ\tЧастота\tВероятность")
    sorted_freq = sorted(freq.items(), key=lambda item: item[1], reverse=True)
    for ch, frequency in sorted_freq:
        probability = frequency / total_chars
        print(f"{repr(ch)}\t{frequency}\t{probability:.4f}")
    print()

    # Шаг 2: Отсортированный алфавит (уже выведен выше)
    print("Шаг 2: Отсортированный алфавит в порядке убывания частот (см. выше).\n")

    # Шаг 3: Построение дерева Хаффмана
    huffman_tree = build_huffman_tree(freq)

    # Шаг 4: Вывод дерева Хаффмана (опционально)
    print("Шаг 3 и 4: Дерево Хаффмана:")
    print_huffman_tree(huffman_tree)
    print()

    # Шаг 5 и 6: Присвоение кодов символам
    huffman_codes = {}
    assign_codes(huffman_tree, '', huffman_codes)
    print("Шаг 5 и 6: Коды символов:")
    print("Символ\tКод\tДлина кода")
    for ch in sorted(huffman_codes.keys(), key=lambda ch: freq[ch], reverse=True):
        print(f"{repr(ch)}\t{huffman_codes[ch]}\t{len(huffman_codes[ch])}")
    print()

    # Шаг 7: Кодирование исходной строки
    encoded_string = ''.join(huffman_codes[ch] for ch in text)
    print(f"Шаг 7: Закодированная строка:\n{encoded_string}\n")

    # Шаг 8: Расчет коэффициентов сжатия
    print("Шаг 8: Расчет коэффициентов сжатия:")
    # Размер исходной строки в битах (предполагая кодировку UTF-16, 16 бит на символ)
    original_bits = total_chars * 16
    print(f"Исходный размер данных:\nS = N * b = {total_chars} * 16 = {original_bits} бит")
    # Размер равномерного кода
    unique_symbols = len(freq)
    uniform_code_length = math.ceil(math.log2(unique_symbols))
    uniform_bits = total_chars * uniform_code_length
    print(f"Размер равномерного кода:\nL = ceil(log2(M)) = ceil(log2({unique_symbols})) = {uniform_code_length} бит на символ")
    print(f"Общий размер при равномерном коде:\nS_uniform = N * L = {total_chars} * {uniform_code_length} = {uniform_bits} бит")
    # Размер сжатой строки в битах
    compressed_bits = len(encoded_string)
    print(f"Размер сжатых данных (код Хаффмана):\nS_compressed = {compressed_bits} бит")
    # Коэффициенты сжатия
    compression_ratio_original = original_bits / compressed_bits
    compression_ratio_uniform = uniform_bits / compressed_bits
    print(f"Коэффициент сжатия относительно исходной кодировки:\nK1 = S / S_compressed = {original_bits} / {compressed_bits} = {compression_ratio_original:.4f}")
    print(f"Коэффициент сжатия относительно равномерного кода:\nK2 = S_uniform / S_compressed = {uniform_bits} / {compressed_bits} = {compression_ratio_uniform:.4f}\n")

    # Шаг 9: Расчет средней длины кода и дисперсии
    print("Шаг 9: Расчет средней длины кода и дисперсии:")
    print("Средняя длина кода рассчитывается по формуле:")
    print("L_avg = Σ (p_i * l_i)")
    average_code_length = 0
    print("\nВычисления:")
    for ch in freq:
        p_i = freq[ch] / total_chars
        l_i = len(huffman_codes[ch])
        product = p_i * l_i
        average_code_length += product
        print(f"Символ: {repr(ch)}, p_i = {freq[ch]} / {total_chars} = {p_i:.4f}, l_i = {l_i}, p_i * l_i = {p_i:.4f} * {l_i} = {product:.4f}")

    print(f"\nСредняя длина кода:\nL_avg = {average_code_length:.4f} бит на символ\n")

    print("Дисперсия длины кода рассчитывается по формуле:")
    print("D = Σ (p_i * (l_i - L_avg)^2)")
    variance = 0
    print("\nВычисления:")
    for ch in freq:
        p_i = freq[ch] / total_chars
        l_i = len(huffman_codes[ch])
        diff = l_i - average_code_length
        squared_diff = diff ** 2
        product = p_i * squared_diff
        variance += product
        print(f"Символ: {repr(ch)}, l_i = {l_i}, (l_i - L_avg) = {diff:.4f}, (l_i - L_avg)^2 = {squared_diff:.4f}, p_i * (l_i - L_avg)^2 = {p_i:.4f} * {squared_diff:.4f} = {product:.4f}")

    print(f"\nДисперсия длины кода:\nD = {variance:.4f} бит^2\n")

def main():
    # Ввод текста от пользователя
    text = input("Введите текст для кодирования: ")

    if not text:
        print("Введите непустой текст.")
        sys.exit(1)

    huffman_coding(text)

if __name__ == '__main__':
    main()
