#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Substitution {
    string variable;
    string term;
};

vector<Substitution> unify(string E1, string E2, map<string, string> subs);
bool occurs_check(string variable, string term, map<string, string> subs);
vector<Substitution> compose(vector<Substitution> subs1, vector<Substitution> subs2);
string apply_substitutions(string term, map<string, string> subs);

int main() {
    string E1, E2;
    cout << "Enter the first term: ";
    cin >> E1;
    cout << "Enter the second term: ";
    cin >> E2;

    map<string, string> subs;
    vector<Substitution> result = unify(E1, E2, subs);
    if (result.empty()) {
        cout << "Unification failed." << endl;
    } else {
        cout << "The unifier is: ";
        for (int i = 0; i < result.size(); i++) {
            cout << result[i].variable << " = " << result[i].term;
            if (i != result.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }

    return 0;
}

vector<Substitution> unify(string E1, string E2, map<string, string> subs) {
    if (E1.empty() || E2.empty()) {
        if (E1 == E2) {
            return {};
        } else {
            return {{}, {}};
        }
    }
    if (isupper(E1[0])) {
        if (occurs_check(E1, E2, subs)) {
            return {{}, {}};
        } else {
            subs[E1] = E2;
            return {{E1, E2}};
        }
    }
    if (isupper(E2[0])) {
        if (occurs_check(E2, E1, subs)) {
            return {{}, {}};
        } else {
            subs[E2] = E1;
            return {{E2, E1}};
        }
    }
    if (E1[0] != E2[0]) {
        return {{}, {}};
    }
    string HE1, HE2;
    int i = 0;
    while (E1[i] != '(') {
        HE1 += E1[i];
        HE2 += E2[i];
        i++;
    }
    int count1 = 1, count2 = 1;
    int j = i + 1;
    while (count1 != 0) {
        if (E1[j] == '(') {
            count1++;
        } else if (E1[j] == ')') {
            count1--;
        }
        j++;
    }
    i = j;
    while (count2 != 0) {
        if (E2[i] == '(') {
count2++;
} else if (E2[i] == ')') {
count2--;
}
i++;
}
string TE1 = E1.substr(j, i - j - 1);
string TE2 = E2.substr(j, i - j - 1);
vector<Substitution> SUBS1 = unify(HE1, HE2, subs);
if (SUBS1.empty()) {
return {{}, {}};
}
TE1 = apply_substitutions(TE1, subs);
TE2 = apply_substitutions(TE2, subs);
vector<Substitution> SUBS2 = unify(TE1, TE2, subs);
if (SUBS2.empty()) {
return {{}, {}};
}
return compose(SUBS1, SUBS2);
}

bool occurs_check(string variable, string term, map<string, string> subs) {
if (isupper(term[0])) {
if (subs.count(term)) {
return occurs_check(variable, subs[term], subs);
} else {
return false;
}
}
int i = 0;
while (i < term.length()) {
if (isupper(term[i])) {
if (occurs_check(variable, term.substr(i), subs)) {
return true;
}
} else if (term[i] == '(') {
int count = 1;
i++;
while (count != 0) {
if (term[i] == '(') {
count++;
} else if (term[i] == ')') {
count--;
}
i++;
}
}
i++;
}
return false;
}

vector<Substitution> compose(vector<Substitution> subs1, vector<Substitution> subs2) {
for (int i = 0; i < subs2.size(); i++) {
subs1.push_back(subs2[i]);
}
return subs1;
}

string apply_substitutions(string term, map<string, string> subs) {
if (isupper(term[0])) {
if (subs.count(term)) {
return apply_substitutions(subs[term], subs);
} else {
return term;
}
}
string result = "";
int i = 0;
while (i < term.length()) {
if (isupper(term[i])) {
result += apply_substitutions(term.substr(i), subs);
while (i < term.length() && (isupper(term[i]) || term[i] == '(' || term[i] == ')')) {
i++;
}
} else {
result += term[i];
i++;
}
}
return result;
}
