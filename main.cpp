#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define WORD_FILE_NAME "CollinsScrabbleWords.txt"
#define ALPHABET_SIZE 26
#define A_CHARACTER 'A'

using namespace std;

class TrieTree {
    public:
        vector<TrieTree*> children;
        bool endOfWord;
        
        TrieTree() {
            children.resize(ALPHABET_SIZE, NULL);
            endOfWord = false;
        }
        void addNode(int letterNum);
};

void TrieTree::addNode(int letterNum) {
    children[letterNum] = new TrieTree();
}

void buildTree(TrieTree* rootNode){
    ifstream wordFile; 
    wordFile.open(WORD_FILE_NAME);

    TrieTree* currentNode = rootNode;

    if (wordFile.is_open()) {
        char currentChar;

        while (wordFile) {
            currentChar = wordFile.get();

            int letterNumber = currentChar - A_CHARACTER;

            // Remove special Characters
            if (letterNumber == -52){
                currentNode->endOfWord = true;
                currentNode = rootNode;
                continue;
            } else if (letterNumber == -55){
                continue;
            }

            if (currentNode->children[letterNumber] == NULL){
                currentNode->addNode(letterNumber);
            }

            currentNode = currentNode->children[letterNumber];
        }
    }
}

int main(){
    TrieTree* root = new TrieTree();

    buildTree(root);

    delete root;
    return 0;
    
}