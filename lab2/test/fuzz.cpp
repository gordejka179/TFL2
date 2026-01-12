#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <regex>
#include <set>
#include <array>

using namespace std;

//РЕГУЛЯРКА ИЗ УСЛОВИЯ:
//(dcdc*c(d*|e*e)|e(cc)*(ee|d)|cac|dd)*c(a|c|d|e)

//генерация случайной строки, заданной длины n, состоящей из символов 
//фиксированного алфавита
string getRandomString(int n, const string& alphabet) {
    string s = "";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, alphabet.size() - 1);
    for (int i = 0; i < n; i++){
        s += alphabet[dist(gen)];
    }
    return s;
}

//проверка на принадлежность слова регулярке
bool checkRegex(const string& word) {
    regex r("(dcdc*c(d*|e*e)|e(cc)*(ee|d)|cac|dd)*c(a|c|d|e)");
    return regex_match(word, r);
}

//получить номер буквы в алфавите
int getLetterNum(const char c, const string& alphabet){
    if (c == 'a') {
        return 0;
    }
    if (c == 'c') {
        return 1;
    }
    if (c == 'd') {
        return 2;
    }
    if (c == 'e') {
        return 3;
    }
    return 179;
}

//ДКА
//храним 21 состояние (столько же в ДКА)
//состояние 0 - стартовое
//и смотрим в какие состояния по буквам a,c,d,e можно перейти соответственно
//если перехода нет, то будет -1
//например, из состояния 0 можно перейти: 
//по букве 'a' нет перехода,
//по букве 'с' в состояние 5,
//по букве 'd' в состояние 2,
//по букве 'e' в состояние 1
//также возвращаем массив финальных состояний
pair<vector<vector<int>>, vector<int>> initDFA(){
    vector<vector<int>> dfa = {{
        {-1, 5, 2, 1},
        {-1, 7, 0, 8},
        {-1, 3, 0, -1},
        {-1, -1, 4, -1},
        {-1, 11, -1, -1},
        {15, 10, 10, 10},
        {-1, 14, 6, 1},
        {-1, 1, -1, -1},
        {-1, -1, -1, 0},
        {-1, 13, 6, 9},
        {-1, -1, -1, -1},
        {-1, 12, 6, 9},
        {15, 18, 19, 20},
        {15, 17, 10, 10},
        {15, 10, 16, 10},
        {-1, 0, -1, -1},
        {-1, 11, -1, -1},
        {-1, 7, 0, 8},
        {15, 18, 19, 20},
        {-1, 14, 6, 1},
        {-1, 13, 6, 9}
    }};

    vector<int> dfaFinalStates;
    dfaFinalStates.push_back(10);
    dfaFinalStates.push_back(15);
    dfaFinalStates.push_back(16);
    dfaFinalStates.push_back(17);
    dfaFinalStates.push_back(18);
    dfaFinalStates.push_back(19);
    dfaFinalStates.push_back(20);
    return {dfa, dfaFinalStates};
}

//НКА
//для каждого состояния будем хранить массив(вектор) из 4 элементов-сетов
//первый сет содержит в себе номера вершин, в которые можно попасть по букве 'a' 
//второй - по букве 'c' и так далее
//если нельзя попасть ни в какую вершину, то -1
//также возвращаем массив финальных состояний 
pair<vector<vector<set<int>>>, vector<int>> initNFA(){
    vector<vector<set<int>>> edges;

    //начинаем для вершины с номером 0
    edges.push_back({
            {-1},
            {13, 7},
            {8, 1},
            {5}
        }
    );
    edges.push_back({
            {-1},
            {2},
            {-1},
            {-1}
        }
    );
    edges.push_back({
            {-1},
            {-1},
            {3},
            {-1}
        }
    );
    edges.push_back({
            {-1},
            {3, 4},
            {-1},
            {-1}
        }
    );
    edges.push_back({
            {-1},
            {13, 7},
            {6, 8, 1},
            {0, 10, 5}
        }
    );
    edges.push_back({
            {-1},
            {11},
            {0},
            {9}
        }
    );
    edges.push_back({
            {-1},
            {7, 13},
            {6, 8, 1},
            {5}
        }
    );
    edges.push_back({
            {14},
            {14},
            {14},
            {14}
        }
    );
    edges.push_back({
            {-1},
            {-1},
            {0},
            {-1}
        }
    );
    edges.push_back({
            {-1},
            {-1},
            {-1},
            {0}
        }
    );
    edges.push_back({
            {-1},
            {-1},
            {-1},
            {0, 10}
        }
    );
    edges.push_back({
            {-1},
            {5},
            {-1},
            {-1}
        }
    );

    edges.push_back({
            {-1},
            {0},
            {-1},
            {-1}
        }
    );

    edges.push_back({
            {12},
            {-1},
            {-1},
            {-1}
        }
    );

    edges.push_back({
            {-1},
            {-1},
            {-1},
            {-1}
        }
    );
    vector<int> nfaFinalStates;
    nfaFinalStates.push_back(14);
    return {edges, nfaFinalStates};
}

