#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// ===== CONSTANTES DE CORES ANSI =====
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define DIM "\033[2m"

// Cores do texto
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// Cores personalizadas
#define TITULO_COR BOLD CYAN
#define MENU_COR BOLD YELLOW
#define SUCESSO_COR BOLD GREEN
#define ERRO_COR BOLD RED
#define INFO_COR BLUE
#define DESTAQUE_COR BOLD MAGENTA

typedef struct
{
    char titulo[100];
    int pag;
    char sinopse[500];
    char inicio[15];
    char termino[15];
    int marc;
} livro;

typedef struct
{
    char nome[100];
    int id;
    char gen[250];
} dados;

void limpar_tela() {
    #ifdef _WIN32
    #else
        system("clear");
    #endif
}

void pausar() {
    printf("\n" DIM "Pressione ENTER para continuar..." RESET);
    getchar();
    getchar();
}

void linha_separadora() {
    printf(CYAN "---------------------------------------------------------------\n" RESET);
}

void cabecalho() {
    limpar_tela();
    printf(TITULO_COR);
    printf("+---------------------------------------------------------------------------------------+\n");
    printf("|                                                                                       |\n");
    printf("|  ___  _  _     _  _       _                       __   __ _       _                _  |\n");
    printf("| | _ )(_)| |__ | |(_) ___ | |_  ___  __  __ _      \\ \\ / /(_) _ _ | |_  _  _  __ _ | | |\n");
    printf("| | _ \\| ||  _ \\| || |/ _ \\|  _|/ -_)/ _|/ _` |      \\   / | || '_||  _|| || |/ _` || | |\n");
    printf("| |___/|_||____/|_||_|\\___/\\__| \\___|\\__|\\__/_|       \\_/  |_||_|   \\__| \\_._|\\__/_||_| |\n");
    printf("|                                                                                       |\n");
    printf("|" YELLOW "                           Sua Biblioteca Digital Pessoal                              " TITULO_COR "|\n");
    printf("|                                                                                       |\n");
    printf("+---------------------------------------------------------------------------------------+\n");
    printf(RESET "\n");
}

void cabecalho_secao(const char* titulo) {
    printf("\n" DESTAQUE_COR "+---------------------------------------------------------------+\n");
    printf("|                         %-29s |\n", titulo);
    printf("+---------------------------------------------------------------+\n" RESET);
}

void exibir_menu() {
    printf(MENU_COR "+---------------------------------------------------------------+\n");
    printf("|                        MENU PRINCIPAL                        |\n");
    printf("+---------------------------------------------------------------+\n");
    printf("| 1 | Cadastrar livro                                         |\n");
    printf("| 2 | Listar livros                                           |\n");
    printf("| 3 | Buscar livro                                            |\n");
    printf("| 4 | Atualizar livro                                         |\n");
    printf("| 5 | Atualizar datas e marcador                              |\n");
    printf("| 6 | Apagar livro                                            |\n");
    printf("| 7 | Salvar lista de livros                                  |\n");
    printf("| 8 | Encerrar programa                                       |\n");
    printf("+---------------------------------------------------------------+\n" RESET);
    printf("\n" BOLD CYAN ">> Digite sua opcao: " RESET);
}

void ler_dados_completos(livro estante[], int *total, dados pessoa[], int *total2)
{
    FILE *arq = fopen("listadelivros.txt", "r");
    if (!arq) return;

    char linha[500];
    *total = 0;
    *total2 = 0;

    while (fgets(linha, sizeof(linha), arq))
    {
        // usuario
        if (strncmp(linha, "Nome:", 5) == 0)
        {
            char *nome = strchr(linha, ':');
            if (nome)
            {
                nome += 2; // pular ": "
                strcpy(pessoa[*total2].nome, nome);
                pessoa[*total2].nome[strcspn(pessoa[*total2].nome, "\n")] = '\0';
            }

            fgets(linha, sizeof(linha), arq);
            sscanf(linha, "Identificacao: %d", &pessoa[*total2].id);

            fgets(linha, sizeof(linha), arq);
            char *generos = strchr(linha, ':');
            if (generos)
            {
                generos += 2;
                strcpy(pessoa[*total2].gen, generos);
                pessoa[*total2].gen[strcspn(pessoa[*total2].gen, "\n")] = '\0';
            }

            (*total2)++;
        }

        // livro
        else if (strncmp(linha, "Livro", 5) == 0)
        {
            char *titulo = strchr(linha, '.');
            if (titulo)
            {
                titulo += 2; // pular ". "
                strcpy(estante[*total].titulo, titulo);
                estante[*total].titulo[strcspn(estante[*total].titulo, "\n")] = '\0';
            }

            fgets(linha, sizeof(linha), arq);
            sscanf(linha, "Numero de paginas: %d", &estante[*total].pag);
            // isso também é novo para mimm

            fgets(linha, sizeof(linha), arq);
            char *sinopse = strchr(linha, ':');
            if (sinopse)
            {
                sinopse += 2;
                strcpy(estante[*total].sinopse, sinopse);
                estante[*total].sinopse[strcspn(estante[*total].sinopse, "\n")] = '\0';
            }

            fgets(linha, sizeof(linha), arq);
            char *inicio = strchr(linha, ':');
            if (inicio)
            {
                inicio += 2;
                strcpy(estante[*total].inicio, inicio);
                estante[*total].inicio[strcspn(estante[*total].inicio, "\n")] = '\0';
            }

            fgets(linha, sizeof(linha), arq);
            char *termino = strchr(linha, ':');
            if (termino)
            {
                termino += 2;
                strcpy(estante[*total].termino, termino);
                estante[*total].termino[strcspn(estante[*total].termino, "\n")] = '\0';
            }

            fgets(linha, sizeof(linha), arq);
            sscanf(linha, "Pagina marcada: %d", &estante[*total].marc);

            (*total)++;

            if (*total >= 100) break;
        }
    }

    fclose(arq);
}

