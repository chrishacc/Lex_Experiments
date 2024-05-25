#include <iostream>
#include <stack>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// �����ķ�����
enum Symbol {
    E, T, G, S, F,
    PLUS, MINUS, MULT, DIV, LPAREN, RPAREN, ID, END, EPSILON
};

// ���� LL(1) ������
map<pair<Symbol, Symbol>, vector<Symbol>> parseTable;

// ��ʼ�� LL(1) ������
void initializeParseTable() {
    parseTable[{E, ID}] = { T, G };
    parseTable[{E, LPAREN}] = { T, G };
    parseTable[{G, PLUS}] = { PLUS, T, G };
    parseTable[{G, MINUS}] = { MINUS, T, G };
    parseTable[{G, RPAREN}] = { EPSILON };
    parseTable[{G, END}] = { EPSILON };
    parseTable[{T, ID}] = { F, S };
    parseTable[{T, LPAREN}] = { F, S };
    parseTable[{S, PLUS}] = { EPSILON };
    parseTable[{S, MINUS}] = { EPSILON };
    parseTable[{S, MULT}] = { MULT, F, S };
    parseTable[{S, DIV}] = { DIV, F, S };
    parseTable[{S, RPAREN}] = { EPSILON };
    parseTable[{S, END}] = { EPSILON };
    parseTable[{F, ID}] = { ID };
    parseTable[{F, LPAREN}] = { LPAREN, E, RPAREN };
}

// ��ȡ���ŵ�����
string getSymbolName(Symbol symbol) {
    switch (symbol) {
    case E: return "E";
    case T: return "T";
    case G: return "G";
    case S: return "S";
    case F: return "F";
    case PLUS: return "+";
    case MINUS: return "-";
    case MULT: return "*";
    case DIV: return "/";
    case LPAREN: return "(";
    case RPAREN: return ")";
    case ID: return "id";
    case END: return "$";
    case EPSILON: return "��";
    default: return "";
    }
}

// �����ǰ�Ķ�ջ״̬������״̬
void printStackAndInput(int step, stack<Symbol> parseStack, vector<Symbol> input, size_t index, const string& production, const string& action) {
    stack<Symbol> tempStack = parseStack;
    vector<string> stackContent;
    while (!tempStack.empty()) {
        stackContent.push_back(getSymbolName(tempStack.top()));
        tempStack.pop();
    }
    reverse(stackContent.begin(), stackContent.end());

    cout << step << "\t";
    for (const auto& symbol : stackContent) {
        cout << symbol << " ";
    }

    cout << "\t";
    for (size_t i = index; i < input.size(); ++i) {
        cout << getSymbolName(input[i]) << " ";
    }

    cout << "\t" << production << "\t" << action << endl;
}

// ����������Ŵ�
void parse(vector<Symbol> input) {
    stack<Symbol> parseStack;
    parseStack.push(END);
    parseStack.push(E);

    size_t index = 0;
    Symbol currentInput = input[index];
    int step = 1;

    cout << "����\t����ջ\t\tʣ�����봮\t���ò���ʽ\t����\n";
    printStackAndInput(step++, parseStack, input, index, "", "��ʼ��");

    while (!parseStack.empty()) {
        Symbol top = parseStack.top();
        parseStack.pop();

        if (top == currentInput) {
            if (top == END) {
                printStackAndInput(step++, parseStack, input, index, "", "����");
                cout << "Success: Input string is accepted." << endl;
                return;
            }
            currentInput = input[++index];
            printStackAndInput(step++, parseStack, input, index, "", "ƥ��");
        }
        else if (parseTable.count({ top, currentInput }) > 0) {
            vector<Symbol> production = parseTable[{top, currentInput}];
            string prodStr = getSymbolName(top) + " -> ";
            for (const auto& sym : production) {
                prodStr += getSymbolName(sym) + " ";
            }

            if (production[0] != EPSILON) {
                for (auto it = production.rbegin(); it != production.rend(); ++it) {
                    parseStack.push(*it);
                }
            }
            printStackAndInput(step++, parseStack, input, index, prodStr, "�Ƶ�");
        }
        else {
            printStackAndInput(step++, parseStack, input, index, "", "����");
            cout << "Error: Unexpected symbol '" << getSymbolName(currentInput) << "' at position " << index << endl;
            return;
        }
    }
    cout << "Error: Unexpected end of input." << endl;
}

// ���� LL(1) �﷨��������
int main() {
    initializeParseTable();

    // ������Ŵ� (id + id) * id
    vector<Symbol> input = { LPAREN, ID, PLUS, ID, RPAREN, MULT, ID, END };

    parse(input);

    return 0;
}
