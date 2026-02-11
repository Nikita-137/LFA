#include <iostream>
#include <string>
#include <random>

int RandomNumber(int a, int b) {
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(a, b);
return dist(gen);
};

class Grammar {
    public:
    char VN[4];
    char VT[2];
    //Конструктор
    Grammar() {
        VN[0] = 'S';
        VN[1] = 'A';
        VN[2] = 'C';
        VN[3] = 'D';
        VT[0] = 'a';
        VT[1] = 'b';
    };
    //множество правил продукции
    void replace(std::string *s, int variant) {
        int l = (*s).length();
        char LC = (*s)[l-1];
        if (LC == VN[0]) {
            (*s)[l-1] = 'a';
            *s = *s + 'A';
        };
        if (LC == VN[1]) {
            if (variant == 0) {
                (*s)[l-1] = 'b';
                *s = *s + 'S';
            } else {
                (*s)[l-1] = 'b';
                *s = *s + 'D';
            };
        };
        if (LC == VN[2]) {
            if (variant == 0) {
                (*s)[l-1] = 'a';
            } else {
                (*s)[l-1] = 'b';
                *s = *s + 'A';
            };
        };
        if (LC == VN[3]) {
            if (variant == 0) {
                (*s)[l-1] = 'b';
                *s = *s + 'C';
            } else {
                (*s)[l-1] = 'a';
                *s = *s + 'D';
            };
        };
    };
    //проверка конечного состояния
    int check(std::string *s) {
        int l = (*s).length();
        char LC = (*s)[l-1];
        if (LC == VT[0]) {
            return 1;
        };
        if (LC == VT[1]) {
            return 1;
        };
        return 0;
    };
};

int main() {
    Grammar gr;
    std::string str = "S";
    for (int i = 0; i<5; i++) {
        int z = 0;
        while (z == 0) {
            std::cout << str << "\n";
            gr.replace(&str, RandomNumber(0,1));
            z = gr.check(&str);
        };
        std::cout << str << "\nNext string:\n";
        str = "S";
    };
    return 0;
};