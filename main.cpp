#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <set>

#define WORD_FILE_NAME "CollinsScrabbleWords.txt"
#define ALPHABET_SIZE 26
#define A_CHARACTER 'A'

using namespace std;

// Keep track of valid words.
set<string> validWords;

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
            
            // Create child if not exist
            if (currentNode->children[letterNumber] == NULL){
                currentNode->addNode(letterNumber);
            }

            currentNode = currentNode->children[letterNumber];
        }
    }
}

void Solver(int x, int y, vector<vector<char> > gameBoard, string wordString, TrieTree* node, vector<vector<bool> > visited){
    visited[x][y] = true;

    // If end of word add it to the word string
    if (node->endOfWord){
        validWords.insert(wordString);
    }
        
    stack< vector<int> > neighbors;

    // Look at 8 squares around the element
    for (int i = -1; i <= 1; i ++){
        for (int j = -1; j <= 1; j ++){
            // Current element to skip
            if (i == 0 && j == 0)
                continue;
        
            // Ensure proposed coordinate is on the game board.
            if (x + i >= 0 && x + i < gameBoard.size() 
                  && y + j >= 0 && y + j < gameBoard[0].size()){
                int letterNumber = gameBoard[x+i][y+j] - A_CHARACTER;
                // Add neighbor if the node has children and has not been visited.
                if (node->children[letterNumber] != NULL && visited[x+i][y+j] == false){
                    vector<int> currNeighbor;
                    // Add Coords
                    currNeighbor.push_back(x+i);
                    currNeighbor.push_back(y+j);
                    
                    neighbors.push(currNeighbor);
                }
            }      
        }
    }
    
    // Explore all neighbr paths
    while (!neighbors.empty()){

        vector<int> currNeighbor = neighbors.top();
        neighbors.pop();

        // Revursive solver
        Solver(currNeighbor[0], currNeighbor[1], gameBoard, (wordString + gameBoard[currNeighbor[0]][currNeighbor[1]]),
            (node->children[(gameBoard[currNeighbor[0]][currNeighbor[1]] - A_CHARACTER)]),visited);
    } 

    return;
}

vector<vector<char> > readGameBoard(){
    cout << "Type each row of the gameboard followed by spaces." << endl;
    cout << "When complete, type \"Done!\"." << endl;

    vector<vector<char> > gameBoard;

    // Arbitrary number to avoid infinite loop
    for (int i = 0; i < 20;  i ++){
        string inputLine = "";
        cin >> inputLine;

        if (inputLine.compare("Done!") == 0){
            break;
        }  else  {
            vector<char> inputChars;
            // Turn input to upper case  
            transform(inputLine.begin(), inputLine.end(), inputLine.begin(), ::toupper);
            // Split string  into char vector
            copy(inputLine.begin(), inputLine.end(), std::back_inserter(inputChars));
            gameBoard.push_back(inputChars);
        }
    }

    return gameBoard;
}

// Sort strings by length
bool compareLen(const string& a, const string& b){
    return (a.size() > b.size()); 
}

void printWords(){
    vector<string> validWordsVector;

    set<string>::iterator itr;
   
   // Buffer the output from other text
   cout << "" << endl;
   cout << "" << endl;

    // Iterate over set elements
    for (itr = validWords.begin();
        itr != validWords.end(); itr++)
        {
            validWordsVector.push_back(*itr);
        }

    // Show largest words first
    sort(validWordsVector.begin(), validWordsVector.end(), compareLen);
    
    for (int i = 0; i < validWordsVector.size(); i ++){
        cout << validWordsVector[i] << endl;
    }
}

int main(){
    TrieTree* root = new TrieTree();
    buildTree(root);

    vector<vector<char> > gameBoard = readGameBoard();

    if (gameBoard.size() == 0){
        delete root;
        return 0;
    }

    vector<vector<bool> > visited;
    visited.resize(gameBoard.size(), vector<bool>(gameBoard[0].size()));

    for (int i = 0; i < gameBoard.size(); i ++){
        for (int j = 0; j < gameBoard[0].size(); j ++){
            // Use string to keep track of current letter combinations since they are mutable
            // and the value can be pulled in constant time.
            string wordString = "";
            wordString.append(1, gameBoard[i][j]);
            
            Solver(i, j, gameBoard, wordString, (root->children[(gameBoard[i][j] - A_CHARACTER)]), visited);
        }
    }

    printWords();    

    delete root;
    return 0;
}