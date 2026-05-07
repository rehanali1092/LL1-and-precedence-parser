#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

using namespace std;

bool isNonTerminal(const string &sym) {
    return sym == "E";
}

bool isTerminal(const string &sym) {
    return !isNonTerminal(sym);
}

string stackToString(const vector<string> &stk) {
    string result;
    for (const string &sym : stk)
        result += sym;
    return result;
}

string inputToString(const vector<string> &tokens, int index) {
    string result;
    for (int j = index; j < tokens.size(); j++)
        result += tokens[j];
    return result;
}

vector<string> tokenize(const string &s) {
    vector<string> tokens;
    for (int i = 0; i < s.size(); i++) {
        if (isspace(static_cast<unsigned char>(s[i])))
            continue;
        if (s[i] == 'i' && i + 1 < s.size() && s[i + 1] == 'd') {
            tokens.push_back("id");
            i++;
            continue;
        }
        string tok(1, s[i]);
        if (tok == "+" || tok == "-" || tok == "*" || tok == "/" || tok == "(" || tok == ")") {
            tokens.push_back(tok);
        } else {
            tokens.clear();
            return tokens;
        }
    }
    tokens.push_back("$");
    return tokens;
}

string getTopTerminal(const vector<string> &stk) {
    for (int i = int(stk.size()) - 1; i >= 0; i--) {
        if (isTerminal(stk[i]))
            return stk[i];
    }
    return "$";
}

char getPrecedence(const map<pair<string, string>, char> &table,
                   const string &a,
                   const string &b) {
    auto it = table.find({a, b});
    if (it == table.end())
        return '?';
    return it->second;
}

bool reduceHandle(vector<string> &stk) {
    int n = int(stk.size());
    if (n >= 1 && stk[n - 1] == "id") {
        stk.pop_back();
        stk.push_back("E");
        return true;
    }
    if (n >= 3 && stk[n - 1] == ")" && stk[n - 2] == "E" && stk[n - 3] == "(") {
        stk.pop_back();
        stk.pop_back();
        stk.pop_back();
        stk.push_back("E");
        return true;
    }
    if (n >= 3 && stk[n - 1] == "E") {
        string op = stk[n - 2];
        if ((op == "+" || op == "-" || op == "*" || op == "/") && stk[n - 3] == "E") {
            stk.pop_back();
            stk.pop_back();
            stk.pop_back();
            stk.push_back("E");
            return true;
        }
    }
    return false;
}

int main() {
    map<pair<string, string>, char> precedence;

    precedence[{"+", "+"}] = '>';
    precedence[{"+", "-"}] = '>';
    precedence[{"+", "*"}] = '<';
    precedence[{"+", "/"}] = '<';
    precedence[{"+", "("}] = '<';
    precedence[{"+", ")"}] = '>';
    precedence[{"+", "id"}] = '<';
    precedence[{"+", "$"}] = '>';

    precedence[{"-", "+"}] = '>';
    precedence[{"-", "-"}] = '>';
    precedence[{"-", "*"}] = '<';
    precedence[{"-", "/"}] = '<';
    precedence[{"-", "("}] = '<';
    precedence[{"-", ")"}] = '>';
    precedence[{"-", "id"}] = '<';
    precedence[{"-", "$"}] = '>';

    precedence[{"*", "+"}] = '>';
    precedence[{"*", "-"}] = '>';
    precedence[{"*", "*"}] = '>';
    precedence[{"*", "/"}] = '>';
    precedence[{"*", "("}] = '<';
    precedence[{"*", ")"}] = '>';
    precedence[{"*", "id"}] = '<';
    precedence[{"*", "$"}] = '>';

    precedence[{"/", "+"}] = '>';
    precedence[{"/", "-"}] = '>';
    precedence[{"/", "*"}] = '>';
    precedence[{"/", "/"}] = '>';
    precedence[{"/", "("}] = '<';
    precedence[{"/", ")"}] = '>';
    precedence[{"/", "id"}] = '<';
    precedence[{"/", "$"}] = '>';

    precedence[{"(", "+"}] = '<';
    precedence[{"(", "-"}] = '<';
    precedence[{"(", "*"}] = '<';
    precedence[{"(", "/"}] = '<';
    precedence[{"(", "("}] = '<';
    precedence[{"(", ")"}] = '=';
    precedence[{"(", "id"}] = '<';

    precedence[{")", "+"}] = '>';
    precedence[{")", "-"}] = '>';
    precedence[{")", "*"}] = '>';
    precedence[{")", "/"}] = '>';
    precedence[{")", ")"}] = '>';
    precedence[{")", "$"}] = '>';

    precedence[{"id", "+"}] = '>';
    precedence[{"id", "-"}] = '>';
    precedence[{"id", "*"}] = '>';
    precedence[{"id", "/"}] = '>';
    precedence[{"id", ")"}] = '>';
    precedence[{"id", "$"}] = '>';

    precedence[{"$", "+"}] = '<';
    precedence[{"$", "-"}] = '<';
    precedence[{"$", "*"}] = '<';
    precedence[{"$", "/"}] = '<';
    precedence[{"$", "("}] = '<';
    precedence[{"$", "id"}] = '<';
    precedence[{"$", "$"}] = '=';

    cout << "Enter input string: ";
    string input;
    getline(cin, input);

    vector<string> tokens = tokenize(input);
    if (tokens.empty()) {
        cout << "Invalid input format. Use id, +, -, *, /, (, ) only." << endl;
        return 0;
    }

    vector<string> stk;
    stk.push_back("$");

    int index = 0;
    bool accepted = false;
    bool rejected = false;

    cout << "\n" << left << setw(25) << "Stack" << left << setw(25) << "Input" << "Action" << endl;
    cout << string(70, '-') << endl;

    while (!rejected) {
        string topTerminal = getTopTerminal(stk);
        string current = tokens[index];

        if (topTerminal == "$" && current == "$" && stk.size() == 2 && stk[1] == "E") {
            cout << left << setw(25) << stackToString(stk)
                 << left << setw(25) << inputToString(tokens, index)
                 << "Accept" << endl;
            accepted = true;
            break;
        }

        char relation = getPrecedence(precedence, topTerminal, current);
        if (relation == '<' || relation == '=') {
            stk.push_back(current);
            index += 1;
            cout << left << setw(25) << stackToString(stk)
                 << left << setw(25) << inputToString(tokens, index)
                 << "Shift" << endl;
        } else if (relation == '>') {
            bool reduced = reduceHandle(stk);
            if (!reduced) {
                cout << left << setw(25) << stackToString(stk)
                     << left << setw(25) << inputToString(tokens, index)
                     << "Reject" << endl;
                rejected = true;
                break;
            }
            cout << left << setw(25) << stackToString(stk)
                 << left << setw(25) << inputToString(tokens, index)
                 << "Reduce" << endl;
        } else {
            cout << left << setw(25) << stackToString(stk)
                 << left << setw(25) << inputToString(tokens, index)
                 << "Reject" << endl;
            rejected = true;
            break;
        }
    }

    if (accepted)
        cout << "\nString Accepted" << endl;
    else
        cout << "\nString Rejected" << endl;

    return 0;
}
