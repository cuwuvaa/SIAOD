from graphviz import Digraph

def create_manual_huffman_tree(codes):
    graph = Digraph(format='png')
    graph.attr('node', shape='circle')

    # Набор для хранения созданных узлов
    created_nodes = {}

    root_id = "root"
    graph.node(root_id, "пусто")
    created_nodes[root_id] = "пусто"

    for char, code in codes.items():
        current_id = root_id
        for i, bit in enumerate(code):
            next_id = current_id + bit

            # Проверка, создан ли узел
            if next_id not in created_nodes:
                label = "пусто" if i < len(code) - 1 else f"{char}\n{code}"
                graph.node(next_id, label)
                created_nodes[next_id] = label
                # Связываем узел с его родителем
                graph.edge(current_id, next_id, label=bit)

            current_id = next_id

    return graph

codes = {
    'Пробел': '001',
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

graph = create_manual_huffman_tree(codes)
graph.render('manual_huffman_tree', view=True)
