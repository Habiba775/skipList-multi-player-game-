#include <iostream>
#include <cstdlib> // For rand()
#include <stack>   // For std::stack
#include <limits>  // For INT_MIN
#include <string>  // For handling user input

using namespace std;

struct Node {
    int value;
    Node* right;
    Node* down;

    Node(int val) : value(val), right(nullptr), down(nullptr) {}
};

struct SkipList {
    Node* head;
    int levelCount;

    SkipList() {
        head = new Node(INT_MIN);
        levelCount = 1;
    }
};

// Search function
Node* search(SkipList& skipList, int target) {
    Node* currentNode = skipList.head;

    while (currentNode) {
        while (currentNode->right && currentNode->right->value < target) {
            currentNode = currentNode->right;
        }

        if (currentNode->right && currentNode->right->value == target) {
            return currentNode->right;
        }

        currentNode = currentNode->down;
    }

    return nullptr;
}

void insertAfter(Node* insertionPoint, Node* newNode) {
    newNode->right = insertionPoint->right;
    insertionPoint->right = newNode;
}
// Insert function
void insert(SkipList& skipList, int value) {
    Node* currentNode = skipList.head;
    stack<Node*> insertionPoints;
    while (currentNode) {
        while (currentNode->right && currentNode->right->value < value) {
            currentNode = currentNode->right;
        }
        insertionPoints.push(currentNode);
        currentNode = currentNode->down;
    }
    Node* lowestLevelNode = insertionPoints.top();
    insertionPoints.pop();
    Node* newNode = new Node(value);
    insertAfter(lowestLevelNode, newNode);
    int level = 1;
    while ((rand() % 2) == 0) { // 50% chance to promote
        if (level >= skipList.levelCount) {
            Node* newLevelHead = new Node(INT_MIN);
            newLevelHead->down = skipList.head;
            skipList.head = newLevelHead;
            skipList.levelCount++;
        }

        if (insertionPoints.empty()) {
            break;
        }

        Node* insertionPoint = insertionPoints.top();
        insertionPoints.pop();

        Node* upperLevelNode = new Node(value);
        insertAfter(insertionPoint, upperLevelNode);
        upperLevelNode->down = newNode;
        newNode = upperLevelNode;
        level++;
    }
}

// Delete function
bool deleteNode(SkipList& skipList, int target) {
    Node* currentNode = skipList.head;
    bool found = false;

    while (currentNode) {
        while (currentNode->right && currentNode->right->value < target) {
            currentNode = currentNode->right;
        }

        if (currentNode->right && currentNode->right->value == target) {
            found = true;
            Node* nodeToDelete = currentNode->right;
            currentNode->right = nodeToDelete->right;
            delete nodeToDelete;
        }

        currentNode = currentNode->down;
    }

    return found;
}

void printSkipList(SkipList& skipList) {
    Node* level = skipList.head;
    while (level) {
        Node* node = level->right;
        while (node) {
            cout << node->value << " ";
            node = node->right;
        }
        cout << "\n";
        level = level->down;
    }
    cout << endl;
}

int main() {
    SkipList skipList;

    int choice, value;
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Insert\n";
        cout << "2. Search\n";
        cout << "3. Delete\n";
        cout << "4. Print Skip List\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter value to insert: ";
                cin >> value;
                insert(skipList, value);
                break;

            case 2:
                cout << "Enter value to search: ";
                cin >> value;
                if (search(skipList, value)) {
                    cout << "Found " << value << " in the skip list.\n";
                } else {
                    cout << "Value " << value << " not found.\n";
                }
                break;

            case 3:
                cout << "Enter value to delete: ";
                cin >> value;
                if (deleteNode(skipList, value)) {
                    cout << "Deleted " << value << " from the skip list.\n";
                } else {
                    cout << "Value " << value << " not found to delete.\n";
                }
                break;

            case 4:
                cout << "Skip List: \n";
                printSkipList(skipList);
                break;

            case 5:
                cout << "Exiting...\n";
                return 0;

            default:
                cout << "Invalid choice, please try again.\n";
        }
    }

    return 0;
}


