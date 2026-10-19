#include <stdio.h>

static void exibeMenu(void)
{
    printf("\n=== Bestiario ===\n");
    printf("1 - Consultar monstro por ID\n");
    printf("2 - Consultar monstro por nome\n");
    printf("3 - Cadastrar monstro\n");
    printf("4 - Alterar monstro\n");
    printf("5 - Remover monstro\n");
    printf("6 - Listar monstros\n");
    printf("7 - Filtrar por terreno\n");
    printf("8 - Filtrar por periodo de atividade\n");
    printf("9 - Salvar bestiario\n");
    printf("0 - Encerrar\n");
    printf("Escolha uma opcao: ");
}

int main(void)
{
    int opcao;

    do
    {
        exibeMenu();

        if (scanf("%d", &opcao) != 1)
        {
            int caractere;

            while ((caractere = getchar()) != '\n' && caractere != EOF)
            {
            }

            if (caractere == EOF)
            {
                break;
            }

            printf("Entrada invalida. Digite um numero entre 0 e 9.\n");
            continue;
        }

        switch (opcao)
        {
        case 0:
            printf("Encerrando o bestiario.\n");
            break;
        case 1:
            printf("Consultar monstro por ID.\n");
            break;
        case 2:
            printf("Consultar monstro por nome.\n");
            break;
        case 3:
            printf("Cadastrar monstro.\n");
            break;
        case 4:
            printf("Alterar monstro.\n");
            break;
        case 5:
            printf("Remover monstro.\n");
            break;
        case 6:
            printf("Listar monstros.\n");
            break;
        case 7:
            printf("Filtrar por terreno.\n");
            break;
        case 8:
            printf("Filtrar por periodo de atividade.\n");
            break;
        case 9:
            printf("Salvar bestiario.\n");
            break;
        default:
            printf("Opcao invalida. Digite um numero entre 0 e 9.\n");
            break;
        }
    } while (opcao != 0);

    return 0;
}