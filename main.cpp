#include <iostream>

struct AVLNode {
    int key;
    int height;
    AVLNode *left;
    AVLNode *right;

    AVLNode(int k) :
            key(k),
            height(1),
            left(nullptr),
            right(nullptr) {};
};

int max(int a, int b) {
    return (a > b) ? a : b;
}

int getHeight(AVLNode *node) {
    if (node == nullptr) return 0;
    return node->height;
}

void UpdateHeight(AVLNode *node) {
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
}

int balance_fact(AVLNode *root) {
    return getHeight(root->left) - getHeight(root->right);
}

AVLNode *rightRotate(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *n3 = x->right;

    x->right = y;
    y->left = n3;

    // Обновление высот
    UpdateHeight(y);
    UpdateHeight(x);

    return x;
}

AVLNode *leftRotate(AVLNode *y) {
    AVLNode *x = y->right;
    AVLNode *n3 = x->left;

    x->left = y;
    y->right = n3;

    // Обновление высот
    UpdateHeight(y);
    UpdateHeight(x);

    return x;
}

AVLNode *Insert(AVLNode *root, int key) {
    // Если корня нет, то мы его создаем со значением передаваемого ключа
    if (root == nullptr) {
        return new AVLNode(key);
    }

    if (key < root->key) {
        root->left = Insert(root->left, key);
    } else if (key > root->key) {
        root->right = Insert(root->right, key);
    } else {
        return root;
    }

    UpdateHeight(root);

    int balance = balance_fact(root);

    if (balance > 1) {
        if (balance_fact(root->left) >= 0) {
            root = rightRotate(root);
        } else {
            root->left = leftRotate(root->left);
            root = rightRotate(root);
        }
    }

    if (balance < -1) {
        if (balance_fact(root->right) <= 0) {
            root = leftRotate(root);
        } else {
            root->right = rightRotate(root->right);
            root = leftRotate(root);
        }
    }


    return root;
}

AVLNode *minValueNode(AVLNode *node) {
    AVLNode *current = node;
    // Проходим влево до последнего узла (узла с наименьшим значением).
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}

AVLNode *Delete(AVLNode *root, int key) {
    // Базовый случай: если корень пуст или ключа нет в дереве.
    if (root == nullptr) {
        return root;
    }

    // Если удаляемый ключ меньше ключа корня, то он находится в левом поддереве.
    if (key < root->key) {
        root->left = Delete(root->left, key);
    }
        // Если удаляемый ключ больше ключа корня, то он находится в правом поддереве.
    else if (key > root->key) {
        root->right = Delete(root->right, key);
    }
        // Если ключ равен ключу корня, тогда этот корень является узлом, который нужно удалить.
    else {
        // Узел с одним ребенком или без детей.
        if ((root->left == nullptr) || (root->right == nullptr)) {
            AVLNode *temp = root->left ? root->left : root->right;

            // Узел без детей.
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else { // Узел с одним ребенком.
                *root = *temp;
            }

            delete temp;
        } else {
            // Узел с двумя детьми: получить узел с наименьшим ключом в правом поддереве.
            AVLNode *temp = minValueNode(root->right);

            // Копировать содержимое узла-преемника в этот узел.
            root->key = temp->key;

            // Удалить узел-преемник.
            root->right = Delete(root->right, temp->key);
        }
    }

    // Если дерево имел только один узел, то вернуть его.
    if (root == nullptr) {
        return root;
    }

    // Обновить высоту текущего узла.
    UpdateHeight(root);

    // Получить фактор баланса, чтобы проверить, стало ли дерево несбалансированным.
    int balance = balance_fact(root);

    // Если этот узел становится несбалансированным, то есть 4 случая.

    // Простое правое вращение.
    if (balance > 1 && balance_fact(root->left) >= 0) {
        return rightRotate(root);
    }

    // Лево-правое вращение.
    if (balance > 1 && balance_fact(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Простое левое вращение.
    if (balance < -1 && balance_fact(root->right) <= 0) {
        return leftRotate(root);
    }

    // Право-левое вращение.
    if (balance < -1 && balance_fact(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

AVLNode *Search(AVLNode *root, int key) {
    // Если корень пуст, или если ключ корня совпадает с искомым ключом.
    if (root == nullptr || root->key == key) {
        return root;
    }
    // Если искомый ключ меньше ключа корня, ищем в левом поддереве.
    if (root->key > key) {
        return Search(root->left, key);
    }
    // Искомый ключ больше ключа корня, ищем в правом поддереве.
    return Search(root->right, key);
}

int main() {
    AVLNode* root = nullptr;

    // Вставляем несколько значений
    root = Insert(root, 30);
    root = Insert(root, 20);
    root = Insert(root, 40);
    root = Insert(root, 50);
    root = Insert(root, 13);

    // Поиск значения в дереве
    AVLNode* searchResult = Search(root, 40);
    if (searchResult != nullptr) {
        std::cout << "Found " << searchResult->key << " in the tree." << std::endl;
    } else {
        std::cout << "Value not found in the tree." << std::endl;
    }

    // Удаляем значение
    root = Delete(root, 40);

    // Пытаемся найти удаленное значение
    searchResult = Search(root, 40);
    if (searchResult != nullptr) {
        std::cout << "Found " << searchResult->key << " in the tree." << std::endl;
    } else {
        std::cout << "Value not found in the tree." << std::endl;
    }

    return 0;
}