void cadastrar_usuario(dados pessoa[], int *total2)
{
    cabecalho();

    if (*total2 == 20)
    {
        printf(ERRO_COR "Capacidade maxima de usuarios atingida.\n" RESET);
        pausar();
        return;
    }

    cabecalho_secao("CADASTRO DE USUARIO");

    printf(INFO_COR "Nome: " RESET);
    scanf(" %[^\n]", pessoa[*total2].nome);

    printf(INFO_COR "Identificacao: " RESET);
    scanf("%d", &pessoa[*total2].id);

    printf(INFO_COR "Generos favoritos: " RESET);
    scanf(" %[^\n]", pessoa[*total2].gen);

    linha_separadora();
    printf(SUCESSO_COR "Seja bem-vindo(a) a sua biblioteca pessoal, %s :D!\n" RESET, pessoa[*total2].nome);
    // ;D
    linha_separadora();

    (*total2)++;
    pausar();
}

void cadastrarlivro(livro estante[], int *total)
{
    limpar_tela();

    if (*total >= 100)
    {
        printf(ERRO_COR "Limite de livros atingido!\n" RESET);
        pausar();
        return;
    }

    cabecalho_secao("CADASTRO DE LIVRO");

    printf(INFO_COR ">> Insira o titulo da obra:\n   > " RESET);
    scanf(" %[^\n]", estante[*total].titulo);

    printf(INFO_COR "\n>> Insira a quantidade de paginas:\n   > " RESET);
    scanf("%d", &estante[*total].pag);

    printf(INFO_COR "\n>> Insira uma breve sinopse do livro:\n   > " RESET);
    scanf(" %[^\n]", estante[*total].sinopse);

    printf(INFO_COR "\n>> Insira a data de inicio da leitura (00/00/0000 se nao comecou):\n   > " RESET);
    scanf(" %[^\n]", estante[*total].inicio);

    printf(INFO_COR "\n>> Insira a data de termino da leitura (00/00/0000 se nao terminou):\n   > " RESET);
    scanf(" %[^\n]", estante[*total].termino);

    printf(INFO_COR "\n>> Insira a pagina marcada:\n   > " RESET);
    scanf("%d", &estante[*total].marc);

    (*total)++;

    linha_separadora();
    printf(SUCESSO_COR "Livro cadastrado com sucesso! :D\n" RESET);
    linha_separadora();
    pausar();
}

void listarlivros(livro estante[], int *total)
{
    limpar_tela();
    cabecalho_secao("LISTA DE LIVROS");

    int esc;
    FILE *arq = fopen("listadelivros.txt", "r");

    if (!arq && *total == 0)
    {
        printf(ERRO_COR "Nenhum livro foi cadastrado! :(\n" RESET);
        pausar();
        return;
    }

    if (arq != NULL)
    {
        printf(INFO_COR "Voce deseja abrir a lista salva no programa passado ou listar os livros agora cadastrados?\n");
        printf("(Insira 1 para a lista antiga ou 2 para a lista atual): " RESET);
        scanf("%d", &esc);

        if (esc == 1)
        {
            limpar_tela();
            cabecalho();
            printf(TITULO_COR "** A seguir, todos os seus livros anteriormente cadastrados:\n\n" RESET);

            char linha[500];
            while (fgets(linha, sizeof(linha), arq))
            {
                printf("%s", linha);
            }
        }
        fclose(arq);
    }
    else
    {
        esc = 2;
    }

    if (esc == 2)
    {
        limpar_tela();
        cabecalho();
        printf(TITULO_COR "** A seguir seus livros agora cadastrados:\n\n" RESET);

        if (*total == 0)
        {
            printf(YELLOW "Nenhum livro cadastrado nesta sessao.\n" RESET);
        }
        else
        {
            for (int i = 0; i < *total; i++)
            {
                printf(BOLD CYAN "*%d. %s\n" RESET, i + 1, estante[i].titulo);
                printf("Numero de paginas: " YELLOW "%d\n" RESET, estante[i].pag);
                printf("Sinopse: %s\n", estante[i].sinopse);
                printf("Inicio da leitura: " GREEN "%s\n" RESET, estante[i].inicio);
                printf("Termino da leitura: " GREEN "%s\n" RESET, estante[i].termino);
                printf("Pagina marcada: " MAGENTA "%d\n\n" RESET, estante[i].marc);
            }
        }
    }

    pausar();
}

