#include <iostream>
#include <stack>
#include <map>
#include <vector>
#include <iomanip>

using namespace std;

map<pair<string, string>, string> table;

// Build parsing table
void buildTable() {
    table[{"E", "id"}] = "TE'";
    table[{"E", "("}]  = "TE'";

    table[{"E'", "+"}] = "+TE'";
    table[{"E'", ")"}] = "eps";
    table[{"E'", "$"}] = "eps";

    table[{"T", "id"}] = "FT'";
    table[{"T", "("}]  = "FT'";

    table[{"T'", "+"}] = "eps";
    table[{"T'", "*"}] = "*FT'";
    table[{"T'", ")"}] = "eps";
    table[{"T'", "$"}] = "eps";

    table[{"F", "id"}] = "id";
    table[{"F", "("}]  = "(E)";
}

// tokenize input
vector<string> tokenize(string s) {
    vector<string> t;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == 'i' && s[i + 1] == 'd') {
            t.push_back("id");
            i++;
        } else {
            t.push_back(string(1, s[i]));
        }
    }
    t.push_back("$");
    return t;
}

// build stack string
string stackToString(stack<string> st) {
    vector<string> temp;
    while (!st.empty()) {
        temp.push_back(st.top());
        st.pop();
    }

    string result;
    for (int i = temp.size() - 1; i >= 0; i--)
        result += temp[i];

    return result;
}

bool isNonTerminal(string s) {
    return (s == "E" || s == "E'" || s == "T" || s == "T'" || s == "F");
}

int main() {
    buildTable();

    string input;
    cout << "Enter input string: ";
    getline(cin, input);

    vector<string> tokens = tokenize(input);

    stack<string> st;
    st.push("$");
    st.push("E");

    int i = 0;

    cout << "\nStack\t\t\tInput\t\t\tAction\n";
    cout << "---------------------------------------------------------------\n";

    while (!st.empty()) {
        string top = st.top();
        string cur = tokens[i];

        string stackContents = stackToString(st);
        string inputContents;
        for (int j = i; j < tokens.size(); j++)
            inputContents += tokens[j];

        // match terminal
        if (top == cur) {
            cout << left << setw(25) << stackContents << left << setw(25) << inputContents;
            cout << "Match " << top << endl;
            st.pop();
            i++;
        }

        // non-terminal
        else if (isNonTerminal(top)) {
            auto it = table.find({top, cur});
            if (it == table.end()) {
                cout << left << setw(25) << stackContents << left << setw(25) << inputContents;
                cout << "Error" << endl;
                cout << "\nString Rejected\n";
                return 0;
            }

            string prod = it->second;
            cout << left << setw(25) << stackContents << left << setw(25) << inputContents;
            cout << top << " -> " << prod << endl;
            st.pop();

            if (prod != "eps") {
                // push RHS in reverse
                vector<string> rhs;

                for (int k = 0; k < prod.size();) {
                    if (prod[k] == 'i' && k + 1 < prod.size() && prod[k + 1] == 'd') {
                        rhs.push_back("id");
                        k += 2;
                    } else if (k + 1 < prod.size() && (prod[k] == 'E' || prod[k] == 'T') && prod[k + 1] == '\'') {
                        rhs.push_back(prod.substr(k, 2));
                        k += 2;
                    } else {
                        rhs.push_back(string(1, prod[k]));
                        k++;
                    }
                }

                for (int k = int(rhs.size()) - 1; k >= 0; k--) {
                    st.push(rhs[k]);
                }
            }
        }

        else {
            cout << "Error\n";
            cout << "\nString Rejected\n";
            return 0;
        }
    }

    if (i == tokens.size())
        cout << "\nString Accepted\n";
    else
        cout << "\nString Rejected\n";

    return 0;
}