/*
 * @Author: cos
 * @Date: 2022-04-18 00:46:57
 * @LastEditTime: 2022-04-18 20:40:38
 * @LastEditors: cos
 * @Description: ����ԭ��ʵ���� �沨�����ʽ���㼰��ֵ
 * @FilePath: \CompileTheory\experiment_3\demo\main.cpp
 */
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <string>
#include <fstream>
using namespace std;
const int Error = -1;
const string ExpFileName = "./exp.txt";
const string existChars = "+-*/()0123456789";
const string op = "+-*/";
map <string, int> opMap = {
    {"+", 1},
    {"-", 1},
    {"*", 2},
    {"/", 2},
};
// ���ļ�
vector<string> readFile(string fileName) {
    vector<string> res;
    try {
        ifstream fin;
        fin.open(fileName);
        string temp;
        while (getline(fin, temp))
            res.push_back(temp);
        return res;
    }
    catch (const exception& e) {
        cerr << e.what() << '\n';
        return res;
    }
}

bool isValid(string exp) {
    if (exp.find_first_not_of(existChars) != string::npos) {
        cout << "Error:����ı��ʽ���Ϸ���ֻ�ܰ���+��*/()���֣�!" << endl;
        return false;
    }
    stack<char> s;  // ()???
    for (auto i : exp) {
        if (i == '(')
            s.push(i);
        else if (i == ')') {
            if (s.empty()) {
                cout << "Error: ) without (!\n";
                return false;
            }
            s.pop();
        }
    }
    return true;
}
class ReversePolishExp {
private:
    int len;
    string exp;
    vector<string> postfixExp;
    stack<string> s;
public:
    ReversePolishExp(string exp) {
        this->exp = exp;
        this->len = exp.length();
        this->postfixExp = getPostfixExp(exp);
    }
    // �ж��Ƿ�Ϊ����
    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }
    // �ж��Ƿ�Ϊ+-*/
    bool isOperate(char c) {
        return op.find(c) != string::npos;
    }
    // ���ַ���ת��Ϊ����
    int s2i(string s) {
        int res = 0;
        for (auto i : s)
            res = res * 10 + (i - '0');
        return res;
    }
    // ����һ��������������
    string readNum(string exp, int& idx) {
        string res = "";
        while (idx < len && isDigit(exp[idx])) {
            res += exp[idx];
            idx++;
        }
        --idx;
        return res;
    }
    // ������׺�����׺���ʽ
    vector<string> getPostfixExp(string exp) {
        vector<string> res;
        for (int i = 0; i < len; ++i) {
            char nowChar = exp[i];
            if (isDigit(nowChar)) {
                res.push_back(readNum(exp, i));
            }
            else if (isOperate(nowChar)) {
                string temp = string(1, nowChar);
                if (opMap[temp] == 1) {  // + -
                    while (!s.empty() && s.top() != "(") {
                        res.push_back(s.top());
                        s.pop();
                    }
                }
                else {    // * /
                    while (!s.empty() && (s.top() == "*" || s.top() == "/")) {
                        res.push_back(s.top());
                        s.pop();
                    }
                }
                s.push(temp);
            }
            else if (nowChar == '(') {
                s.push("(");
            }
            else if (nowChar == ')') {
                while (!s.empty() && s.top() != "(") {
                    res.push_back(s.top());
                    s.pop();
                }
                s.pop();
            }
        }
        while (!s.empty()) {
            res.push_back(s.top());
            s.pop();
        }
        return res;
    }
    // �����׺���ʽ��ֵ
    double compute() {
        stack<double> s2;
        for (auto item : postfixExp) {
            if (isDigit(item[0])) {  // �����ֵĻ� ֱ����ջ
                s2.push(s2i(item) * 1.0);
            }
            else {
                double b = s2.top();
                s2.pop();
                double a = s2.top();
                s2.pop();
                switch (item[0]) {
                case '+': s2.push(a + b); break;
                case '-': s2.push(a - b); break;
                case '*': s2.push(a * b); break;
                case '/':
                    if (b == 0) {
                        cout << "Error: denominator is 0!" << endl;
                        return Error;
                    }
                    s2.push(a / b);
                    break;
                }
            }
        }
        return s2.top();
    }
    void printPostfixExp() {
        if (this->postfixExp.empty()) {
            cout << "postfixExp is empty" << endl;
            return;
        }
        cout << "������沨�����ʽΪ: ";
        for (auto i : this->postfixExp)
            cout << i << " ";
        cout << endl;
    }
    void printExp() {
        if (this->exp.empty()) {
            cout << "exp is empty" << endl;
            return;
        }
        cout << "�������׺���ʽΪ: " << this->exp << endl;
    }
};
int main() {
    cout << "��ȡ���ļ���Ϊ��" << ExpFileName << endl;
    vector<string> exps = readFile(ExpFileName);
    int len = exps.size();
    for (int i = 0; i < len; i++) {
        string exp = exps[i];
        cout << "\n------------------����" << i + 1 << ": " << exp << "--------------------" << endl;
        if (!isValid(exp)) continue;
        cout << "��" << i + 1 << "������ı��ʽ�Ϸ�����ʼ����" << endl;
        ReversePolishExp res(exp);
        res.printExp();
        res.printPostfixExp();
        cout << "���沨�����ʽ�ļ���ֵΪ��" << res.compute() << endl;
    }
    return 0;
}

