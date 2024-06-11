#include <iostream> 
#include <string> 
#include <stdlib.h>
#include <fstream> 
#include <sstream>
#include <algorithm> 

using namespace std;

struct Node {
    string birthday;
    string surname;
    string city;
    Node* left = nullptr;
    Node* right = nullptr;
};

bool isTreeBuilt = false;
Node* dataList = nullptr;
int dataCount = 0;
Node* minValueNode(Node* node);

Node* newNode(string birthday, string surname, string city) {
    Node* temp = new Node;
    temp->birthday = birthday;
    temp->surname = surname;
    temp->city = city;
    temp->left = temp->right = nullptr;
    return temp;
}

bool isNumber(const string& str) {
    return all_of(str.begin(), str.end(), ::isdigit);
}

bool isAlpha(const string& str) {
    return all_of(str.begin(), str.end(), ::isalpha);
}

Node* insert(Node* node, const string& birthday, const string& surname, const string& city) {
    if (!isNumber(birthday)) {
        cout << "Узел не создан, ввод данных неправильный.\n\n";
        return node;
    }
    if (!isAlpha(surname) || !isAlpha(city)) {
        cout << "Узел не создан, ввод данных неправильный.\n\n";
        return node;
    }
    if (node == nullptr) {

        node = new Node;
        node->birthday = birthday;
        node->surname = surname;
        node->city = city;
        node->left = nullptr;
        node->right = nullptr;
    }
    else {

        if (birthday < node->birthday) {
            node->left = insert(node->left, birthday, surname, city);
        }
        else if (birthday > node->birthday) {
            node->right = insert(node->right, birthday, surname, city);
        }
        else {
            cout << "Данные не сохранились - Дубликат: " << birthday << endl;
            cout << endl;
        }
    }
    return node;
}

void addDataToBST(Node*& root) {
    for (int i = 0; i < dataCount; i++) {
        root = insert(root, dataList[i].birthday, dataList[i].surname, dataList[i].city);
    }
    isTreeBuilt = true;
    cout << "Бинарное дерево поиска построено.\n\n";
}

void printTree(Node* node, string indent = "") {
    if (node != nullptr) {
        printTree(node->left, indent);
        cout << indent << "Дата рождения: " << node->birthday << endl;
        cout << indent << "Фамилия: " << node->surname << endl;
        cout << indent << "Город рождения: " << node->city << endl;
        cout << endl;
        printTree(node->right, indent);
    }
}

void rootLeftRight(Node* root, string indent = "") {
    if (root == nullptr) {
        return;
    }
    cout << indent << "Дата рождения: " << root->birthday << endl;
    cout << indent << "Фамилия: " << root->surname << endl;
    cout << indent << "Город рождения: " << root->city << endl;
    cout << endl;
    rootLeftRight(root->left, indent);
    rootLeftRight(root->right, indent);
}

void rootLeftRightToFile(Node* root, ofstream& file) {
    if (root == nullptr) {
        return;
    }
    file << root->birthday << " " << root->surname << " " << root->city << endl;
    rootLeftRightToFile(root->left, file);
    rootLeftRightToFile(root->right, file);
}

bool deleteNodeByBirthday(Node*& root, string& birthday) {
    if (root == nullptr) {
        return false;
    }
    if (root->birthday == birthday) {

        if (root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            root = temp;
            return true;
        }
        else if (root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            root = temp;
            return true;
        }

        Node* temp = minValueNode(root->right);

        root->birthday = temp->birthday;
        root->surname = temp->surname;
        root->city = temp->city;

        bool isDeleted = deleteNodeByBirthday(root->right, temp->birthday);
        return true;
    }
    else if (birthday < root->birthday) {
        return deleteNodeByBirthday(root->left, birthday);
    }
    else {
        return deleteNodeByBirthday(root->right, birthday);
    }
}

