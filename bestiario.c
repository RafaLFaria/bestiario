#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bestiario.h"

/* Prepara o bestiário zerando o contador e limpando os slots */
void inicializaBestiario(Bestiario *b) {
    if (b != NULL) {
        b->quantidade = 0;
        for (int i = 0; i < MAX_MONSTROS; i++) {
            b->monstros[i].id = 0;
            b->monstros[i].quantidadeTerrenos = 0;
            b->monstros[i].quantidadeMovimentos = 0;
            b->monstros[i].quantidadeAtaques = 0;
        }
    }
}

/* Limpa os espaços em branco sobrando nas pontas de uma string (bem útil para ler arquivos) */
static char *trim(char *str) {
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    char *end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

/* Lê o arquivo de texto bestiario.txt linha por linha e popula o struct */
int carregaBestiario(Bestiario *b, const char *nomeArquivo) {
    FILE *f = fopen(nomeArquivo, "r");
    if (!f) return 0; // Deu ruim ao abrir o arquivo
    
    inicializaBestiario(b);
    char linha[1024];
    int dentroMonstro = 0;
    Monstro atual;
    memset(&atual, 0, sizeof(Monstro));

    while (fgets(linha, sizeof(linha), f) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0'; // Tira o \n do final
        char *linhaTrim = trim(linha);

        /* Pula linhas em branco ou comentários que começam com # */
        if (linhaTrim[0] == '\0' || linhaTrim[0] == '#') continue;

        if (strcmp(linhaTrim, "[MONSTRO]") == 0) {
            dentroMonstro = 1;
            memset(&atual, 0, sizeof(Monstro));
            continue;
        }
        
        if (strcmp(linhaTrim, "[/MONSTRO]") == 0) {
            if (dentroMonstro) {
                // Passou na validação? Então guarda no vetor do bestiário!
                if (validaMonstro(&atual)) {
                    if (b->quantidade < MAX_MONSTROS) {
                        b->monstros[b->quantidade++] = atual;
                    }
                }
                dentroMonstro = 0;
            }
            continue;
        }

        /* Se estamos dentro de um bloco de monstro, lemos chave=valor */
        if (dentroMonstro) {
            char *igual = strchr(linhaTrim, '=');
            if (igual != NULL) {
                *igual = '\0';
                char *chave = trim(linhaTrim);
                char *valor = trim(igual + 1);

                if (strcmp(chave, "id") == 0) atual.id = atoi(valor);

                else if (strcmp(chave, "nome") == 0) strncpy(atual.nome, valor, sizeof(atual.nome) - 1);

                else if (strcmp(chave, "descricao") == 0) strncpy(atual.descricao, valor, sizeof(atual.descricao) - 1);

                else if (strcmp(chave, "frequencia") == 0) atual.frequencia = textoParaFrequencia(valor);

                else if (strcmp(chave, "numeroEncontrado") == 0) converteTextoDado(valor, &atual.numeroEncontrado);

                else if (strcmp(chave, "covil") == 0) sscanf(valor, "%d,%d", &atual.minimoCovil, &atual.maximoCovil);

                else if (strcmp(chave, "terrenos") == 0) {
                    char *token = strtok(valor, ",");
                    while (token != NULL && atual.quantidadeTerrenos < MAX_TERRENOS) {
                        atual.terrenos[atual.quantidadeTerrenos++] = textoParaTerreno(trim(token));
                        token = strtok(NULL, ",");
                    }
                }
                
                else if (strcmp(chave, "atividade") == 0) atual.atividade = textoParaAtividade(valor);

                else if (strcmp(chave, "dieta") == 0) atual.dieta = textoParaDieta(valor);

                else if (strcmp(chave, "inteligencia") == 0) atual.inteligencia = atoi(valor);

                else if (strcmp(chave, "tesouro") == 0) strncpy(atual.tesouro, valor, sizeof(atual.tesouro) - 1);

                else if (strcmp(chave, "ordem") == 0) atual.tendencia.ordem = textoParaEixoOrdem(valor);

                else if (strcmp(chave, "moralidade") == 0) atual.tendencia.moral = textoParaEixoMoral(valor);

                else if (strcmp(chave, "ca") == 0) atual.classeArmadura = atoi(valor);

                else if (strcmp(chave, "dv") == 0) converteTextoDado(valor, &atual.dadosVida);

                else if (strcmp(chave, "thac0") == 0) atual.thac0 = atoi(valor);

                else if (strcmp(chave, "resistenciaMagia") == 0) atual.resistenciaMagia = atoi(valor);

                else if (strcmp(chave, "altura") == 0) sscanf(valor, "%f,%f", &atual.alturaMinima, &atual.alturaMaxima);
                
                else if (strcmp(chave, "moral") == 0) atual.moral = atoi(valor);

                else if (strcmp(chave, "xp") == 0) atual.valorXP = atoi(valor);
            }
        }
    }
    fclose(f);
    return b->quantidade > 0 ? 1 : 0;
}

/* Grava os monstros atuais em um arquivo de saída */
int salvaBestiario(const Bestiario *b, const char *nomeArquivo) {
    if (b == NULL) return 0;
    FILE *f = fopen(nomeArquivo, "w");
    if (!f) return 0;

    for (int i = 0; i < b->quantidade; i++) {
        const Monstro *m = &b->monstros[i];
        fprintf(f, "[MONSTRO]\n");
        fprintf(f, "id=%d\n", m->id);
        fprintf(f, "nome=%s\n", m->nome);
        fprintf(f, "[/MONSTRO]\n\n");
    }
    fclose(f);
    return 1;
}

/* Insere monstro novo no final, cuidando pra não repetir ID */
int insereMonstro(Bestiario *b, Monstro monstro) {
    if (b == NULL || b->quantidade >= MAX_MONSTROS) return 0;
    if (!validaMonstro(&monstro)) return 0;
    
    for (int i = 0; i < b->quantidade; i++) {
        if (b->monstros[i].id == monstro.id) return 0; // ID duplicado bloqueado!
    }
    
    b->monstros[b->quantidade++] = monstro;
    return 1;
}

/* Funções de busca por ID (retornando ponteiro normal ou constante) e por nome */
Monstro *buscaMonstroPorId(Bestiario *b, int id) {
    if (b == NULL) return NULL;
    for (int i = 0; i < b->quantidade; i++) {
        if (b->monstros[i].id == id) return &b->monstros[i];
    }
    return NULL;
}

const Monstro *consultaMonstroPorId(const Bestiario *b, int id) {
    if (b == NULL) return NULL;
    for (int i = 0; i < b->quantidade; i++) {
        if (b->monstros[i].id == id) return &b->monstros[i];
    }
    return NULL;
}

Monstro *buscaMonstroPorNome(Bestiario *b, const char *nome) {
    if (b == NULL || nome == NULL) return NULL;
    for (int i = 0; i < b->quantidade; i++) {
        if (strcasecmp(b->monstros[i].nome, nome) == 0) return &b->monstros[i];
    }
    return NULL;
}

/* Altera os dados de um monstro já existente */
int alteraMonstro(Bestiario *b, int id, Monstro novosDados) {
    if (b == NULL || !validaMonstro(&novosDados)) return 0;
    for (int i = 0; i < b->quantidade; i++) {
        if (b->monstros[i].id == id) {
            b->monstros[i] = novosDados;
            return 1;
        }
    }
    return 0;
}

/* Remove um monstro e puxa os outros pro lado pra manter o vetor contíguo */
int removeMonstro(Bestiario *b, int id) {
    if (b == NULL) return 0;
    int pos = -1;
    for (int i = 0; i < b->quantidade; i++) {
        if (b->monstros[i].id == id) { 
            pos = i; 
            break; 
        }
    }
    if (pos == -1) return 0; // Não achou

    for (int i = pos; i < b->quantidade - 1; i++) {
        b->monstros[i] = b->monstros[i + 1];
    }
    b->quantidade--;
    return 1;
}

/* Mostra uma listagem rápida com ID e nome de todo mundo */
void listaMonstros(const Bestiario *b) {
    if (b == NULL || b->quantidade == 0) {
        printf("Bestiario vazio.\n");
        return;
    }
    for (int i = 0; i < b->quantidade; i++) {
        printf("[%03d] %s\n", b->monstros[i].id, b->monstros[i].nome);
    }
}

/* Consultas ecológicas para filtros */
int viveNoTerreno(const Monstro *m, Terreno terreno) {
    if (m == NULL) return 0;
    for (int i = 0; i < m->quantidadeTerrenos; i++) {
        if (m->terrenos[i] == terreno) return 1;
    }
    return 0;
}

int ativoNoPeriodo(const Monstro *m, Atividade periodo) {
    if (m == NULL) return 0;
    return (m->atividade == periodo || m->atividade == QUALQUER_HORARIO);
}

int listaMonstrosPorTerreno(const Bestiario *b, Terreno terreno, Monstro *resultado[], int capacidadeResultado) {
    if (b == NULL || resultado == NULL) return 0;
    int count = 0;
    for (int i = 0; i < b->quantidade && count < capacidadeResultado; i++) {
        if (viveNoTerreno(&b->monstros[i], terreno)) {
            resultado[count++] = &b->monstros[i];
        }
    }
    return count;
}