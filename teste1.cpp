#include <iostream>
#include <unordered_map>
#include <queue>
#include <string>
#include <vector> 
#include <fstream>
#include <locale> // Adicionando a biblioteca locale para suporte Unicode

using namespace std;

struct node {
    wstring ch; // Usando wstring para suportar caracteres Unicode
    int freq;
    node *left;
    node *right;
};

struct comp {
    bool operator()(node* l, node* r) {
        return l->freq > r->freq;
    }
};

node *create_node(const wstring& c, int frequency, node *left, node *right){
    node *p = new node;
    p->ch = c;
    p->freq = frequency;
    p->left = left;
    p->right = right;
    return p;
}

void encode(node *root, const wstring& str, unordered_map<wstring, string> &huffcode){
    if(!root) return;
    if(!root->left && !root->right){
        huffcode[root->ch] = string(str.begin(), str.end());
    }
    encode(root->left, str + L"0", huffcode);
    encode(root->right, str + L"1", huffcode);
}



void decode(node *root, int &top_index, const string& str){
    if(!root) return;
    if(!root->left && !root->right){
        wcout << root->ch;
        return;
    }
    top_index++;
    if(str[top_index] == '0') decode(root->left, top_index, str);
    else if(str[top_index] == '1') decode(root->right, top_index, str);
}

void buildHuffTree(const wstring& text){
    unordered_map<wstring, int> freq;
    for(wchar_t ch : text){
        freq[wstring(1, ch)]++;
    }

    priority_queue<node*, vector<node*> , comp> pq; 

    for(auto pair : freq){
        pq.push(create_node(pair.first, pair.second, nullptr, nullptr));
    }

    while(pq.size() != 1){
        node *left = pq.top();
        pq.pop();
        node *right = pq.top();
        pq.pop();
        int soma = left->freq + right->freq;
        pq.push(create_node(L"", soma, left, right));
    }

    node *root = pq.top();
    unordered_map<wstring, string> huffCode;
    encode(root, L"", huffCode);

    wcout << L"Codigos binarios da arvore criada: \n";
    for(auto pair : huffCode){
        wcout << pair.first << L": " << wstring(pair.second.begin(), pair.second.end()) << endl;

    }

    wcout << L"\nString original: " << text << endl;

    string str = "";
    for(wchar_t ch : text){
        str += huffCode[wstring(1, ch)];
    }
    wcout << L"String criptografada: " << wstring(str.begin(), str.end()) << endl;


    int top_index = -1;
    wcout << L"\nString despirocada: ";
    while(top_index < (int)str.size() - 2){
        decode(root, top_index, str);
    }
    wcout << endl;
}

int main(){
    wstring text = L"Bste é um texto de teste com caracteres Unicode. Aqui estão alguns exemplos de caracteres especiais: ♥, ☺, €, ☀, こんにちは, привет, مرحبا. Espero que este texto ajude a testar o suporte completo a Unicode em seu programa."; // Texto com caracteres Unicode
    setlocale(LC_ALL, ""); // Configurando a localização para suporte completo a Unicode
    buildHuffTree(text);
    return 0;
}