int countNodes(Node* root) {
    if (root == nullptr) {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}

void addDataToGlobalArray(const string& birthday, const string& surname, const string& city) {
    Node* newDataList = new Node[dataCount + 1];
    if (dataList != nullptr) {
        memcpy(newDataList, dataList, dataCount * sizeof(Node));
        delete[] dataList;
    }
    newDataList[dataCount].birthday = birthday;
    newDataList[dataCount].surname = surname;
    newDataList[dataCount].city = city;
    newDataList[dataCount].left = newDataList[dataCount].right = nullptr;
    dataList = newDataList;
    dataCount++;
}

Node* minValueNode(Node* node) {
    Node* current = node;

    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

void deleteAllNodes(Node* root) {
    if (root == nullptr) {
        return;
    }
    deleteAllNodes(root->left);
    deleteAllNodes(root->right);
    delete root;
}

bool changeNode(Node* root, string birthday, string newSurname, string newCity) {
    if (root == nullptr) {
        return false;
    }

    if (root->birthday == birthday) {
        root->surname = newSurname;
        root->city = newCity;
        return true;
    }
    else if (birthday < root->birthday) {
        return changeNode(root->left, birthday, newSurname, newCity);
    }
    else {
        return changeNode(root->right, birthday, newSurname, newCity);
    }
}

void printSortedNodes(Node* root) {
    if (root == nullptr) {
        return;
    }
    printSortedNodes(root->left);
    cout << root->birthday << " " << root->surname << " " << root->city << endl;
    printSortedNodes(root->right);
}

Node* loadDataFromFile(Node* root, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Не удалось открыть файл для чтения: " << filename << endl;
        cout << endl;
        return root;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string birthday, surname, city;
        if (!(iss >> birthday >> surname >> city)) {
            cerr << "Ошибка при разборе строки: " << line << std::endl;
            continue;
        }
        root = insert(root, birthday, surname, city);
    }
    file.close();
    cout << "Данные загружены и дерево построено.\n\n";
    return root;
}

void findCousins(Node* root, const string& surname) {
    if (root == nullptr) {
        return;
    }
    if (root->surname == surname) {
        cout << root->birthday << " " << root->surname << " " << root->city << endl;
    }
    findCousins(root->left, surname);
    findCousins(root->right, surname);
}

void findMaxValueAndCount(Node* root, string& maxValue, int& count) {
    if (root == nullptr) {
        return;
    }
    if (root->birthday > maxValue) {
        maxValue = root->birthday;
        count = 1;
    }
    else if (root->birthday == maxValue) {
        count++;
    }
    findMaxValueAndCount(root->left, maxValue, count);
    findMaxValueAndCount(root->right, maxValue, count);
}

void printMaxValueAndCount(Node* root) {
    string maxValue;
    int count = 0;
    findMaxValueAndCount(root, maxValue, count);
    cout << "Максимальное значение: " << maxValue << endl;
    cout << "Количество узлов с максимальным значением: " << count << endl;
}

int main() {
    system("color F0");
    setlocale(0, "ru");
    Node* root = nullptr;
    string birthday, surname, city;
    string filename;
    ofstream file;
    char choice;
    bool isTreeCreated = false;

    do {
        if (!isTreeCreated) {
            cout << "\t\ta. Создать пустое дерево\n";
            cout << "\t\tq. Выход из программы\n\n";
            cout << "Выберите действие: ";
            cin >> choice;
            if (choice == 'a') {
                root = nullptr;
                cout << "Пустое дерево создано.\n\n";
                isTreeCreated = true;
                continue;
            }
            else if (choice == 'q') {
                cout << "Выход из программы.\n";
                break;
            }
            else {
                cout << "Неверный выбор. Попробуйте снова.\n\n";
                continue;
            }
        }
        else {
            cout << "\t\tb. Добавить узел с данными\n";
            cout << "\t\tc. Построить бинарное дерево поиска\n";
            cout << "\t\td. Вывести данные в виде дерева\n";
            cout << "\t\te. Вывести данные путем обхода дерева (корень-левое-правое)\n";
            cout << "\t\tf. Удаление узла по дате рождения\n";
            cout << "\t\tg. Удаление всех узлов\n";
            cout << "\t\th. Изменение узла с данными\n";
            cout << "\t\ti. Подсчет всех узлов с данными\n";
            cout << "\t\tj. Сохранить данные в текстовый файл\n";
            cout << "\t\tk. Вывод данных дерева в отсортированном виде (по дате)\n";
            cout << "\t\tl. Загрузка данных из файла и построение дерева( .txt)\n";
            cout << "\t\tm. Поиск всех однофамильцев\n";
            cout << "\t\tn. Вывод максимального значения и количества узлов с этим значением\n";
            cout << "\t\tq. Выход из программы\n\n";
            cout << "Выберите действие: ";
            cin >> choice;
            Node* newRoot = nullptr;
            switch (choice) {
            case 'a':
                root = nullptr;
                cout << "Пустое дерево создано.\n";
                break;
            case 'b':
                cout << "Введите дату рождения: ";
                cin >> birthday;
                cout << "Введите фамилию: ";
                cin >> surname;
                cout << "Введите город рождения: ";
                cin >> city;
                cout << "\n";
                newRoot = insert(root, birthday, surname, city);
                if (newRoot != nullptr) {
                    root = newRoot;
                }
                break;
            case 'c':
                if (!root) {
                    cout << "Для начала создайте узел.\n\n";
                }
                else {
                    isTreeBuilt = true;
                    cout << "Дерево построено.\n\n";
                }
                break;
            case 'd':
                if (!root) {
                    cout << "Для начала создайте узел.\n\n";
                }
                else {
                    cout << "Вывод данных в виде дерева:\n\n";
                    printTree(root);
                }
                break;
            case 'e':
                if (!root) {
                    cout << "Для начала создайте узел.\n\n";
                }
                else {
                    cout << "Вывод данных обходом дерева (корень-левое-правое):\n\n";
                    rootLeftRight(root);
                }
                break;
            case 'f':
                if (!root) {
                    cout << "Для начала создайте узел.\n\n";
                }
                else {
                    cout << "Введите дату рождения узла для удаления: ";
                    cin >> birthday;
                    bool isDeleted = deleteNodeByBirthday(root, birthday);
                    if (!isDeleted) {
                        cout << "Узла с указанным днем рождения не найдено.\n\n";
                    }
                    else {
                        cout << "Узел найден и был удалён.\n\n";
                    }
                }
                break;
            case 'g':
                if (!root) {
                    cout << "Для начала создайте узел.\n\n";
                }
                else {
                    deleteAllNodes(root);
                    root = nullptr;
                    cout << "Все узлы удалены.\n\n";
                }
                break;
            case 'h':
                if (!root) {
                    cout << "Для начала создайте узел.\n\n";
                }
                else {
                    cout << "Введите дату рождения узла для изменения: ";
                    cin >> birthday;
                    cout << "Введите новую фамилию: ";
                    cin >> surname;
                    cout << "Введите новый город рождения: ";
                    cin >> city;
                    cout << "\n";
                    bool isNodeChanged = changeNode(root, birthday, surname, city);
                    if (!isNodeChanged) {
                        cout << "Данного узла не существует.\n\n";
                    }
                }
                break;
            case 'i':
                if (!root) {
                    cout << "Узлов не обнаружено. Для начала создайте узел.\n\n";
                }
                else {
                    int nodeCount = countNodes(root);
                    cout << "Количество узлов в дереве: " << nodeCount << "\n" << endl;
                }
                break;
            case 'j':
                if (!root) {
                    cout << "Для начала создайте узел.\n\n";
                }
                else {
                    cout << "Введите имя файла для сохранения данных: ";
                    cin >> filename;
                    file.open(filename);
                    if (file.is_open()) {
                        rootLeftRightToFile(root, file);
                        file.close();
                        cout << "Данные сохранены в файл " << filename << ".\n\n";
                    }
                    else {
                        cerr << "Не удалось открыть файл для записи.\n\n";
                    }
                }
                break;
            case 'k':
                if (!isTreeBuilt) {
                    cout << "Сначала создайте бинарное дерево поиска.\n\n";
                }
                else {
                    cout << "Вывод данных дерева в отсортированном виде по дате:\n\n";
                    printSortedNodes(root);
                    cout << "\n";
                }
                break;
            case 'l':
                cout << "Введите имя файла для загрузки данных: ";
                cin >> filename;
                root = nullptr;
                root = loadDataFromFile(root, filename);
                isTreeCreated = true;
                continue;
                break;
            case 'm':
                if (!isTreeBuilt) {
                    cout << "Сначала создайте бинарное дерево поиска.\n\n";
                }
                else {
                    cout << "Введите фамилию для поиска однофамильцев: ";
                    cin >> surname;
                    cout << "Однофамильцы:\n\n";
                    findCousins(root, surname);
                    cout << "\n";
                }
                break;
            case 'n':
                if (!isTreeBuilt) {
                    cout << "Сначала создайте бинарное дерево поиска.\n\n";
                }
                else {
                    cout << "Максимальное значение и количество узлов с этим значением:\n";
                    printMaxValueAndCount(root);
                    cout << "\n";
                }
                break;
            case 'q':
                cout << "Выход из программы.\n"; break;
            default:
                cout << "Неверный выбор. Попробуйте снова.\n\n"; break;
            }
        }
    } while (choice != 'q');
    return 0;
}