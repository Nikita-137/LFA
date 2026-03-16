#include <iostream>
#include <string>
#include <random>
#include <vector>

#include <set>
#include <map>
#include <algorithm>

// Из первой лабы
int RandomNumber(int a, int b) {
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(a, b);
return dist(gen);
};

class Grammar {
    public:
    std::vector <char> VN;
    std::vector <std::vector <char>> VT;
    int z = 7;
    //Конструктор
    Grammar() { //Дефолт

    };

    Grammar(int k) {
        VN.resize(7);
        VT.resize(7, std::vector<char>(2));
        VN[0] = 'S';
        VN[1] = 'A';
        VN[2] = 'A';
        VN[3] = 'C';
        VN[4] = 'C';
        VN[5] = 'D';
        VN[6] = 'D';
        
        VT[0][0] = 'a'; VT[0][1] = 'A';
        VT[1][0] = 'b'; VT[1][1] = 'S';
        VT[2][0] = 'b'; VT[2][1] = 'D';
        VT[3][0] = 'a'; VT[3][1] = 'F';
        VT[4][0] = 'b'; VT[4][1] = 'A';
        VT[5][0] = 'b'; VT[5][1] = 'C';
        VT[6][0] = 'a'; VT[6][1] = 'D';
    };
    //Создание строки
    void StringCreator(std::string *s) {
        int l = (*s).length();
        while((*s)[l-1]!= 'F') {
            std::vector <int> variants;
            for (int i = 0; i<z; i++) {
                if ((*s)[l-1] == VN[i]) {
                    variants.push_back(i);
                };
            };
            int k = RandomNumber(0, variants.size()-1);
            int RI = variants[k];
            (*s)[l-1] = VT[RI][0];
            (*s) = (*s) + VT[RI][1];
            //std::cout << (*s) << "\n";
            l = (*s).length();
            };
        (*s).pop_back();
    };
    
};

class FiniteAutomaton {
    public:
    int n;
    std::vector <char> From;
    std::vector <char> Through;
    std::vector <char> To;
    //Конструктор
    FiniteAutomaton() { //Дефолт
        n = 6;
        From.resize(n);
        Through.resize(n);
        To.resize(n);
        From[0] = '0';  Through[0] = 'b'; To[0] = '0';
        From[1] = '0';  Through[1] = 'b'; To[1] = '1';
        From[2] = '1';  Through[2] = 'b'; To[2] = '2';
        From[3] = '0';  Through[3] = 'a'; To[3] = '0';
        From[4] = '2';  Through[4] = 'a'; To[4] = '2';
        From[5] = '1';  Through[5] = 'a'; To[5] = '1';
    };
    //Демонстрация конечного автомата
    void Show() {
        for (int i = 0; i<n; i++) {
            std::cout << "q" << From[i] << "--" << Through[i] << "-->" << "q" << To[i] << "\n";
        };
    };
    //Проверка
    int Check(std::string *s) {
        *s = *s + "F";
        int l = (*s).length();
        while (l>1) {
            int l1 = (*s).length();
            for (int i = 0; i<n; i++) {
                if ((*s)[l-2] == Through[i]) {
                    if ((*s)[l-1] == To[i]) {
                        (*s)[l-2] = From[i];
                        (*s).pop_back(); 
                    };
                };
            };
            //std::cout << (*s) << "\n";
            l = (*s).length();
            if (l == l1) {
                return 0;
            };
        };
        if ((*s) == "S") {
            return 1;
        };
        return 0;
    };
};

//Начало второй лабы

Grammar automatonToGrammar(FiniteAutomaton fa) {
    Grammar g;
    g.z = fa.n;
    for (int i = 0; i<g.z; i++) {
        g.VN.resize(g.z);
        g.VT.resize(g.z, std::vector<char>(g.z));
        for (int i = 0; i<g.z; i++) {
            g.VN[i] = fa.From[i];
            g.VT[i][0] = fa.Through[i];
            g.VT[i][1] = fa.To[i];
        };
    };
    return g;
};

void ShowGrammar(Grammar g) {
    for (int i = 0; i<g.z; i++) {
        std::cout << "q" << g.VN[i] << "->" << g.VT[i][0] << "q" << g.VT[i][1];
        if (g.VN[i]=='2' && g.VT[i][1]=='2') {
            std::cout << "\n";
            std::cout << "q" << g.VN[i] << "->" << g.VT[i][0];
        };
        std::cout << "\n";
    };
};

int IsDeterministic(FiniteAutomaton fa) {
    for (int i = 0; i<fa.n; i++) {
        for (int j = i+1; j<fa.n; j++) {
            if (fa.From[i] == fa.From[j]) {
                if (fa.Through[i] == fa.Through[j]) {
                    std::cout << 'q' << fa.From[i] << "-->" << fa.Through[i] << "-->" << 'q' << fa.To[i] << "\n";
                    std::cout << 'q' << fa.From[j] << "-->" << fa.Through[j] << "-->" << 'q' << fa.To[j] << "\n";
                    return 0;
                };
            };
        };
    };
    return 1;
};

FiniteAutomaton MakeDetermistic(FiniteAutomaton nfa) {
    FiniteAutomaton dfa;
    
    std::vector<std::string> sets;  // множества как строки
    std::map<std::string, char> setToId;  // множество -> новый идентификатор
    
    sets.push_back("0");
    setToId["0"] = '1';
    
    int currentSet = 0;
    std::vector<std::tuple<char, char, char>> dfaTransitions;
    
    char nextId = '2';
    
    while (currentSet < sets.size()) {
        std::string set = sets[currentSet];
        char fromId = setToId[set];
        for (char symbol : {'a', 'b'}) {
            std::string newSet = "";
            for (char state : set) {
                for (int i = 0; i < nfa.n; i++) {
                    if (nfa.From[i] == state && nfa.Through[i] == symbol) {
                        char toState = nfa.To[i];
                        if (newSet.find(toState) == std::string::npos) {
                            newSet += toState;
                        }
                    }
                }
            }
            std::sort(newSet.begin(), newSet.end());
            if (!newSet.empty()) {
                if (setToId.find(newSet) == setToId.end()) {
                    sets.push_back(newSet);
                    setToId[newSet] = nextId++;
                }
                dfaTransitions.push_back({fromId, symbol, setToId[newSet]});
            };
        };
        currentSet++;
    };

    dfa.n = dfaTransitions.size();
    dfa.From.resize(dfa.n);
    dfa.Through.resize(dfa.n);
    dfa.To.resize(dfa.n);
    
    for (int i = 0; i < dfa.n; i++) {
        dfa.From[i] = std::get<0>(dfaTransitions[i]);
        dfa.Through[i] = std::get<1>(dfaTransitions[i]);
        dfa.To[i] = std::get<2>(dfaTransitions[i]);
    }
    
    return dfa;
}

int main() {
    Grammar gr;
    FiniteAutomaton fa;
    //fa.Show();
    //std::cout << "\n";
    gr = automatonToGrammar(fa);
    ShowGrammar(gr);
    std::cout << "\n";

    std::cout << IsDeterministic(fa) << "\n";
    std::cout << "\n";

    fa = MakeDetermistic(fa);
    fa.Show();
    return 0;
};