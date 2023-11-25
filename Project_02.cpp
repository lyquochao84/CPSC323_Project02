/*
    Project: CPSC 323 - Project 2
    Group members: Hao Ly, Darren Cruz, Isaac Perez
    Program: Trace input string
    Input:  (1) (id+id)*id$      (2) id*id$      (3) (id*)$   
    Output: Displayed along the stack implementation. 
    Language: C++
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stack>
#include <string>
#include <vector>

using namespace std;

struct Entry
{
    string input;
    string action;
};

class Parser
{
    private:
        vector<vector<Entry>> parsingTable;

    public:
        Parser() 
        {
            // Initialize the parsing table as per the given table
            parsingTable = {
                {{"id", "5"}, {"(", "4"}, {"E", "1"}, {"T", "2"}, {"F", "3"}},
                {{"+", "6"}, {"$", "acc"}},
                {{"+", "R2"}, {"*", "7"}, {")", "R2"}, {"$", "R2"}},
                {{"+", "R4"}, {"*", "R4"}, {")", "R4"}, {"$", "R4"}},
                {{"id", "5"}, {"(", "4"}, {"E", "8"}, {"T", "2"}, {"F", "3"}},
                {{"+", "R6"}, {"*", "R6"}, {")", "R6"}, {"$", "R6"}},
                {{"id", "5"}, {"(", "4"}, {"T", "9"}, {"F", "3"}},
                {{"id", "5"}, {"(", "4"}, {"F", "10"}},
                {{"+", "6"}, {")", "11"}},
                {{"+", "R1"}, {"*", "7"}, {")", "R1"}, {"$", "R1"}},
                {{"+", "R3"}, {"*", "R3"}, {")", "R3"}, {"$", "R3"}},
                {{"+", "R5"}, {"*", "R5"}, {")", "R5"}, {"$", "R5"}}
            };
        }

        string stackToString(const stack<string> &s)
        {
            stack<string> tempStack(s);
            string result;

            while (!tempStack.empty())
            {
                result.insert(0, tempStack.top());
                tempStack.pop();
            }

            return result;
        }

        void parseInput(string inputString)
        {
            stack<string> stack;

            string input = inputString;
            string action;
            int stepCount = 0;
            string currentInput;
            string next;

            stack.push("$");
            stack.push("0");
            string stackString = stackToString(stack);
            int top = stoi(stack.top());

            cout << endl; 
            cout << setw(6) << "Step" << setw(30) << "Stack" << setw(30) << "Input" << setw(30) << "Action" << endl;

            while (!stack.empty())
            {
                Entry entry;
                bool foundEntry = false;
                top = stoi(stack.top());

                // Check to see if the current value of the inputString is "id"
                if (input[0] == 'i')
                {
                    currentInput = input.substr(0, 2);
                    input.erase(0, 2);
                }
                
                else
                {
                    currentInput = input[0];
                    input.erase(0, 1);
                }

                for (const Entry &e : parsingTable[top])
                {
                    if (e.input == currentInput)
                    {
                        entry = e;
                        foundEntry = true;
                    }
                }

                // If the current value of the inputString does not take us to an empty cell in the Parsing table
                if (foundEntry == true)
                {
                    // If the current value of the inputString takes us to Accept
                    if (entry.action == "acc")
                    {
                        for (int i = 0; i < 4; i++)
                        {
                            stack.pop();
                        }

                        action = "Accepted";
                        cout << setw(6) << stepCount << setw(30) << stackString << setw(30) << input << setw(30) << action << endl;

                        cout << "\n\n" << inputString << " is valid" << endl;
                    }

                    // If the current value of the inputString takes us to a Reduce Rule
                    else if (entry.action[0] == 'R')
                    {
                        input.insert(0, currentInput);

                        // If the current value of the inputString takes us to R1 (E->E+T)
                        if (entry.action[1] == '1')
                        {
                            // Remove the "E+T" from stackString
                            for (int i = 0; i < 6; i++)
                            {
                                stack.pop();
                            }

                            top = stoi(stack.top());
                            next = entry.action;
                            action = "Reduce " + currentInput + " --> " + next;

                            cout << setw(6) << stepCount << setw(30) << stackString << setw(30) << input << setw(30) << action << endl;

                            // Push "E" into the stack
                            currentInput = "E";

                            for (const Entry &e : parsingTable[top])
                            {
                                if (e.input == currentInput)
                                {
                                    entry = e;
                                }
                            }
                            next = entry.action;
                            stack.push(currentInput);
                            stack.push(next);
                            stackString = stackToString(stack);
                            stepCount++;
                        }

                        // If the current value of the inputString takes us to R2 (E->T)
                        else if (entry.action[1] == '2')
                        {
                            // Remove the "T" from stackString
                            for (int i = 0; i < 2; i++)
                            {
                                stack.pop();
                            }

                            top = stoi(stack.top());
                            next = entry.action;
                            action = "Reduce " + currentInput + " --> " + next;

                            cout << setw(6) << stepCount << setw(30) << stackString << setw(30) << input << setw(30) << action << endl;

                            // Push "E" into the stack
                            currentInput = "E";

                            for (const Entry &e : parsingTable[top])
                            {
                                if (e.input == currentInput)
                                {
                                    entry = e;
                                }
                            }
                            next = entry.action;
                            stack.push(currentInput);
                            stack.push(next);
                            stackString = stackToString(stack);
                            stepCount++;
                        }

                        // If the current value of the inputString takes us to R3 (T->T*F)
                        else if (entry.action[1] == '3')
                        {
                            // Remove the "T*F" from stackString
                            for (int i = 0; i < 6; i++)
                            {
                                stack.pop();
                            }

                            top = stoi(stack.top());
                            next = entry.action;
                            action = "Reduce " + currentInput + " --> " + next;

                            cout << setw(6) << stepCount << setw(30) << stackString << setw(30) << input << setw(30) << action << endl;

                            // Push "T" into the stack
                            currentInput = "T";

                            for (const Entry &e : parsingTable[top])
                            {
                                if (e.input == currentInput)
                                {
                                    entry = e;
                                }
                            }
                            next = entry.action;
                            stack.push(currentInput);
                            stack.push(next);
                            stackString = stackToString(stack);
                            stepCount++;
                        }

                        // If the current value of the inputString takes us to R4 (T->F)
                        else if (entry.action[1] == '4')
                        {
                            // Remove the "F" from stackString
                            for (int i = 0; i < 2; i++)
                            {
                                stack.pop();
                            }

                            top = stoi(stack.top());
                            next = entry.action;
                            action = "Reduce " + currentInput + " --> " + next;

                            cout << setw(6) << stepCount << setw(30) << stackString << setw(30) << input << setw(30) << action << endl;

                            // Push "T" into the stack
                            currentInput = "T";

                            for (const Entry &e : parsingTable[top])
                            {
                                if (e.input == currentInput)
                                {
                                    entry = e;
                                }
                            }
                            next = entry.action;
                            stack.push(currentInput);
                            stack.push(next);
                            stackString = stackToString(stack);
                            stepCount++;
                        }

                        // If the current value of the inputString takes us to R5 (F->(E))
                        else if (entry.action[1] == '5')
                        {
                            // Remove the "(E)" from stackString
                            for (int i = 0; i < 6; i++)
                            {
                                stack.pop();
                            }

                            top = stoi(stack.top());
                            next = entry.action;
                            action = "Reduce " + currentInput + " --> " + next;

                            cout << setw(6) << stepCount << setw(30) << stackString << setw(30) << input << setw(30) << action << endl;

                            // Push "F" into the stack
                            currentInput = "F";

                            for (const Entry &e : parsingTable[top])
                            {
                                if (e.input == currentInput)
                                {
                                    entry = e;
                                }
                            }
                            next = entry.action;
                            stack.push(currentInput);
                            stack.push(next);
                            stackString = stackToString(stack);
                            stepCount++;
                        }

                        // If the current value of the inputString takes us to R6 (F->id)
                        else
                        {
                            // Remove the "id" from stackString
                            for (int i = 0; i < 2; i++)
                            {
                                stack.pop();
                            }

                            top = stoi(stack.top());
                            next = entry.action;
                            action = "Reduce " + currentInput + " --> " + next;

                            cout << setw(6) << stepCount << setw(30) << stackString << setw(30) << input << setw(30) << action << endl;

                            // Push "F" into the stack
                            currentInput = "F";

                            for (const Entry &e : parsingTable[top])
                            {
                                if (e.input == currentInput)
                                {
                                    entry = e;
                                }
                            }
                            next = entry.action;
                            stack.push(currentInput);
                            stack.push(next);
                            stackString = stackToString(stack);
                            stepCount++;
                        }
                    }

                    // If the current value of the inputString takes us to Shift
                    else
                    {
                        next = entry.action;
                        stack.push(currentInput);
                        stack.push(next);
                        action = "Shift " + currentInput + " --> GoTo S" + next;

                        cout << setw(6) << stepCount << setw(30) << stackString << setw(30) << input << setw(30) << action << endl;
                        stackString = stackToString(stack);
                        stepCount++;
                    }
                }

                // If the current value of the inputString takes us to an empty cell in the Parsing table
                else
                {
                    action = "invalid --> Not Accepted";
                    cout << setw(6) << stepCount << setw(30) << stackString << setw(30) << input << setw(30) << action << endl;

                    cout << "\n\n" << inputString << " is invalid" << endl;
                    break;
                }
            }
        }
};

int main()
{
    Parser parser;
    string inputString;
    cout << "Enter the input string: ";
    cin >> inputString;

    parser.parseInput(inputString);
    return 0;
}