void buscarlivro(livro estante[], int *total)
{
    limpar_tela();
    cabecalho_secao("BUSCAR LIVRO");

    char busca[100];
    printf(INFO_COR "Insira o titulo do livro:\n   > " RESET);
    scanf(" %[^\n]", busca);

    for (int i = 0; i < *total; i++)
    {
        if (strcmp(estante[i].titulo, busca) == 0)
        {
            printf(SUCESSO_COR "\nLivro encontrado:\n" RESET);

            printf(BOLD CYAN "*%d. %s\n" RESET, i + 1, estante[i].titulo);
            printf("Numero de paginas: " YELLOW "%d\n" RESET, estante[i].pag);
            printf("Sinopse: %s\n", estante[i].sinopse);
            printf("Inicio da leitura: " GREEN "%s\n" RESET, estante[i].inicio);
            printf("Termino da leitura: " GREEN "%s\n" RESET, estante[i].termino);
            printf("Pagina marcada: " MAGENTA "%d\n" RESET, estante[i].marc);

            pausar();
            return;
        }
    }

    printf(ERRO_COR "\nLivro nao encontrado.\n" RESET);
    pausar();
}

void atualizarlivro(livro estante[], int *total)
{
    limpar_tela();
    cabecalho_secao("ATUALIZAR LIVRO");

    char busca[100];
    printf(INFO_COR "Insira o titulo do livro a atualizar:\n   > " RESET);
    scanf(" %[^\n]", busca);

    for (int i = 0; i < *total; i++)
    {
        if (strcmp(estante[i].titulo, busca) == 0)
        {
            printf(INFO_COR "\nNovo titulo:\n   > " RESET);
            scanf(" %[^\n]", estante[i].titulo);

            printf(INFO_COR "\nNova quantidade de paginas:\n   > " RESET);
            scanf("%d", &estante[i].pag);

            printf(INFO_COR "\n>> Nova sinopse do livro:\n   > " RESET);
            scanf(" %[^\n]", estante[i].sinopse);

            printf(INFO_COR "\nNova data de inicio:\n   > " RESET);
            scanf(" %[^\n]", estante[i].inicio);

            printf(INFO_COR "\nNova data de termino:\n   > " RESET);
            scanf(" %[^\n]", estante[i].termino);

            printf(INFO_COR "\nNova pagina marcada:\n   > " RESET);
            scanf("%d", &estante[i].marc);

            linha_separadora();
            printf(SUCESSO_COR "Livro atualizado com sucesso!\n" RESET);
            linha_separadora();
            pausar();
            return;
        }
    }

    printf(ERRO_COR "\nLivro nao encontrado.\n" RESET);
    pausar();
}

void atualizarrapido(livro estante[], int *total)
{
    limpar_tela();
    cabecalho_secao("ATUALIZAR DATAS E MARCADOR");

    char busca[100];
    printf(INFO_COR "Insira o titulo do livro:\n   > " RESET);
    scanf(" %[^\n]", busca);

    for (int i = 0; i < *total; i++)
    {
        if (strcmp(estante[i].titulo, busca) == 0)
        {
            printf(INFO_COR "\nNova data de inicio:\n   > " RESET);
            scanf(" %[^\n]", estante[i].inicio);

            printf(INFO_COR "\nNova data de termino:\n   > " RESET);
            scanf(" %[^\n]", estante[i].termino);

            printf(INFO_COR "\nNova pagina marcada:\n   > " RESET);
            scanf("%d", &estante[i].marc);

            linha_separadora();
            printf(SUCESSO_COR "Atualizacao rapida concluida!\n" RESET);
            linha_separadora();
            pausar();
            return;
        }
    }

    printf(ERRO_COR "\nLivro nao encontrado.\n" RESET);
    pausar();
}

