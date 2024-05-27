#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <map>
#include <iomanip>

using namespace std;

// 定义LR分析表
map<int, map<char, string>> ACTION;
map<int, map<char, int>> GOTO;

// 状态栈和符号栈
stack<int> stateStack;
stack<char> symbolStack;

// 初始化LR分析表
void initializeTables() {
    // ACTION表初始化
    ACTION[0]['i'] = "S5";
    ACTION[0]['('] = "S4";
    ACTION[1]['+'] = "S6";
    ACTION[1]['#'] = "acc";
    ACTION[2]['+'] = "r2";
    ACTION[2]['*'] = "S7";
    ACTION[2][')'] = "r2";
    ACTION[2]['#'] = "r2";
    ACTION[3]['+'] = "r4";
    ACTION[3]['*'] = "r4";
    ACTION[3][')'] = "r4";
    ACTION[3]['#'] = "r4";
    ACTION[4]['i'] = "S5";
    ACTION[4]['('] = "S4";
    ACTION[5]['+'] = "r6";
    ACTION[5]['*'] = "r6";
    ACTION[5][')'] = "r6";
    ACTION[5]['#'] = "r6";
    ACTION[6]['i'] = "S5";
    ACTION[6]['('] = "S4";
    ACTION[7]['i'] = "S5";
    ACTION[7]['('] = "S4";
    ACTION[8]['+'] = "S6";
    ACTION[8][')'] = "S11";
    ACTION[9]['+'] = "r1";
    ACTION[9]['*'] = "S7";
    ACTION[9][')'] = "r1";
    ACTION[9]['#'] = "r1";
    ACTION[10]['+'] = "r3";
    ACTION[10]['*'] = "r3";
    ACTION[10][')'] = "r3";
    ACTION[10]['#'] = "r3";
    ACTION[11]['+'] = "r5";
    ACTION[11]['*'] = "r5";
    ACTION[11][')'] = "r5";
    ACTION[11]['#'] = "r5";

    // GOTO表初始化
    GOTO[0]['E'] = 1;
    GOTO[0]['T'] = 2;
    GOTO[0]['F'] = 3;
    GOTO[4]['E'] = 8;
    GOTO[4]['T'] = 2;
    GOTO[4]['F'] = 3;
    GOTO[6]['T'] = 9;
    GOTO[6]['F'] = 3;
    GOTO[7]['F'] = 10;
}

// 获取当前动作
string getAction(int state, char symbol) {
    if (ACTION[state].find(symbol) != ACTION[state].end()) {
        return ACTION[state][symbol];
    }
    return "error";
}

// 获取GOTO值
int getGoto(int state, char symbol) {
    if (GOTO[state].find(symbol) != GOTO[state].end()) {
        return GOTO[state][symbol];
    }
    return -1;
}

// 归约操作
void reduce(int rule) {
    switch (rule) {
    case 1: // E -> E + T
        for (int i = 0; i < 3; ++i) {
            stateStack.pop();
            symbolStack.pop();
        }
        symbolStack.push('E');
        stateStack.push(getGoto(stateStack.top(), 'E'));
        break;
    case 2: // E -> T
        stateStack.pop();
        symbolStack.pop();
        symbolStack.push('E');
        stateStack.push(getGoto(stateStack.top(), 'E'));
        break;
    case 3: // T -> T * F
        for (int i = 0; i < 3; ++i) {
            stateStack.pop();
            symbolStack.pop();
        }
        symbolStack.push('T');
        stateStack.push(getGoto(stateStack.top(), 'T'));
        break;
    case 4: // T -> F
        stateStack.pop();
        symbolStack.pop();
        symbolStack.push('T');
        stateStack.push(getGoto(stateStack.top(), 'T'));
        break;
    case 5: // F -> ( E )
        for (int i = 0; i < 3; ++i) {
            stateStack.pop();
            symbolStack.pop();
        }
        symbolStack.push('F');
        stateStack.push(getGoto(stateStack.top(), 'F'));
        break;
    case 6: // F -> i
        stateStack.pop();
        symbolStack.pop();
        symbolStack.push('F');
        stateStack.push(getGoto(stateStack.top(), 'F'));
        break;
    }
}

// 打印当前栈的内容
void printStack(stack<int> s) {
    vector<int> v;
    while (!s.empty()) {
        v.push_back(s.top());
        s.pop();
    }
    for (int i = v.size() - 1; i >= 0; --i) {
        cout << v[i] << " ";
    }
}

void printSymbolStack(stack<char> s) {
    vector<char> v;
    while (!s.empty()) {
        v.push_back(s.top());
        s.pop();
    }
    for (int i = v.size() - 1; i >= 0; --i) {
        cout << v[i] << " ";
    }
}

// LR(1)分析函数
void parse(string input) {
    stateStack.push(0);
    symbolStack.push('#');
    int step = 1;

    cout << "步骤\t状态栈\t\t\t符号栈\t\t\t剩余输入串\t\t动作" << endl;

    while (true) {
        int state = stateStack.top();
        char symbol = input[0];
        string action = getAction(state, symbol);

        // 打印当前步骤信息
        cout << setw(4) << step << "\t";
        printStack(stateStack);
        cout << "\t\t\t";
        printSymbolStack(symbolStack);
        cout << "\t\t\t" << input << "\t\t\t";

        if (action[0] == 'S') {
            // 移进操作
            int nextState = stoi(action.substr(1));
            stateStack.push(nextState);
            symbolStack.push(symbol);
            input = input.substr(1);
            cout << "移进";
        }
        else if (action[0] == 'r') {
            // 归约操作
            int rule = stoi(action.substr(1));
            reduce(rule);
            cout << "归约 " << action;
        }
        else if (action == "acc") {
            cout << "接受";
            break;
        }
        else {
            cout << "错误";
            break;
        }

        cout << endl;
        step++;
    }

    cout << endl << "分析结束。" << endl;
}

int main() {
    initializeTables();
    string input;
    cout << "请输入以#结束的符号串: ";
    cin >> input;
    parse(input);
    return 0;
}
