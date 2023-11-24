/*
    Project: CPSC 323 - Project 2
    Group members: Hao Ly, Darren Cruz, Isaac Perez
    Program: Trace input string
    Input:  (1) (id+id)*id$      (2) id*id$      (3) (id*)$   
    Output: Displayed along the stack implementation. 
    Language: C++
*/
#include <iostream>
#include <fstream>
#include <stack>
#include <vector>

using namespace std;

struct Action {
    char type; // 'S' for shift, 'R' for reduce, 'A' for accept
    int state_or_production;
};

struct Entry {
    char input;
    Action action;
};

class Parser {
private:
    vector<vector<Entry>> parsingTable;

public:
    Parser() {
        // Initialize the parsing table as per the given table
        parsingTable = {
            {{'i', {'S', 5}}, {'(', {'S', 4}}, {'E', {'S', 1}}, {'T', {'S', 2}}, {'F', {'S', 3}}},
            {{'+', {'S', 6}}, {'$', {'A', 0}}},
            {{'+', {'R', 2}}, {'*', {'S', 7}}, {')', {'R', 2}}, {'$', {'R', 2}}},
            {{'+', {'R', 4}}, {'*', {'R', 4}}, {')', {'R', 4}}, {'$', {'R', 4}}},
            {{'i', {'S', 5}}, {'(', {'S', 4}}, {'E', {'G', 8}}, {'T', {'S', 2}}, {'F', {'S', 3}}},
            {{'+', {'R', 6}}, {'*', {'R', 6}}, {')', {'R', 6}}, {'$', {'R', 6}}},
            {{'i', {'S', 5}}, {'(', {'S', 4}}, {'T', {'G', 9}}, {'F', {'S', 3}}},
            {{'i', {'S', 5}}, {'(', {'S', 4}}, {'F', {'G', 10}}},
            {{'+', {'S', 6}}, {')', {'S', 11}}},
            {{'+', {'R', 1}}, {'*', {'S', 7}}, {')', {'R', 1}}, {'$', {'R', 1}}},
            {{'+', {'R', 3}}, {'*', {'R', 3}}, {')', {'R', 3}}, {'$', {'R', 3}}},
            {{'+', {'R', 5}}, {'*', {'R', 5}}, {')', {'R', 5}}, {'$', {'R', 5}}}
        };
    }

    void parseInput(const string& input) {
        stack<int> stateStack;
        stack<char> symbolStack;
        ofstream outputFile("output.txt");

        stateStack.push(0);
        symbolStack.push('$');

        int step = 1;

        outputFile << "Input: " << input << "\n\n";
        outputFile << "Stack Output:\n";
        outputFile << "Step\tStack\tInput\tAction\n";

        for (size_t i = 0; i < input.size(); ++i) {
            char currentSymbol = input[i];
            int currentState = stateStack.top();
            char topSymbol = symbolStack.top();

            Entry entry;
            bool foundEntry = false;

            for (const Entry& e : parsingTable[currentState]) {
                if (e.input == currentSymbol) {
                    entry = e;
                    foundEntry = true;
                    break;
                }
            }

            if (!foundEntry) {
                outputFile << step << "\tError\n";
                break;
            }

            stack<int> tempStack = stateStack;
            while (!tempStack.empty()) {
                outputFile << tempStack.top() << " ";
                tempStack.pop();
            }
            outputFile << "\t" << input.substr(i) << "\t";

            if (entry.action.type == 'S') {
                stateStack.push(entry.action.state_or_production);
                symbolStack.push(currentSymbol);
                outputFile << "Shift " << entry.action.state_or_production;
                ++i; // Move to the next input symbol
            } else if (entry.action.type == 'R') {
                int production = entry.action.state_or_production;
                int popCount = 2 * production - 1;

                for (int j = 0; j < popCount; ++j) {
                    stateStack.pop();
                    symbolStack.pop();
                }

                char nonTerminal = 'E' + production - 1;
                currentState = stateStack.top();
                int nextState = parsingTable[currentState][nonTerminal - 'E'].action.state_or_production;

                stateStack.push(nextState);
                symbolStack.push(nonTerminal);

                outputFile << "Reduce " << production;
            } else if (entry.action.type == 'A') {
                outputFile << "Accept";
                break;
            } else if (entry.action.type == 'G') {
                stateStack.push(entry.action.state_or_production);
                symbolStack.push(currentSymbol);
                outputFile << "GoTo " << entry.action.state_or_production;
            }

            outputFile << "\n";
            ++step;
        }

        outputFile << "\nOutput: ";
        if (stateStack.top() == 0 && symbolStack.top() == 'E') {
            outputFile << "String is accepted.\n";
        } else {
            outputFile << "String is not accepted.\n";
        }

        outputFile.close();
    }
};

int main() {
    Parser parser;
    parser.parseInput("(id+id)*id$");
    return 0;
}