void apagar_livro(livro estante[], int *total)
{
    limpar_tela();
    cabecalho_secao("APAGAR LIVRO");

    char busca[100];
    printf(INFO_COR "Insira o titulo do livro que deseja apagar:\n   > " RESET);
    scanf(" %[^\n]", busca);

    for (int i = 0; i < *total; i++)
    {
        if (strcmp(estante[i].titulo, busca) == 0)
        {
            for (int j = i; j < *total - 1; j++)
            {
                estante[j] = estante[j + 1];
            }

            (*total)--;
            linha_separadora();
            printf(SUCESSO_COR "Livro apagado com sucesso!\n" RESET);
            linha_separadora();
            pausar();
            return;
        }
    }

    printf(ERRO_COR "\nLivro nao encontrado. Tente outro titulo.\n" RESET);
    pausar();
}

void salvar_lista(livro estante[], dados pessoa[], int *total, int *total2)
{
    limpar_tela();
    cabecalho_secao("SALVAR LISTA DE LIVROS");

    int esc, esc1;

    printf(INFO_COR "Deseja manter a ultima lista salva? (1 para sim ou 2 para nao): " RESET);
    scanf("%d", &esc);

    FILE *arq = fopen("listadelivros.txt", "w");

    if (arq == NULL)
    {
        printf(ERRO_COR "\nErro ao abrir o arquivo.\n" RESET);
        pausar();
        return;
    }

    printf(INFO_COR "Deseja salvar a lista com seus dados? (1 para sim ou 2 para nao): " RESET);
    scanf("%d", &esc1);

    printf(YELLOW "\nSalvando sua biblioteca...\n" RESET);

    if (esc1 == 1)
    {
        fprintf(arq, "+---------------------------------------------------------------+\n");
        fprintf(arq, "|                        DADOS DO USUARIO                      |\n");
        fprintf(arq, "+---------------------------------------------------------------+\n");

        for (int i = 0; i < *total2; i++)
        {
            fprintf(arq, "Nome: %s\n", pessoa[i].nome);
            fprintf(arq, "Identificacao: %d\n", pessoa[i].id);
            fprintf(arq, "Generos preferidos: %s\n\n", pessoa[i].gen);
        }
    }

    fprintf(arq, "+---------------------------------------------------------------+\n");
    fprintf(arq, "|                   A seguir sua ultima lista                   |\n");
    fprintf(arq, "+---------------------------------------------------------------+\n\n");

    for (int i = 0; i < *total; i++)
    {
        fprintf(arq, "Livro %d. %s\n", i + 1, estante[i].titulo);
        fprintf(arq, "Numero de paginas: %d paginas\n", estante[i].pag);
        fprintf(arq, "Sinopse: %s\n", estante[i].sinopse);
        fprintf(arq, "Inicio da leitura: %s\n", estante[i].inicio);
        fprintf(arq, "Termino da leitura: %s\n", estante[i].termino);
        fprintf(arq, "Pagina marcada: %d\n\n", estante[i].marc);
    }

    fclose(arq);
    linha_separadora();
    printf(SUCESSO_COR "Lista salva com sucesso!\n" RESET);
    linha_separadora();
    pausar();

    if (esc == 2)
    {
        *total = 0;
        *total2 = 0;
        memset(estante, 0, 100 * sizeof(livro));
        memset(pessoa, 0, 20 * sizeof(dados));
    }
}

int main()
{
    setlocale(LC_ALL, "Portuguese_Brasil");

    livro *estante = malloc(100 * sizeof(livro));
    dados *pessoa = malloc(20 * sizeof(dados));

    if (estante == NULL || pessoa == NULL)
    {
        printf(ERRO_COR "Erro ao alocar memoria.\n" RESET);
        return 1;
    }

    int total = 0, total2 = 0, opcao;
    ler_dados_completos(estante, &total, pessoa, &total2);

    cadastrar_usuario(pessoa, &total2);

    do
    {
        limpar_tela();
        cabecalho();
        exibir_menu();
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            cadastrarlivro(estante, &total);
            break;
        case 2:
            listarlivros(estante, &total);
            break;
        case 3:
            buscarlivro(estante, &total);
            break;
        case 4:
            atualizarlivro(estante, &total);
            break;
        case 5:
            atualizarrapido(estante, &total);
            break;
        case 6:
            apagar_livro(estante, &total);
            break;
        case 7:
            salvar_lista(estante, pessoa, &total, &total2);
            break;
        case 8:
            limpar_tela();
            printf(TITULO_COR "+---------------------------------------------------------------+\n");
            printf("|                                                               |\n");
            printf("|              Obrigado por usar nossa biblioteca!              |\n");
            printf("|                   Ate a proxima leitura!                     |\n");
            printf("|                                                               |\n");
            printf("+---------------------------------------------------------------+\n" RESET);

            free(estante);
            free(pessoa);
            estante = NULL;
            pessoa = NULL;
            break;
        default:
            printf(ERRO_COR "\nOpcao invalida. Tente novamente.\n" RESET);
            pausar();
        }
    } while (opcao != 8);

    return 0;
}

