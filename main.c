#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bestiario.h"

/* Mostra o menu bonitinho no terminal de 0 a 9 */
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
    Bestiario b;
    inicializaBestiario(&b);

    /* Carrega o arquivo fornecido automaticamente ao iniciar */
    if (carregaBestiario(&b, "bestiario.txt")) {
        printf("Bestiario carregado com sucesso! Total de criaturas: %d\n", b.quantidade);
    } else {
        printf("Aviso: Nao foi possivel carregar 'bestiario.txt' automaticamente.\n");
    }

    int opcao;
    do
    {
        exibeMenu();
        if (scanf("%d", &opcao) != 1)
        {
            int caractere;
            while ((caractere = getchar()) != '\n' && caractere != EOF) {}
            if (caractere == EOF) break;
            printf("Entrada invalida. Digite um numero entre 0 e 9.\n");
            continue;
        }

        switch (opcao)
        {
        case 0:
            printf("Encerrando o bestiario.\n");
            break;
            
        case 1: {
            int id;

            printf("Digite o ID do monstro: ");
            scanf("%d", &id);

            const Monstro *m = consultaMonstroPorId(&b, id);

            if (m != NULL) {
                exibeMonstro(m);
            } else {
                printf("Monstro com ID %d nao encontrado.\n", id);
            }
            break;
        }
        
        case 2: {
            char nome[50];

            printf("Digite o nome do monstro: ");
            scanf("%49s", nome);

            Monstro *m = buscaMonstroPorNome(&b, nome);
            if (m != NULL) {
                exibeMonstro(m);
            } else {
                printf("Monstro '%s' nao encontrado.\n", nome);
            }
            break;
        }
        
        case 3: {
            Monstro novo;
            memset(&novo, 0, sizeof(Monstro));

            printf("ID do novo monstro: ");
            scanf("%d", &novo.id);

            printf("Nome: ");
            scanf("%49s", novo.nome);

            strcpy(novo.descricao, "Criatura cadastrada manualmente.");

            novo.frequencia = COMUM;
            novo.quantidadeTerrenos = 1;
            novo.terrenos[0] = CAVERNA;
            novo.quantidadeMovimentos = 1;
            novo.movimentos[0].tipo = TERRESTRE;
            novo.movimentos[0].deslocamento = 6;
            novo.quantidadeAtaques = 1;
            strcpy(novo.ataques[0].nome, "GOLPE");
            novo.ataques[0].dano.quantidade = 1;
            novo.ataques[0].dano.faces = 6;
            novo.dadosVida.quantidade = 1;
            novo.dadosVida.faces = 8;
            novo.classeArmadura = 7;
            novo.thac0 = 20;
            novo.resistenciaMagia = 0;
            novo.alturaMinima = 1.0f;
            novo.alturaMaxima = 2.0f;
            novo.moral = 10;
            novo.valorXP = 15;

            if (insereMonstro(&b, novo)) {
                printf("Monstro cadastrado com sucesso!\n");
            } else {
                printf("Falha ao cadastrar monstro (ID duplicado ou dados invalidos).\n");
            }
            break;
        }
        
        case 4: {
            int id;
            printf("Digite o ID do monstro a alterar: ");
            scanf("%d", &id);

            Monstro *m = buscaMonstroPorId(&b, id);
            if (m == NULL) {
                printf("Monstro nao encontrado.\n");
                break;
            }
            Monstro alt = *m;

            printf("Novo nome (atual: %s): ", alt.nome);
            scanf("%49s", alt.nome);

            printf("Nova CA (atual: %d): ", alt.classeArmadura);
            scanf("%d", &alt.classeArmadura);

            if (alteraMonstro(&b, id, alt)) {
                printf("Monstro alterado com sucesso!\n");
            } else {
                printf("Falha ao alterar monstro.\n");
            }
            break;
        }
        
        case 5: {
            int id;

            printf("Digite o ID do monstro a remover: ");
            scanf("%d", &id);

            if (removeMonstro(&b, id)) {
                printf("Monstro removido com sucesso!\n");
            } else {
                printf("Monstro nao encontrado.\n");
            }
            break;
        }
        
        case 6:
            listaMonstros(&b);
            break;
            
        case 7: {
            int terr;

            printf("Escolha o terreno (0: CAVERNA, 1: RUINAS, 2: FLORESTA, 3: MONTANHA, 4: PLANICIE, 5: DESERTO, 6: URBANO, 7: PANTANO): ");
            scanf("%d", &terr);

            Monstro *res[MAX_MONSTROS];
            int qtd = listaMonstrosPorTerreno(&b, (Terreno)terr, res, MAX_MONSTROS);
            
            printf("Monstros encontrados no terreno: %d\n", qtd);
            
            for (int i = 0; i < qtd; i++) {
                printf("- [%03d] %s\n", res[i]->id, res[i]->nome);
            }
            break;
        }
        
        case 8: {
            int ativ;

            printf("Escolha o periodo (0: DIURNO, 1: NOTURNO, 2: QUALQUER_HORARIO): ");
            scanf("%d", &ativ);

            printf("Monstros ativos no periodo:\n");

            for (int i = 0; i < b.quantidade; i++) {
                if (ativoNoPeriodo(&b.monstros[i], (Atividade)ativ)) {
                    printf("- [%03d] %s\n", b.monstros[i].id, b.monstros[i].nome);
                }
            }
            break;
        }
        
        case 9:
            if (salvaBestiario(&b, "bestiario-saida.txt")) {
                printf("Bestiario salvo com sucesso em 'bestiario-saida.txt'!\n");
            } else {
                printf("Erro ao salvar o bestiario.\n");
            }
            break;
            
        default:
            printf("Opcao invalida. Digite um numero entre 0 e 9.\n");
            break;
        }
    } while (opcao != 0);

    return 0;
}