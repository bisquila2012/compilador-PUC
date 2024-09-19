#include <bits/stdc++.h>

using namespace std;

map<string, int> variaveis; // Para armazenar as variáveis

map<int, string> programa;   // Para armazenar o programa BASIC

int linhaAtual = 10;         // Controla a linha atual

bool pararPrograma = false;  // Flag para parar o programa

// Função para dividir uma string

vector<string> dividir(const string& str, char delimitador) {
    vector<string> tokens;

    string token;

    istringstream fluxoDeTokens(str);
    
    while (getline(fluxoDeTokens, token, delimitador)) {
        if (!token.empty()) { // Ignora tokens vazios

            tokens.push_back(token);
        }
    }
    return tokens;
}

// Função para processar múltiplos comandos na mesma linha, separados por ":"

vector<string> dividirPorDoisPontos(const string& str) {

    return dividir(str, ':');
}

// Função para carregar a entrada de um arquivo
void carregarPrograma(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo!" << endl;
    }
    
    string linha;
    
    while (getline(arquivo, linha)) {
        int numeroDaLinha;
        string comando;
        istringstream iss(linha);
        
        iss >> numeroDaLinha;
        getline(iss, comando);
        
        programa[numeroDaLinha] = comando;
        cout << "Carregando linha " << numeroDaLinha << ": " << comando << endl;
    }
}

// Função para comparar valores
bool avaliarCondicao(int esquerda, string op, int direita) {
    if (op == "<=") return esquerda <= direita; if (op == ">=") return esquerda >= direita; if (op == "<") return esquerda < direita; if (op == ">") return esquerda > direita; if (op == "==") return esquerda == direita; if (op == "!=") return esquerda != direita;

    return false;
}

// Função para processar uma única linha de comando BASIC

bool processarComando(const string& comando) {

    cout << "Executando comando: " << comando << endl;

    vector<string> tokens = dividir(comando, ' ');

    // Remover comentários

    if (tokens[0] == "rem") {

        return false; // Ignora a linha de comentário
    }

    // Comando de saída

    if (tokens[0] == "print") {

        if (tokens[1][0] == '"') {

            string saida = comando.substr(comando.find("\"") + 1);

            saida = saida.substr(0, saida.rfind("\""));

            cout << saida << endl;
        } else 
        {
            // Exibir variável

            if (variaveis.find(tokens[1]) != variaveis.end()) {

                cout << variaveis[tokens[1]] << endl;

            } else {

                cout << "Erro: Variável não definida -> " << tokens[1] << endl;
            }
        }
        return false;
    }

    // Comando de entrada
    else if (tokens[0] == "input") {

        int valor;
        cout << "Digite " << tokens[1] << ": ";

        cin >> valor;
        
        variaveis[tokens[1]] = valor;

        cout << "Variável " << tokens[1] << " atribuída com o valor: " << valor << endl;

        return false;
    }

    // Atribuição de variável

    else if (tokens[1] == "=") {

        if (tokens.size() >= 5 && (tokens[3] == "+" || tokens[3] == "-")) {

            if (tokens[3] == "+") {

                variaveis[tokens[0]] = variaveis[tokens[2]] + variaveis[tokens[4]];

            } else 
            {
                variaveis[tokens[0]] = variaveis[tokens[2]] - variaveis[tokens[4]];
            }
        } else 
        {
            try {
                variaveis[tokens[0]] = stoi(tokens[2]);

            } catch (...) {

                if (variaveis.find(tokens[2]) != variaveis.end()) {

                    variaveis[tokens[0]] = variaveis[tokens[2]]; // Copiar valor da variável

                } else {

                    cout << "Erro: Atribuição inválida -> " << tokens[0] << endl;
                }
            }
        }
        cout << "Variável " << tokens[0] << " atribuída com o valor: " << variaveis[tokens[0]] << endl;
        return false;
    }

    // Goto

    else if (tokens[0] == "goto") {

        if (programa.find(stoi(tokens[1])) != programa.end()) {

            linhaAtual = stoi(tokens[1]);

            cout << "Indo para linha: " << linhaAtual << endl;
        } else {

            cout << "Erro: Linha " << tokens[1] << " não encontrada!" << endl;

            pararPrograma = true; // Para o programa, pois a linha não existe
        }

        return true;
    }

    // Condicional if com operadores adicionais

    else if (tokens[0] == "if")
     {

        if (variaveis.find(tokens[1]) != variaveis.end()) 
        {
            int valorVar = variaveis[tokens[1]];
            int valorComparar = stoi(tokens[3]);

            cout << "Comparando " << tokens[1] << " (" << valorVar << ") com " << valorComparar << endl;

            if (avaliarCondicao(valorVar, tokens[2], valorComparar)) {

                if (programa.find(stoi(tokens[5])) != programa.end()) {

                    linhaAtual = stoi(tokens[5]);
                    cout << "Condição verdadeira, indo para linha: " << linhaAtual << endl;
                    return true;

                } else 
                {
                    cout << "Erro: Linha " << tokens[5] << " não encontrada!" << endl;
                    pararPrograma = true;
                }
            }
        } else 
        {
            cout << "Erro: Variável " << tokens[1] << " não encontrada para a comparação." << endl;
        }
    }

    // Comando halt para parar a execução
    else if (tokens[0] == "halt") {

        pararPrograma = true;
        cout << "Programa finalizado com o comando HALT." << endl;

        return true;
    }

    return false;
}

// Função para executar o programa

void executarPrograma() {

    while (programa.find(linhaAtual) != programa.end() && !pararPrograma) {

        cout << " -----------------------------------------\n";
        cout << "Linha atual: " << linhaAtual << endl;
        
        string linhaComando = programa[linhaAtual];
        vector<string> comandos = dividirPorDoisPontos(linhaComando);
        bool linhaMudou = false;

        for (const string& comando : comandos) {

            if (processarComando(comando)) {

                linhaMudou = true;
                break; // Sai do loop assim que um comando "goto" ou "halt" for executado
            }
        }

        // Atualizar corretamente linhaAtual se nenhum goto ou halt for executado

        if (!linhaMudou) {

            linhaAtual += 10; // Avança para a próxima linha se não houver mudança
        }
        
        // Log para verificar valores de A e S após cada iteração
        if (variaveis.find("A") != variaveis.end()) {

            cout << "Valor de A após iteração: " << variaveis["A"] << endl;
        }
        if (variaveis.find("S") != variaveis.end()) {

            cout << "Valor de S após iteração: " << variaveis["S"] << endl;
        }
    }
}

int main() {
    
    // Carregar o programa BASIC a partir do arquivo
    carregarPrograma("compilador.txt");

    // Executar o programa
    executarPrograma();
}
