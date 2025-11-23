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
pair<array<array<int, 4>, 21>, array<int, 7>> initDFA(){
    array<array<int, 4>, 21> dfa = {{
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

    array<int, 7> dfaFinalStates;
    dfaFinalStates[0] = 10;
    dfaFinalStates[1] = 15;
    dfaFinalStates[2] = 16;
    dfaFinalStates[3] = 17;
    dfaFinalStates[4] = 18;
    dfaFinalStates[5] = 19;
    dfaFinalStates[6] = 20;
    return {dfa, dfaFinalStates};
}

//НКА
//для каждого состояния будем хранить массив(вектор) из 4 элементов-сетов
//первый сет содержит в себе номера вершин, в которые можно попасть по букве 'a' 
//второй - по букве 'c' и так далее
//если нельзя попасть ни в какую вершину, то -1
//также возвращаем массив финальных состояний 
pair<vector<vector<set<int>>>, array<int, 1>> initNFA(){
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
    array<int, 1> nfaFinalStates;
    nfaFinalStates[0] = 14;
    return {edges, nfaFinalStates};
}

//проверка на принадлежность ПКА
//также как с дка, но вернем 3 массива
//первый - с переходами
//второй и третий - для финальных состояний в 1-ой и 2-ой частях ПКА соответственно
tuple<array<array<int, 4>, 32>, array<int, 7>, array<int, 11>> initAFA(){
    array<array<int, 4>, 32> afa = {{
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
        {22, 21, 21, 27},
        {21, 23, 21, 21},
        {21, 24, 21, 21},
        {21, 25, 21, 21},
        {21, 26, 21, 21},
        {21, -1, 21, 21},
        {21, 28, 21, 21},
        {21, 29, 21, 21},
        {21, 30, 21, 21},
        {21, 31, 21, 21},
        {21, -1, 21, 21}
    }};

    array<int, 7> afaFinalStates1;
    afaFinalStates1[0] = 10;
    afaFinalStates1[1] = 15;
    afaFinalStates1[2] = 16;
    afaFinalStates1[3] = 17;
    afaFinalStates1[4] = 18;
    afaFinalStates1[5] = 19;
    afaFinalStates1[6] = 20;

    array<int, 11> afaFinalStates2;
    afaFinalStates2[0] = 21;
    afaFinalStates2[1] = 22;
    afaFinalStates2[2] = 23;
    afaFinalStates2[3] = 24;
    afaFinalStates2[4] = 25;
    afaFinalStates2[5] = 26;
    afaFinalStates2[6] = 27;
    afaFinalStates2[7] = 28;
    afaFinalStates2[8] = 29;
    afaFinalStates2[9] = 30;
    afaFinalStates2[10] = 31;
    return {afa, afaFinalStates1, afaFinalStates2};
}


//проверка на принадлежность слова детерминированному конечному автомату
bool checkDFA(const array<array<int, 4>, 21>& dfa, const string& word, const string& alphabet, const array<int, 7>& finalStates){
    int currentState = 0; //текущее состояние
    for (int i = 0; i < word.size(); i++){
        currentState = dfa[currentState][getLetterNum(word[i], alphabet)];
        //cout << currentState << " " << word[i] << endl;
        if (currentState == -1){
            return false;
        }
    }
    for (int i = 0; i < 7; i++){
        if (currentState == finalStates[i]){
            return true;
        }
    } 
    return false;
}

//проверка на принадлежность слова недетерминированному конечному автомату
bool checkNFA(const vector<vector<set<int>>>& nfa, const string& word, const string& alphabet, const array<int,1> nfaFinalStates){
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
        if (i == nfaFinalStates[0]){
            return true;
        }
    }
    return false;
}





//проверка на принадлежность слова переключающемуся конечному автомату (ПКА)
bool checkAFA(const array<array<int, 4>, 32>& dfa, const string& word, const string& alphabet, const array<int, 7>& finalStates1, const array<int, 11>& finalStates2){
    int currentState = 21; //текущее состояние
    for (int i = 0; i < word.size(); i++){
        currentState = dfa[currentState][getLetterNum(word[i], alphabet)];
        //cout << currentState << " " << word[i] << endl;
        if (currentState == -1){
            return false;
        }
    }

    bool doesBelongSecondPart = false;
    for (int i = 0; i < 11; i++){
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
    for (int i = 0; i < 7; i++){
        if (currentState == finalStates1[i]){
            return true;
        }
    } 
    return false;
    

}



int main(){
    string alphabet = "acde";

    //ДКА
    array<array<int, 4>, 21> dfa;
    array<int, 7> dfaFinalStates;
    tie(dfa, dfaFinalStates) = initDFA();

    //НКА
    //для каждого состояния будем хранить массив(вектор) из 4 элементов-сетов
    //первый сет содержит в себе номера вершин, в которые можно попасть по букве 'a' 
    //второй - по букве 'c' и так далее
    vector<vector<set<int>>> nfa; 
    array<int, 1> nfaFinalStates;
    tie(nfa, nfaFinalStates) = initNFA();

    //ПКА
    array<array<int, 4>, 32> afa;
    array<int, 7> afaFinalStates1;
    array<int, 11> afaFinalStates2;
    tie(afa, afaFinalStates1, afaFinalStates2) = initAFA();

    int isError = 0;
    string errorWord = "";
    //будем генерировать слова длины от 0 до 100
    for (int i = 0; i < 100; i++){
        //по 1000 слов каждой длины
        for (int j = 0; j < 1000; j++){
            string word = getRandomString(i, alphabet);
            //string word = "aeeed";
            bool r1 = checkRegex(word);
            bool r2 = checkDFA(dfa, word, alphabet, dfaFinalStates);
            bool r3 = checkNFA(nfa, word, alphabet, nfaFinalStates);
            bool r4 = checkAFA(afa, word, alphabet, afaFinalStates1, afaFinalStates2);
            if ((r1 != r2) || (r1 != r3) || (r1 != r4)){
                //cout << 57 << r1 << r2 << r3 << r4 << endl;
                isError = 1;
                if (errorWord == ""){
                    errorWord = word;
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