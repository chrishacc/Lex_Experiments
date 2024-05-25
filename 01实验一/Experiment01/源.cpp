#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <string>
#include <unordered_map>

using namespace std;

enum TokenType {
    KEYWORD = 1,
    SEPARATOR,
    OPERATOR,
    REL_OPERATOR,
    NUMBER,
    IDENTIFIER,
    ERROR = 100
};

struct Token {
    TokenType type;
    int attribute;
    string value;
    int line;
    int column;
};

unordered_map<string, int> keywords = {
    {"if", 22}, {"then", 23}, {"else", 24}, {"return", 25}, {"int", 21}, {"main", 20}
};

unordered_map<char, int> separators = {
    {'(', 1}, {')', 2}, {'{', 3}, {'}', 4}, {';', 5}, {',', 6}
};

unordered_map<string, int> operators = {
    {"+", 7}, {"-", 8}, {"*", 9}, {"/", 10}
};

unordered_map<string, int> rel_operators = {
    {"<", 11}, {"<=", 12}, {"=", 13}, {">", 14}, {">=", 15}, {"<>", 16}
};

vector<Token> tokens;
int line = 1, column = 0;

void addToken(TokenType type, int attr, const string& value) {
    tokens.push_back({ type, attr, value, line, column });
}

char getchar(ifstream& in) {
    char c;
    if (in.get(c)) {
        if (c == '\n') {
            line++;
            column = 0;
        }
        else {
            column++;
        }
        return c;
    }
    return EOF;
}

void lexicalAnalysis(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Cannot open file!" << endl;
        return;
    }

    char ch;
    while ((ch = getchar(infile)) != EOF) {
        if (isspace(ch)) continue;
        string word;
        if (isalpha(ch)) {
            word += ch;
            while (isalnum((ch = getchar(infile))) || ch == '_') {
                word += ch;
            }
            infile.unget();

            if (keywords.find(word) != keywords.end()) {
                addToken(KEYWORD, keywords[word], word);
            }
            else {
                addToken(IDENTIFIER, 0, word);
            }
        }
        else if (isdigit(ch)) {
            word += ch;
            while (isdigit((ch = getchar(infile)))) {
                word += ch;
            }
            infile.unget();
            addToken(NUMBER, 30, word);
        }
        else if (separators.find(ch) != separators.end()) {
            addToken(SEPARATOR, separators[ch], string(1, ch));
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            word += ch;
            addToken(OPERATOR, operators[word], word);
        }
        else if (ch == '<' || ch == '>' || ch == '=' || ch == '!') {
            word += ch;
            if ((ch = getchar(infile)) == '=' || ch == '>') {
                word += ch;
            }
            else {
                infile.unget();
            }
            if (rel_operators.find(word) != rel_operators.end()) {
                addToken(REL_OPERATOR, rel_operators[word], word);
            }
            else {
                addToken(ERROR, 0, word);
            }
        }
        else {
            addToken(ERROR, 0, string(1, ch));
        }
    }

    infile.close();
}

void printTokens() {
    cout << "词法分析结果:" << endl;
    cout << "单词\t\t二元序列\t类型\t\t位置" << endl;
    for (const auto& token : tokens) {
        cout  << token.value << "\t\t"
            << token.type << ", " << token.attribute << "\t\t"
            << token.type << "\t\t"
            << token.line << ", " << token.column 
            << endl;
    }
}

int main() {
    lexicalAnalysis("source.c");
    printTokens();
    return 0;
}
