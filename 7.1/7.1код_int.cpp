#include <iostream>
#include <windows.h>
using namespace std;

struct Node
{
    int data;
    Node* left = nullptr;
    Node* right = nullptr;
    int height = 1;
    Node(int data)
    {
        this->data = data;
    }
};

int getHeight(Node* node)
{
    if (node == nullptr)
        return 0;
    return node->height;
}

int getBalanceFactor(Node* node)
{
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

Node* rightRotate(Node* y)
{
    Node* x = y->left;
    Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    // Return new root
    return x;
}

Node* leftRotate(Node* x)
{
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    // Return new root
    return y;
}

Node* insert(Node* node, int key)
{
    // 1. Perform normal BST insertion
    if (node == nullptr)
        return new Node(key);

    if (key < node->data)
        node->left = insert(node->left, key);
    else if (key > node->data)
        node->right = insert(node->right, key);
    else // Duplicate keys not allowed
        return node;

    // 2. Update height of this ancestor node
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // 3. Get the balance factor
    int balance = getBalanceFactor(node);

    // 4. Balance the node if it is unbalanced

    // Left Left Case
    if (balance > 1 && key < node->left->data)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->data)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->data)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->data)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Return the (unchanged) node pointer
    return node;
}

void printTree(Node* node, int space = 0, int height = 10)
{
    if (node == nullptr)
        return;

    space += height;

    // Process right child first
    printTree(node->right, space);

    cout << endl;
    for (int i = height; i < space; i++)
        cout << " ";
    cout << node->data << "\n";

    // Process left child
    printTree(node->left, space);
}

void inOrderTraversal(Node* node)
{
    if (node == nullptr)
        return;
    inOrderTraversal(node->left);
    cout << node->data << " ";
    inOrderTraversal(node->right);
}

void postOrderTraversal(Node* node)
{
    if (node == nullptr)
        return;
    postOrderTraversal(node->left);
    postOrderTraversal(node->right);
    cout << node->data << " ";
}

int pathLength(Node* node, int value)
{
    if (node == nullptr)
        return -1; // Value not found
    if (node->data == value)
        return 0; // Value found at current node
    else if (value < node->data)
    {
        int left = pathLength(node->left, value);
        if (left >= 0)
            return left + 1;
        else
            return -1;
    }
    else // value > node->data
    {
        int right = pathLength(node->right, value);
        if (right >= 0)
            return right + 1;
        else
            return -1;
    }
}

int treeHeight(Node* node)
{
    return getHeight(node);
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Node* parent = nullptr; // Initialize root to nullptr
    char menu; 
    parent = insert(parent, 100);
    parent = insert(parent, 10);
    parent = insert(parent, 50);
    parent = insert(parent, 300);
    parent = insert(parent, 60);
    parent = insert(parent, 5);
    parent = insert(parent, 40);
    int data;
    while (true)
    {
        cout << "1. Add 2. Print 3. In-order traversal 4. Post-order traversal 5. Path length to value 6. Tree height\nCommand: ";
        cin >> menu;
        switch (menu)
        {
        case '1':
            cout << "data? : ";
            cin >> data;
            parent = insert(parent, data); // Assign the returned node to parent
            cout << "Successfully! Here is the tree:\n\n\n";
            printTree(parent);
            cout << "\n\n\n";
            break;
        case '2':
            cout << "Tree (upper - right, lower - left)\n\n\n";
            printTree(parent);
            cout << "\n\n\n";
            break;
        case '3':
            cout << "In-order traversal: ";
            inOrderTraversal(parent);
            cout << "\n\n\n";
            break;
        case '4':
            cout << "Post-order traversal: ";
            postOrderTraversal(parent);
            cout << "\n\n\n";
            break;
        case '5':
            cout << "Value? : ";
            cin >> data;
            {
                int length = pathLength(parent, data);
                if (length >= 0)
                    cout << "Length of path from root to " << data << " is " << length << "\n\n\n";
                else
                    cout << "Value not found in tree.\n\n\n";
            }
            break;
        case '6':
            cout << "Height of the tree is: " << treeHeight(parent) << "\n\n\n";
            break;
        default:
            cout << "Invalid command\n\n\n";
            break;
        }
    }
    return 0;
}