#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <map>
#include <iomanip>

using namespace std;

// ����LR������
map<int, map<char, string>> ACTION;
map<int, map<char, int>> GOTO;

// ״̬ջ�ͷ���ջ
stack<int> stateStack;
stack<char> symbolStack;

// ��ʼ��LR������
void initializeTables() {
    // ACTION���ʼ��
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

    // GOTO���ʼ��
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

// ��ȡ��ǰ����
string getAction(int state, char symbol) {
    if (ACTION[state].find(symbol) != ACTION[state].end()) {
        return ACTION[state][symbol];
    }
    return "error";
}

// ��ȡGOTOֵ
int getGoto(int state, char symbol) {
    if (GOTO[state].find(symbol) != GOTO[state].end()) {
        return GOTO[state][symbol];
    }
    return -1;
}

// ��Լ����
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

// ��ӡ��ǰջ������
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

// LR(1)��������
void parse(string input) {
    stateStack.push(0);
    symbolStack.push('#');
    int step = 1;

    cout << "����\t״̬ջ\t\t\t����ջ\t\t\tʣ�����봮\t\t����" << endl;

    while (true) {
        int state = stateStack.top();
        char symbol = input[0];
        string action = getAction(state, symbol);

        // ��ӡ��ǰ������Ϣ
        cout << setw(4) << step << "\t";
        printStack(stateStack);
        cout << "\t\t\t";
        printSymbolStack(symbolStack);
        cout << "\t\t\t" << input << "\t\t\t";

        if (action[0] == 'S') {
            // �ƽ�����
            int nextState = stoi(action.substr(1));
            stateStack.push(nextState);
            symbolStack.push(symbol);
            input = input.substr(1);
            cout << "�ƽ�";
        }
        else if (action[0] == 'r') {
            // ��Լ����
            int rule = stoi(action.substr(1));
            reduce(rule);
            cout << "��Լ " << action;
        }
        else if (action == "acc") {
            cout << "����";
            break;
        }
        else {
            cout << "����";
            break;
        }

        cout << endl;
        step++;
    }

    cout << endl << "����������" << endl;
}

int main() {
    initializeTables();
    string input;
    cout << "��������#�����ķ��Ŵ�: ";
    cin >> input;
    parse(input);
    return 0;
}
