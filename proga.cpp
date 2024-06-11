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
        cout << "���� �� ������, ���� ������ ������������.\n\n";
        return node;
    }
    if (!isAlpha(surname) || !isAlpha(city)) {
        cout << "���� �� ������, ���� ������ ������������.\n\n";
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
            cout << "������ �� ����������� - ��������: " << birthday << endl;
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
    cout << "�������� ������ ������ ���������.\n\n";
}

void printTree(Node* node, string indent = "") {
    if (node != nullptr) {
        printTree(node->left, indent);
        cout << indent << "���� ��������: " << node->birthday << endl;
        cout << indent << "�������: " << node->surname << endl;
        cout << indent << "����� ��������: " << node->city << endl;
        cout << endl;
        printTree(node->right, indent);
    }
}

void rootLeftRight(Node* root, string indent = "") {
    if (root == nullptr) {
        return;
    }
    cout << indent << "���� ��������: " << root->birthday << endl;
    cout << indent << "�������: " << root->surname << endl;
    cout << indent << "����� ��������: " << root->city << endl;
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
        cerr << "�� ������� ������� ���� ��� ������: " << filename << endl;
        cout << endl;
        return root;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string birthday, surname, city;
        if (!(iss >> birthday >> surname >> city)) {
            cerr << "������ ��� ������� ������: " << line << std::endl;
            continue;
        }
        root = insert(root, birthday, surname, city);
    }
    file.close();
    cout << "������ ��������� � ������ ���������.\n\n";
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
    cout << "������������ ��������: " << maxValue << endl;
    cout << "���������� ����� � ������������ ���������: " << count << endl;
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
            cout << "\t\ta. ������� ������ ������\n";
            cout << "\t\tq. ����� �� ���������\n\n";
            cout << "�������� ��������: ";
            cin >> choice;
            if (choice == 'a') {
                root = nullptr;
                cout << "������ ������ �������.\n\n";
                isTreeCreated = true;
                continue;
            }
            else if (choice == 'q') {
                cout << "����� �� ���������.\n";
                break;
            }
            else {
                cout << "�������� �����. ���������� �����.\n\n";
                continue;
            }
        }
        else {
            cout << "\t\tb. �������� ���� � �������\n";
            cout << "\t\tc. ��������� �������� ������ ������\n";
            cout << "\t\td. ������� ������ � ���� ������\n";
            cout << "\t\te. ������� ������ ����� ������ ������ (������-�����-������)\n";
            cout << "\t\tf. �������� ���� �� ���� ��������\n";
            cout << "\t\tg. �������� ���� �����\n";
            cout << "\t\th. ��������� ���� � �������\n";
            cout << "\t\ti. ������� ���� ����� � �������\n";
            cout << "\t\tj. ��������� ������ � ��������� ����\n";
            cout << "\t\tk. ����� ������ ������ � ��������������� ���� (�� ����)\n";
            cout << "\t\tl. �������� ������ �� ����� � ���������� ������( .txt)\n";
            cout << "\t\tm. ����� ���� �������������\n";
            cout << "\t\tn. ����� ������������� �������� � ���������� ����� � ���� ���������\n";
            cout << "\t\tq. ����� �� ���������\n\n";
            cout << "�������� ��������: ";
            cin >> choice;
            Node* newRoot = nullptr;
            switch (choice) {
            case 'a':
                root = nullptr;
                cout << "������ ������ �������.\n";
                break;
            case 'b':
                cout << "������� ���� ��������: ";
                cin >> birthday;
                cout << "������� �������: ";
                cin >> surname;
                cout << "������� ����� ��������: ";
                cin >> city;
                cout << "\n";
                newRoot = insert(root, birthday, surname, city);
                if (newRoot != nullptr) {
                    root = newRoot;
                }
                break;
            case 'c':
                if (!root) {
                    cout << "��� ������ �������� ����.\n\n";
                }
                else {
                    isTreeBuilt = true;
                    cout << "������ ���������.\n\n";
                }
                break;
            case 'd':
                if (!root) {
                    cout << "��� ������ �������� ����.\n\n";
                }
                else {
                    cout << "����� ������ � ���� ������:\n\n";
                    printTree(root);
                }
                break;
            case 'e':
                if (!root) {
                    cout << "��� ������ �������� ����.\n\n";
                }
                else {
                    cout << "����� ������ ������� ������ (������-�����-������):\n\n";
                    rootLeftRight(root);
                }
                break;
            case 'f':
                if (!root) {
                    cout << "��� ������ �������� ����.\n\n";
                }
                else {
                    cout << "������� ���� �������� ���� ��� ��������: ";
                    cin >> birthday;
                    bool isDeleted = deleteNodeByBirthday(root, birthday);
                    if (!isDeleted) {
                        cout << "���� � ��������� ���� �������� �� �������.\n\n";
                    }
                    else {
                        cout << "���� ������ � ��� �����.\n\n";
                    }
                }
                break;
            case 'g':
                if (!root) {
                    cout << "��� ������ �������� ����.\n\n";
                }
                else {
                    deleteAllNodes(root);
                    root = nullptr;
                    cout << "��� ���� �������.\n\n";
                }
                break;
            case 'h':
                if (!root) {
                    cout << "��� ������ �������� ����.\n\n";
                }
                else {
                    cout << "������� ���� �������� ���� ��� ���������: ";
                    cin >> birthday;
                    cout << "������� ����� �������: ";
                    cin >> surname;
                    cout << "������� ����� ����� ��������: ";
                    cin >> city;
                    cout << "\n";
                    bool isNodeChanged = changeNode(root, birthday, surname, city);
                    if (!isNodeChanged) {
                        cout << "������� ���� �� ����������.\n\n";
                    }
                }
                break;
            case 'i':
                if (!root) {
                    cout << "����� �� ����������. ��� ������ �������� ����.\n\n";
                }
                else {
                    int nodeCount = countNodes(root);
                    cout << "���������� ����� � ������: " << nodeCount << "\n" << endl;
                }
                break;
            case 'j':
                if (!root) {
                    cout << "��� ������ �������� ����.\n\n";
                }
                else {
                    cout << "������� ��� ����� ��� ���������� ������: ";
                    cin >> filename;
                    file.open(filename);
                    if (file.is_open()) {
                        rootLeftRightToFile(root, file);
                        file.close();
                        cout << "������ ��������� � ���� " << filename << ".\n\n";
                    }
                    else {
                        cerr << "�� ������� ������� ���� ��� ������.\n\n";
                    }
                }
                break;
            case 'k':
                if (!isTreeBuilt) {
                    cout << "������� �������� �������� ������ ������.\n\n";
                }
                else {
                    cout << "����� ������ ������ � ��������������� ���� �� ����:\n\n";
                    printSortedNodes(root);
                    cout << "\n";
                }
                break;
            case 'l':
                cout << "������� ��� ����� ��� �������� ������: ";
                cin >> filename;
                root = nullptr;
                root = loadDataFromFile(root, filename);
                isTreeCreated = true;
                continue;
                break;
            case 'm':
                if (!isTreeBuilt) {
                    cout << "������� �������� �������� ������ ������.\n\n";
                }
                else {
                    cout << "������� ������� ��� ������ �������������: ";
                    cin >> surname;
                    cout << "������������:\n\n";
                    findCousins(root, surname);
                    cout << "\n";
                }
                break;
            case 'n':
                if (!isTreeBuilt) {
                    cout << "������� �������� �������� ������ ������.\n\n";
                }
                else {
                    cout << "������������ �������� � ���������� ����� � ���� ���������:\n";
                    printMaxValueAndCount(root);
                    cout << "\n";
                }
                break;
            case 'q':
                cout << "����� �� ���������.\n"; break;
            default:
                cout << "�������� �����. ���������� �����.\n\n"; break;
            }
        }
    } while (choice != 'q');
    return 0;
}