//задаём ПКА1
//также как с дка, но вернем 3 массива
//первый - с переходами
//второй и третий - для финальных состояний в 1-ой и 2-ой частях ПКА1 соответственно
tuple<vector<vector<int>>, vector<int>, vector<int>> initAFA1(){
    vector<vector<int>> afa = {{
        //первая часть:
        {-1, 5, 2, 1},
        {-1, 7, 0, 8},
        {-1, 3, 0, -1},
        {-1, -1, 4, -1},
        {-1, 11, -1, -1},
        {15, 10, 10, 10},
        {-1, 14, 6, 1},
        {-1, 1, -1, -1},
        {-1, -1, -1, 0},
        {-1, 13, 6, 9},
        {-1, -1, -1, -1},
        {-1, 12, 6, 9},
        {15, 18, 19, 20},
        {15, 17, 10, 10},
        {15, 10, 16, 10},
        {-1, 0, -1, -1},
        {-1, 11, -1, -1},
        {-1, 7, 0, 8},
        {15, 18, 19, 20},
        {-1, 14, 6, 1},
        {-1, 13, 6, 9},

        //вторая часть:
        {21, 21, 21, 22},
        {21, 23, 21, 22},
        {21, 24, 21, 22},
        {21, 25, 21, 22},
        {21, 26, 21, 22},
        {21, 27, 21, 22},
        {-1, 22, -1, -1}

    }};

    vector<int> afaFinalStates1{10, 15, 16, 17, 18, 19, 20};

    vector<int> afaFinalStates2{21, 22, 23, 24, 25, 26};
    return {afa, afaFinalStates1, afaFinalStates2};
}

//задаём вторую часть ПКА2 как НКА
pair<vector<vector<set<int>>>, vector<int>> initNFAForSecondPartInSecondAFA(){
    vector<vector<set<int>>> edges;
    edges.push_back({
            {-1},
            {0, 1},
            {0},
            {0}
        }
    );
    edges.push_back({
            {2},
            {3},
            {0, 3},
            {0, 3}
        }
    );
    edges.push_back({
            {-1},
            {0},
            {-1},
            {-1}
        }
    );
    edges.push_back({
            {-1},
            {-1},
            {-1},
            {-1}
        }
    );
    
    vector<int> nfaFinalStates;
    nfaFinalStates.push_back(2);
    nfaFinalStates.push_back(3);
    return {edges, nfaFinalStates};
    
}


//проверка на принадлежность слова детерминированному конечному автомату
bool checkDFA(const vector<vector<int>>& dfa, const string& word, const string& alphabet, const vector<int>& finalStates){
    int currentState = 0; //текущее состояние
    for (int i = 0; i < word.size(); i++){
        currentState = dfa[currentState][getLetterNum(word[i], alphabet)];
        if (currentState == -1){
            return false;
        }
    }
    for (int i = 0; i < finalStates.size(); i++){
        if (currentState == finalStates[i]){
            return true;
        }
    } 
    return false;
}

//проверка на принадлежность слова недетерминированному конечному автомату
bool checkNFA(const vector<vector<set<int>>>& nfa, const string& word, const string& alphabet, const vector<int>& nfaFinalStates){
    set<int> oldStates;
    set<int> newStates;
    oldStates.insert(0); //добавляем стартовую вершину

    for (int i = 0; i < word.size(); i++){
        for (int q1: oldStates){
            for (int q2: nfa[q1][getLetterNum(word[i], alphabet)]){
                //cout << q2 << endl;
                if (q2 != -1){
                    newStates.insert(q2);
                }
            }
        }
        oldStates = newStates;
        newStates.clear();
    }

    for (int i: oldStates){
        for (int j: nfaFinalStates){
            if (i == j){
                return true;
            }
        }
    }
    return false;
}





