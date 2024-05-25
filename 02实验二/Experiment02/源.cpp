#include <iostream>
#include <stack>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// 定义文法规则
enum Symbol {
    E, T, G, S, F,
    PLUS, MINUS, MULT, DIV, LPAREN, RPAREN, ID, END, EPSILON
};

// 定义 LL(1) 分析表
map<pair<Symbol, Symbol>, vector<Symbol>> parseTable;

// 初始化 LL(1) 分析表
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

// 获取符号的名称
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
    case EPSILON: return "ε";
    default: return "";
    }
}

// 输出当前的堆栈状态和输入状态
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

// 解析输入符号串
void parse(vector<Symbol> input) {
    stack<Symbol> parseStack;
    parseStack.push(END);
    parseStack.push(E);

    size_t index = 0;
    Symbol currentInput = input[index];
    int step = 1;

    cout << "步骤\t分析栈\t\t剩余输入串\t所用产生式\t动作\n";
    printStackAndInput(step++, parseStack, input, index, "", "初始化");

    while (!parseStack.empty()) {
        Symbol top = parseStack.top();
        parseStack.pop();

        if (top == currentInput) {
            if (top == END) {
                printStackAndInput(step++, parseStack, input, index, "", "接受");
                cout << "Success: Input string is accepted." << endl;
                return;
            }
            currentInput = input[++index];
            printStackAndInput(step++, parseStack, input, index, "", "匹配");
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
            printStackAndInput(step++, parseStack, input, index, prodStr, "推导");
        }
        else {
            printStackAndInput(step++, parseStack, input, index, "", "错误");
            cout << "Error: Unexpected symbol '" << getSymbolName(currentInput) << "' at position " << index << endl;
            return;
        }
    }
    cout << "Error: Unexpected end of input." << endl;
}

// 测试 LL(1) 语法分析程序
int main() {
    initializeParseTable();

    // 输入符号串 (id + id) * id
    vector<Symbol> input = { LPAREN, ID, PLUS, ID, RPAREN, MULT, ID, END };

    parse(input);

    return 0;
}
