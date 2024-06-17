#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <windows.h>

struct Cadastro {
    char nome[100];
    char cpf[12];
    int dia_nascimento;
    int mes_nascimento;
    int ano_nascimento;
    int horarioEntradaHora;
    int horarioEntradaMinuto;
};

int calcularIdade(int diaNascimento, int mesNascimento, int anoNascimento); 
void salvarInformacoes(struct Cadastro cadastro, int idade, const char* situacao);
void visualizarRelatorios(); 
int loginEnfermeira(); 
void menuEnfermeira(); 
void iniciarPreTriagem();
void ordeprioridade(FILE *file1, FILE *file2);
void copiarConteudo(FILE *file1, FILE *file2);

int main() {
    UINT CPAGE_UTF8 = 65001;
    SetConsoleOutputCP(CPAGE_UTF8);
    
    system("cls");

    int opcao;

    do {
        printf("----- Menu Principal -----\n");
        printf("1. Iniciar Pré-triagem\n");
        printf("2. Área da Enfermeira\n");
        printf("3. Encerrar\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer do teclado

        switch (opcao) {
            case 1:
                iniciarPreTriagem();
                break;
            case 2:
                if (loginEnfermeira()) {
                    menuEnfermeira();
                }
                break;
            case 3:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    } while (opcao != 3);

    return 0;
}

int calcularIdade(int diaNascimento, int mesNascimento, int anoNascimento) {
    time_t agora;
    struct tm *agora_info;
    int anoAtual, mesAtual, diaAtual;

    time(&agora);
    agora_info = localtime(&agora);

    anoAtual = agora_info->tm_year + 1900;
    mesAtual = agora_info->tm_mon + 1;
    diaAtual = agora_info->tm_mday;

    int idade = anoAtual - anoNascimento;

    if (mesAtual < mesNascimento || (mesAtual == mesNascimento && diaAtual < diaNascimento)) {
        idade--;
    }

    return idade;
}

void salvarInformacoes(struct Cadastro cadastro, int idade, const char* situacao) {
    FILE *file = fopen("informacoes_paciente.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    fprintf(file, "Nome: %s", cadastro.nome);
    fprintf(file, "CPF: %s\n", cadastro.cpf);
    fprintf(file, "Data de Nascimento: %02d/%02d/%04d\n", cadastro.dia_nascimento, cadastro.mes_nascimento, cadastro.ano_nascimento);
    fprintf(file, "Idade: %d\n", idade);
    fprintf(file, "Situação: %s\n", situacao);
    fprintf(file, "Horario de Entrada: %02d:%02d\n", cadastro.horarioEntradaHora, cadastro.horarioEntradaMinuto);
    fprintf(file, "-----------------------------\n");

    fclose(file);
}

void visualizarRelatorios() {
    FILE *file = fopen("informacoes_paciente.txt", "r");
    if (file == NULL) {
        printf("Nenhum relatório encontrado!\n");
        return;
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);
}

int loginEnfermeira() {
    char username[50], password[50];

    printf("Digite o nome de usuário: ");
    scanf("%s", username);
    printf("Digite a senha: ");
    scanf("%s", password);

    if (strcmp(username, "enfermeira") == 0 && strcmp(password, "1234") == 0) {
        return 1;
    } else {
        printf("Login falhou! Usuário ou senha incorretos.\n");
        return 0;
    }
}

void menuEnfermeira() {
    int opcaoEnfermeira;

    do {
        printf("----- Área da Enfermeira -----\n");
        printf("1. Visualizar Relatórios\n");
        printf("2. Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcaoEnfermeira);

        switch (opcaoEnfermeira) {
            case 1:
                visualizarRelatorios();
                break;
            case 2:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    } while (opcaoEnfermeira != 2);
}

void iniciarPreTriagem() {
    struct Cadastro cadastro;
    int continuar = 1;

    while (continuar) {
        system("cls");

        printf("Digite seu nome completo: ");
        fgets(cadastro.nome, sizeof(cadastro.nome), stdin);

        printf("Digite o CPF (SEM PONTUAÇÕES): ");
        scanf("%s", cadastro.cpf);

        printf("Digite sua data de nascimento (DD MM AAAA): ");
        scanf("%d %d %d", &cadastro.dia_nascimento, &cadastro.mes_nascimento, &cadastro.ano_nascimento);

        printf("Digite o horário de chegada do Paciente (HH:MM): ");
        scanf("%d : %d", &cadastro.horarioEntradaHora, &cadastro.horarioEntradaMinuto);
        
        system("cls");

        int idade = calcularIdade(cadastro.dia_nascimento, cadastro.mes_nascimento, cadastro.ano_nascimento);

        char situacao[30];

        if (idade < 20) {
            int p1, p2, p3, p4, p5, pr;

            printf("\nResponda as perguntas a seguir com 1 para sim ou 0 para não.\n");
            printf("\nRisco de vida?\n");
            scanf("%i", &p1);

            if (p1 == 1) {
                printf("Emergência\n");
                strcpy(situacao, "Emergência");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            }

            printf("Vias respiratórias trancadas?\n");
            scanf("%i", &p2);

            if (p2 == 1) {
                printf("Emergência\n");
                strcpy(situacao, "Emergência");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            }

            printf("Respiração ineficaz (falta de ar)?\n");
            scanf("%i", &p3);
            printf("Choque?\n");
            scanf("%i", &p4);
            printf("Criança não responde a estímulos físicos (belisco)?\n");
            scanf("%i", &p5);

            pr = p1 + p2 + p3 + p4;

            if (pr >= 2) {
                printf("Emergência\n");
                strcpy(situacao, "Emergência");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            } else if (pr == 1) {
                printf("Muito urgente\n");
                strcpy(situacao, "Muito urgente");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            }

            printf("Dor severa (Insuportável)?\n");
            scanf("%i", &p1);
            printf("Grande hemorragia (sangramento) incontrolável?\n");
            scanf("%i", &p2);
            printf("Alteração do nível de consciência?\n");
            scanf("%i", &p3);
            printf("Criança quente?\n");
            scanf("%i", &p4);

            pr = p1 + p2 + p3 + p4;

            if (pr >= 2) {
                printf("Muito urgente\n");
                strcpy(situacao, "Muito urgente");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            } else if (pr == 1) {
                printf("Urgente\n");
                strcpy(situacao, "Urgente");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            }

            printf("Dor moderada?\n");
            scanf("%i", &p1);
            printf("Pequena hemorragia (sangramento) incontrolável?\n");
            scanf("%i", &p2);
            printf("Vômitos persistentes?\n");
            scanf("%i", &p3);
            printf("Quente?\n");
            scanf("%i", &p4);
            printf("Crise asmática?\n");
            scanf("%i", &p5);

            pr = p1 + p2 + p3 + p4 + p5;

            if (pr >= 2) {
                printf("Urgente\n");
                strcpy(situacao, "Urgente");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            } else if (pr == 1) {
                printf("Pouco urgente\n");
                strcpy(situacao, "Pouco urgente");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            }

            printf("Dor?\n");
            scanf("%i", &p1);
            printf("Febril?\n");
            scanf("%i", &p2);
            printf("Vômito?\n");
            scanf("%i", &p3);
            printf("Problema recente?\n");
            scanf("%i", &p4);

            pr = p1 + p2 + p3 + p4;

            if (pr >= 1) {
                printf("Pouco urgente\n");
                strcpy(situacao, "Pouco urgente");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            } else {
                printf("Não urgente\n");
                strcpy(situacao, "Não urgente");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            }

        } else {
            int p6, p7, p8, p9, p10, pr2;

            printf("\nResponda as perguntas a seguir com 1 para sim ou 0 para não.\n");
            printf("\nRisco de vida?\n");
            scanf("%i", &p6);

            if (p6 == 1) {
                printf("Emergência\n");
                strcpy(situacao, "Emergência");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            }

            printf("Vias respiratórias trancadas?\n");
            scanf("%i", &p7);

            if (p7 == 1) {
                printf("Emergência\n");
                strcpy(situacao, "Emergência");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            }

            printf("Respiração ineficaz (falta de ar\n");
            scanf("%i", &p8);
            printf("Choque?\n");
            scanf("%i", &p9);
            printf("Criança não responde a estímulos físicos (belisco)?\n");
            scanf("%i", &p10);

            pr2 = p6 + p7 + p8 + p9;

            if (pr2 >= 2) {
                printf("Emergência\n");
                strcpy(situacao, "Emergência");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            } else if (pr2 == 1) {
                printf("Muito urgente\n");
                strcpy(situacao, "Muito urgente");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            }

            printf("Dor severa?\n");
            scanf("%i", &p6);
            printf("Grande hemorragia (sangramento) incontrolável?\n");
            scanf("%i", &p7);
            printf("Alteração do nível de consciência?\n");
            scanf("%i", &p8);
            printf("Criança quente?\n");
            scanf("%i", &p9);

            pr2 = p6 + p7 + p8 + p9;

            if (pr2 >= 2) {
                printf("Muito urgente\n");
                strcpy(situacao, "Muito urgente");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            } else if (pr2 == 1) {
                printf("Urgente\n");
                strcpy(situacao, "Urgente");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            }

            printf("Dor moderada?\n");
            scanf("%i", &p6);
            printf("Pequena hemorragia (sangramento) incontrolável?\n");
            scanf("%i", &p7);
            printf("Vômitos persistentes?\n");
            scanf("%i", &p8);
            printf("Quente?\n");
            scanf("%i", &p9);
            printf("Crise asmática?\n");
            scanf("%i", &p10);

            pr2 = p6 + p7 + p8 + p9 + p10;

            if (pr2 >= 2) {
                printf("Urgente\n");
                strcpy(situacao, "Urgente");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            } else if (pr2 == 1) {
                printf("Pouco urgente\n");
                strcpy(situacao, "Pouco urgente");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            }

            printf("Dor?\n");
            scanf("%i", &p6);
            printf("Febril?\n");
            scanf("%i", &p7);
            printf("Vômito?\n");
            scanf("%i", &p8);
            printf("Problema recente?\n");
            scanf("%i", &p9);

            pr2 = p6 + p7 + p8 + p9;

            if (pr2 >= 1) {
                printf("Pouco urgente\n");
                strcpy(situacao, "Pouco urgente");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            } else {
                printf("Não urgente\n");
                strcpy(situacao, "Não urgente");
                system("cls");
                salvarInformacoes(cadastro, idade, situacao);
                goto perguntar_novo_cadastro;
            }
        }
        
perguntar_novo_cadastro:
        printf("Deseja cadastrar um novo paciente? (1 - Sim / 0 - Não): ");
        scanf("%d", &continuar);
        fflush(stdin); // Limpar o buffer do teclado

        if (continuar != 1 && continuar != 0) {
            printf("Opção inválida. Digite 1 para Sim ou 0 para Não.\n");
            continuar = 1;
            system("pause");
        }
    }
}

void ordeprioridade(FILE *file1, FILE *file2){
    // Abertura do arquivo de origem "relatorio.txt" em modo de leitura
    FILE *file1 = fopen("informacoes_paciente.txt", "r");
    // Verifica se o arquivo foi aberto com sucesso
    if (file1 == NULL) {
        printf("Não foi possível abrir o arquivo.");
        return 1; // Retorna 1 para indicar falha
    }

    // Abertura do arquivo de destino "ordemprioridade.txt" em modo de escrita
    FILE *file2 = fopen("ordemprioridade.txt", "w");

    // Chamada da função para copiar conteúdo do arquivo1 para arquivo2
    copiarConteudo(file1, file2);

    char emergencia[20]; // Declaração da variável para armazenar a condição de emergência
    int pontuacao; // Declaração da variável para armazenar a pontuação

    // Atribuição da pontuação com base na condição de emergência
    // Nota: Nesta versão, presume-se que as condições de emergência são consistentemente formatadas no arquivo de origem
    if (strcmp(emergencia, "Emergência") == 0) {
        pontuacao = 10;
    } else if (strcmp(emergencia, "Muito Urgente") == 0) {
        pontuacao = 8;
    } else if (strcmp(emergencia, "Urgente") == 0) {
        pontuacao = 6;
    } else if (strcmp(emergencia, "Pouco Urgente") == 0) {
        pontuacao = 4;
    } else if (strcmp(emergencia, "Não Urgente") == 0) {
        pontuacao = 2;
    } else {
        pontuacao = 0; // Atribui 0 se a condição de emergência não for reconhecida
    }

    // Definição do tamanho da lista 
    //i= contagem dos passos;
    //aux= serve para fazer a troca quando necessário;
    //flag= serve como sinalização para quando a sequência estiver totalmente ordenada, indicando quando a ordenação foi concluída;
    //lista = vetor que vai armazenar os dados;

    #define N 50
    int i, aux, flag;
    int lista[N]; // Declaração da lista de inteiros

    // Loop para preencher a lista com valores digitados pelo usuário
    //conjunto de instruções para receber os dados da lista através do teclado. Para isso foi criado uma estrutura de repetição;
    //i<= 49 pois dentro do vetor irá de 0 á 49.
    for (i = 0; i < N; i++) {
        printf("Item %d: ", i + 1);
        scanf("%d", &lista[i]);
    }

    // Algoritmo de ordenação (Bubble Sort) para ordenar a lista em ordem decrescente
    //flag é definido o valor de para a representação de um valor verdadeiro;
    //flag = 1 (não está ordenada), flag = 0 (está ordenada);
    flag = 1;
    while (flag) {
        flag = 0;
        //A lista possui 50 itens, mas o loop vai sempre comparando de dois em dois, ou seja, quando chegar no 49° item comparará com o 50°, tendo que parar a comparação no 49°. Dentro do vetor o 49° item corresponde ao subscrito 48, pois começa em 0 os valores da lista a 49°. 
        for (i = 0; i < N - 1; i++) {
            //ex: if(lista[0]<lista[0+1, ou seja, 1]);
            //< para ordem descrescente 
            if (lista[i] < lista[i + 1]) {
                // Troca de posições se o elemento atual for menor que o próximo
                  //resgatando o primeiro elemento comparado "i" para dentro da variável aux;
                aux = lista[i];
                //atribuindo um novo valor a variável i;
                lista[i] = lista[i + 1];
                //promovi a troca para satisfazer a condição atribuída em if;
                lista[i + 1] = aux;
                  //atribuindo valor verdadeiro (1) para variável flag.
                //como não tem a noção se a lista está devidamente ordenada, por isso se "liga" o flag para que ele possa retornar dentro do while para que ele possa repetir o processo. Se a lista totalmente ordenada, ele não entrará em if, pois estará indicando que nenhum elemento anterior é maior que o próximo
                flag = 1; // Sinaliza que ocorreu uma troca
            }
        }
    }

    // Impressão da lista ordenada na tela
     //mostrar o resultado
    for (i = 0; i < N; i++) {
         //i+1 para indicar o item 1, pois, 0+1 = 1;
        //lista[1] =  valor do item;
        printf("\nItem %d: %d", i + 1, lista[i]);
    }

    // Fechamento dos arquivos
    fclose(file1);
    fclose(file2);

    return 0; // Retorna 0 para indicar sucesso
}

// Implementação da função para copiar conteúdo de file1 para file2
void copiarConteudo(FILE *file1, FILE *file2) {
    char leitor[5000]; // Declaração de um buffer para armazenar os dados lidos do arquivo

    // Loop para ler o conteúdo do arquivo1 e escrever no arquivo2
    while (fgets(leitor, sizeof(leitor), file1) != NULL) {
        fputs(leitor, file2);
    }
}