//проверка на принадлежность слова переключающемуся конечному автомату (ПКА)
bool checkAFA1(const vector<vector<int>>& dfa, const string& word, const string& alphabet, const vector<int>& finalStates1, const vector<int>& finalStates2){
    int currentState = 21; //текущее состояние
    for (int i = 0; i < word.size(); i++){
        currentState = dfa[currentState][getLetterNum(word[i], alphabet)];
        if (currentState == -1){
            return false;
        }
    }

    bool doesBelongSecondPart = false;
    for (int i = 0; i < finalStates2.size(); i++){
        if (currentState == finalStates2[i]){
            doesBelongSecondPart = true;
        }
    } 

    if (!doesBelongSecondPart){
        return false;
    }

    currentState = 0; //текущее состояние
    for (int i = 0; i < word.size(); i++){
        currentState = dfa[currentState][getLetterNum(word[i], alphabet)];
        if (currentState == -1){
            return false;
        }
    }
    for (int i = 0; i < finalStates1.size(); i++){
        if (currentState == finalStates1[i]){
            return true;
        }
    } 
    return false;
    

}

//проверка для ПКА2
//одна из частей - дка, другая - нка
bool checkAFA2(const string& word, const string& alphabet,
    const vector<vector<set<int>>>& nfa, const vector<int>& nfaFinalStates, 
    const vector<vector<int>>& dfa, const vector<int>& dfaFinalStates){
    bool r3 = checkNFA(nfa, word, alphabet, nfaFinalStates);
    if (!r3){
        return false;
    }

    bool r4 = checkDFA(dfa, word, alphabet, dfaFinalStates);
    if (!r4){
        return false;
    }

    return true;

}





int main(){
    string alphabet = "acde";

    //ДКА
    vector<vector<int>> dfa;
    vector<int> dfaFinalStates;
    
    tie(dfa, dfaFinalStates) = initDFA();

    //НКА
    //для каждого состояния будем хранить массив(вектор) из 4 элементов-сетов
    //первый сет содержит в себе номера вершин, в которые можно попасть по букве 'a' 
    //второй - по букве 'c' и так далее
    vector<vector<set<int>>> nfa; 
    vector<int> nfaFinalStates;
    tie(nfa, nfaFinalStates) = initNFA();

    //ПКА1
    vector<vector<int>> afa;
    vector<int> afaFinalStates1;
    vector<int> afaFinalStates2;
    tie(afa, afaFinalStates1, afaFinalStates2) = initAFA1();

    //ПКА2
    vector<vector<set<int>>> nfaForSecondPartInSecondAFA; 
    vector<int> nfaForSecondPartInSecondAFAFinalStates;
    tie(nfaForSecondPartInSecondAFA, nfaForSecondPartInSecondAFAFinalStates) = initNFAForSecondPartInSecondAFA();

    int isError = 0;
    string errorWord = "";

    for (int i = 1; i < 60; i++){
        if (isError){
            break;
        }
        for (int j = 0; j < 100000; j++){
            string word = getRandomString(i, alphabet);
            //string word = "eccccccdca";
            //eccccccdcd

            bool r1 = checkRegex(word);
            bool r2 = checkDFA(dfa, word, alphabet, dfaFinalStates);
            bool r3 = checkNFA(nfa, word, alphabet, nfaFinalStates);
            bool r4 = checkAFA1(afa, word, alphabet, afaFinalStates1, afaFinalStates2);
            bool r5 = checkAFA2(word, alphabet, nfaForSecondPartInSecondAFA, nfaForSecondPartInSecondAFAFinalStates, dfa, dfaFinalStates);
            if ((r1 != r2) || (r1 != r3) || (r1 != r4)|| (r1 != r5)){
                cout << 57 << r1 << r2 << r3 << r4 << endl;
                isError = 1;
                if (errorWord == ""){
                    errorWord = word;
                    break;
                }
            }
        }
    }
    if (isError == 1){
        cout << "ошибка: слово " + errorWord + " нарушает эквивалентность"; 
    }else{
        cout << "все тесты успешны" << endl;
    }
}