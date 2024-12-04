import matplotlib.pyplot as plt

def visualize_encoded_text(text, codes):
    encoded_text = [codes[char] for char in text]

    fig, ax = plt.subplots(figsize=(10, 2))
    ax.set_axis_off()

    y_pos = 0.3
    for i, (char, code) in enumerate(zip(text, encoded_text)):
        x_pos = i * 1.5
        ax.text(x_pos, y_pos, char, fontsize=7, ha='center', va='bottom')
        ax.text(x_pos, y_pos - 0.2, code, fontsize=7, ha='center', va='top')

    plt.xlim(-1, len(text) * 1.5)
    plt.ylim(-0.5, 1)
    plt.title('Visualization of Encoded Text')
    plt.show()

# Словарь кодов
codes = {
    ' ': '001',
    'В': '11000',
    'И': '11001',
    'Л': '0000',
    'а': '1101',
    'в': '010',
    'е': '1010',
    'и': '100',
    'й': '10111',
    'к': '0001',
    'л': '10110',
    'н': '11110',
    'о': '11111',
    'р': '11100',
    'с': '0111',
    'ч': '0110',
    'ь': '11101'
}

# Входной текст
text = "Лисовский Иван Валерьевич"

# Визуализация текста
visualize_encoded_text(text, codes)
