#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
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
    if (!f) return 0;
    
    inicializaBestiario(b);
    char linha[2048];
    int dentroMonstro = 0;
    Monstro atual;
    
    while (fgets(linha, sizeof(linha), f) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        char *linhaTrim = trim(linha);

        if (linhaTrim[0] == '\0' || linhaTrim[0] == '#') continue;

        if (strcmp(linhaTrim, "[MONSTRO]") == 0) {
            dentroMonstro = 1;
            memset(&atual, 0, sizeof(Monstro));
            continue;
        }
        
        if (strcmp(linhaTrim, "[/MONSTRO]") == 0) {
            if (dentroMonstro) {
                if (validaMonstro(&atual)) {
                    if (b->quantidade < MAX_MONSTROS) {
                        b->monstros[b->quantidade++] = atual;
                    } else {
                        printf("Aviso: Limite maximo de monstros atingido.\n");
                    }
                } else {
                    printf("Aviso: Falha na validacao do monstro ID %d (%s).\n", atual.id, atual.nome);
                }
                dentroMonstro = 0;
            }
            continue;
        }

        if (dentroMonstro) {
            char *igual = strchr(linhaTrim, '=');
            if (igual != NULL) {
                *igual = '\0';
                char *chave = trim(linhaTrim);
                char *valor = trim(igual + 1);

                /* Identificadores básicos e Ecologia */
                if (strcmp(chave, "id") == 0) atual.id = atoi(valor);
                else if (strcmp(chave, "nome") == 0) strncpy(atual.nome, valor, sizeof(atual.nome) - 1);
                else if (strcmp(chave, "descricao") == 0) strncpy(atual.descricao, valor, sizeof(atual.descricao) - 1);
                else if (strcmp(chave, "frequencia") == 0) atual.frequencia = textoParaFrequencia(valor);
                else if (strcmp(chave, "numeroEncontrado") == 0) converteTextoDado(valor, &atual.numeroEncontrado);
                else if (strcmp(chave, "covil") == 0) sscanf(valor, "%d,%d", &atual.minimoCovil, &atual.maximoCovil);
                else if (strcmp(chave, "climas") == 0) {
                    char *token = strtok(valor, ",");
                    while (token != NULL && atual.quantidadeClimas < MAX_CLIMAS) {
                        atual.climas[atual.quantidadeClimas++] = textoParaClima(trim(token));
                        token = strtok(NULL, ",");
                    }
                }
                else if (strcmp(chave, "terrenos") == 0) {
                    char *token = strtok(valor, ",");
                    while (token != NULL && atual.quantidadeTerrenos < MAX_TERRENOS) {
                        atual.terrenos[atual.quantidadeTerrenos++] = textoParaTerreno(trim(token));
                        token = strtok(NULL, ",");
                    }
                }
                else if (strcmp(chave, "organizacao") == 0) atual.organizacao = textoParaOrganizacao(valor);
                else if (strcmp(chave, "atividade") == 0) atual.atividade = textoParaAtividade(valor);
                else if (strcmp(chave, "dieta") == 0) atual.dieta = textoParaDieta(valor);
                else if (strcmp(chave, "inteligencia") == 0) atual.inteligencia = atoi(valor);
                else if (strcmp(chave, "tesouro") == 0) strncpy(atual.tesouro, valor, sizeof(atual.tesouro) - 1);
                else if (strcmp(chave, "ordem") == 0) atual.tendencia.ordem = textoParaEixoOrdem(valor);
                else if (strcmp(chave, "moralidade") == 0) atual.tendencia.moral = textoParaEixoMoral(valor);
                
                /* Atributos de Combate */
                else if (strcmp(chave, "ca") == 0) atual.classeArmadura = atoi(valor);
                else if (strcmp(chave, "movimentos") == 0) {
                    char *token = valor;
                    while (token != NULL && *token != '\0' && atual.quantidadeMovimentos < MAX_MOVIMENTOS) {
                        char *pipe = strchr(token, '|');
                        if (pipe) *pipe = '\0';
                        
                        char *colon1 = strchr(token, ':');
                        if (colon1) {
                            *colon1 = '\0';
                            atual.movimentos[atual.quantidadeMovimentos].tipo = textoParaTipoMovimento(trim(token));
                            char *colon2 = strchr(colon1 + 1, ':');
                            if (colon2) {
                                *colon2 = '\0';
                                atual.movimentos[atual.quantidadeMovimentos].deslocamento = atoi(trim(colon1 + 1));
                                char bufManobra[30];
                                snprintf(bufManobra, sizeof(bufManobra), "MANOBRA_%s", trim(colon2 + 1));
                                atual.movimentos[atual.quantidadeMovimentos].manobrabilidade = textoParaManobrabilidade(bufManobra);
                            } else {
                                atual.movimentos[atual.quantidadeMovimentos].deslocamento = atoi(trim(colon1 + 1));
                                atual.movimentos[atual.quantidadeMovimentos].manobrabilidade = SEM_MANOBRABILIDADE;
                            }
                            atual.quantidadeMovimentos++;
                        }
                        if (pipe) token = pipe + 1; else token = NULL;
                    }
                }
                else if (strcmp(chave, "dv") == 0) converteTextoDado(valor, &atual.dadosVida);
                else if (strcmp(chave, "thac0") == 0) atual.thac0 = atoi(valor);
                else if (strcmp(chave, "ataques") == 0) {
                    char *token = valor;
                    while (token != NULL && *token != '\0' && atual.quantidadeAtaques < MAX_ATAQUES) {
                        char *pipe = strchr(token, '|');
                        if (pipe) *pipe = '\0';
                        
                        char *colon = strchr(token, ':');
                        if (colon) {
                            *colon = '\0';
                            strncpy(atual.ataques[atual.quantidadeAtaques].nome, trim(token), 39);
                            converteTextoDado(trim(colon + 1), &atual.ataques[atual.quantidadeAtaques].dano);
                            atual.quantidadeAtaques++;
                        }
                        if (pipe) token = pipe + 1; else token = NULL;
                    }
                }
                else if (strcmp(chave, "ataquesEspeciais") == 0 || strcmp(chave, "defesasEspeciais") == 0) {
                    int isAtaque = (strcmp(chave, "ataquesEspeciais") == 0);
                    int *qtd = isAtaque ? &atual.quantidadeAtaquesEspeciais : &atual.quantidadeDefesasEspeciais;
                    HabilidadeEspecial *hab = isAtaque ? atual.ataquesEspeciais : atual.defesasEspeciais;
                    char *token = valor;
                    
                    if (strcmp(trim(valor), "NENHUM") != 0 && strcmp(trim(valor), "NENHUMA") != 0) {
                        while (token != NULL && *token != '\0' && *qtd < MAX_ESPECIAIS) {
                            char *pipe = strchr(token, '|');
                            if (pipe) *pipe = '\0';
                            
                            char *c1 = strchr(token, ':');
                            if (c1) {
                                *c1 = '\0';
                                char bufTipo[60];
                                snprintf(bufTipo, sizeof(bufTipo), "ESPECIAL_%s", trim(token));
                                hab[*qtd].tipo = textoParaTipoEspecial(bufTipo);
                                
                                char *c2 = strchr(c1 + 1, ':');
                                if (c2) {
                                    *c2 = '\0';
                                    hab[*qtd].valor = atoi(trim(c1 + 1));
                                    strncpy(hab[*qtd].descricao, trim(c2 + 1), sizeof(hab[*qtd].descricao) - 1);
                                } else {
                                    hab[*qtd].valor = atoi(trim(c1 + 1));
                                }
                                (*qtd)++;
                            }
                            if (pipe) token = pipe + 1; else token = NULL;
                        }
                    }
                }
                else if (strcmp(chave, "resistenciaMagia") == 0) atual.resistenciaMagia = atoi(valor);
                else if (strcmp(chave, "tamanho") == 0) atual.tamanho = textoParaTamanho(valor);
                else if (strcmp(chave, "altura") == 0) sscanf(valor, "%f,%f", &atual.alturaMinima, &atual.alturaMaxima);
                else if (strcmp(chave, "moral") == 0) atual.moral = atoi(valor);
                else if (strcmp(chave, "classificacaoMoral") == 0) {
                    char bufMoral[50];
                    snprintf(bufMoral, sizeof(bufMoral), "MORAL_%s", valor);
                    atual.classificacaoMoral = textoParaClassificacaoMoral(bufMoral);
                }
                else if (strcmp(chave, "xp") == 0) atual.valorXP = atoi(valor);
                
                /* Encontros e Poderes */
                else if (strcmp(chave, "poderBase") == 0) atual.poder.poderBase = atoi(valor);
                else if (strcmp(chave, "modDia") == 0) atual.poder.modDia = atoi(valor);
                else if (strcmp(chave, "modNoite") == 0) atual.poder.modNoite = atoi(valor);
                else if (strcmp(chave, "modTerreno") == 0) {
                    char *token = valor;
                    while (token != NULL && *token != '\0' && atual.poder.quantidadeModTerrenos < MAX_TERRENOS) {
                        char *comma = strchr(token, ',');
                        if (comma) *comma = '\0';
                        char *colon = strchr(token, ':');
                        if (colon) {
                            *colon = '\0';
                            atual.poder.modTerreno[atual.poder.quantidadeModTerrenos].terreno = textoParaTerreno(trim(token));
                            atual.poder.modTerreno[atual.poder.quantidadeModTerrenos].modificador = atoi(trim(colon + 1));
                            atual.poder.quantidadeModTerrenos++;
                        }
                        if (comma) token = comma + 1; else token = NULL;
                    }
                }
                else if (strcmp(chave, "bonusPorAdicional") == 0) atual.poder.bonusPorAdicional = atoi(valor);
                else if (strcmp(chave, "limiteGrupoGrande") == 0) atual.poder.limiteGrupoGrande = atoi(valor);
                else if (strcmp(chave, "bonusGrupoGrande") == 0) atual.poder.bonusGrupoGrande = atoi(valor);
                else if (strcmp(chave, "descricaoCombate") == 0) strncpy(atual.descricaoCombate, valor, sizeof(atual.descricaoCombate) - 1);
                else if (strcmp(chave, "habitatSociedade") == 0) strncpy(atual.habitatSociedade, valor, sizeof(atual.habitatSociedade) - 1);
                else if (strcmp(chave, "descricaoEcologia") == 0) strncpy(atual.descricaoEcologia, valor, sizeof(atual.descricaoEcologia) - 1);
            }
        }
    }
    if (dentroMonstro) {
        printf("Aviso: Registro incompleto: faltou [/MONSTRO] para o ID %d (%s).\n",
               atual.id, atual.nome);
    }
    fclose(f);
    return b->quantidade > 0 ? 1 : 0;
}

int salvaBestiario(const Bestiario *b, const char *nomeArquivo) {
    if (b == NULL) return 0;
    FILE *f = fopen(nomeArquivo, "w");
    if (!f) return 0;

    for (int i = 0; i < b->quantidade; i++) {
        const Monstro *m = &b->monstros[i];
        char buffer[50];

        fprintf(f, "[MONSTRO]\n");
        fprintf(f, "id=%d\n", m->id);
        fprintf(f, "nome=%s\n", m->nome);
        fprintf(f, "descricao=%s\n", m->descricao);
        fprintf(f, "frequencia=%s\n", frequenciaParaTexto(m->frequencia));
        
        dadoParaTexto(m->numeroEncontrado, buffer, sizeof(buffer));
        fprintf(f, "numeroEncontrado=%s\n", buffer);
        fprintf(f, "covil=%d,%d\n", m->minimoCovil, m->maximoCovil);
        
        fprintf(f, "climas=");
        if (m->quantidadeClimas == 0) fprintf(f, "QUALQUER");
        for (int j = 0; j < m->quantidadeClimas; j++) {
            fprintf(f, "%s%s", climaParaTexto(m->climas[j]), j < m->quantidadeClimas - 1 ? "," : "");
        }
        fprintf(f, "\n");
        
        fprintf(f, "terrenos=");
        for (int j = 0; j < m->quantidadeTerrenos; j++) {
            fprintf(f, "%s%s", terrenoParaTexto(m->terrenos[j]), j < m->quantidadeTerrenos - 1 ? "," : "");
        }
        fprintf(f, "\n");

        fprintf(f, "organizacao=%s\n", organizacaoParaTexto(m->organizacao));
        fprintf(f, "atividade=%s\n", atividadeParaTexto(m->atividade));
        fprintf(f, "dieta=%s\n", dietaParaTexto(m->dieta));
        fprintf(f, "inteligencia=%d\n", m->inteligencia);
        fprintf(f, "tesouro=%s\n", strlen(m->tesouro) > 0 ? m->tesouro : "NENHUM");
        fprintf(f, "ordem=%s\n", eixoOrdemParaTexto(m->tendencia.ordem));
        fprintf(f, "moralidade=%s\n", eixoMoralParaTexto(m->tendencia.moral));
        fprintf(f, "ca=%d\n", m->classeArmadura);

        fprintf(f, "movimentos=");
        for (int j = 0; j < m->quantidadeMovimentos; j++) {
            fprintf(f, "%s:%d", tipoMovimentoParaTexto(m->movimentos[j].tipo), m->movimentos[j].deslocamento);
            if (m->movimentos[j].manobrabilidade != SEM_MANOBRABILIDADE) {
                const char* man = manobrabilidadeParaTexto(m->movimentos[j].manobrabilidade);
                fprintf(f, ":%s", man + 8); /* Omitir o prefixo "MANOBRA_" */
            }
            if (j < m->quantidadeMovimentos - 1) fprintf(f, "|");
        }
        fprintf(f, "\n");

        dadoParaTexto(m->dadosVida, buffer, sizeof(buffer));
        fprintf(f, "dv=%s\n", buffer);
        fprintf(f, "thac0=%d\n", m->thac0);

        fprintf(f, "ataques=");
        if (m->quantidadeAtaques == 0) fprintf(f, "NENHUM\n");
        for (int j = 0; j < m->quantidadeAtaques; j++) {
            dadoParaTexto(m->ataques[j].dano, buffer, sizeof(buffer));
            fprintf(f, "%s:%s%s", m->ataques[j].nome, buffer, j < m->quantidadeAtaques - 1 ? "|" : "");
        }
        if (m->quantidadeAtaques > 0) fprintf(f, "\n");

        fprintf(f, "ataquesEspeciais=");
        if (m->quantidadeAtaquesEspeciais == 0) fprintf(f, "NENHUM\n");
        else {
            for (int j = 0; j < m->quantidadeAtaquesEspeciais; j++) {
                const char* t = tipoEspecialParaTexto(m->ataquesEspeciais[j].tipo) + 9; /* Omite "ESPECIAL_" */
                fprintf(f, "%s:%d:%s%s", t, m->ataquesEspeciais[j].valor, m->ataquesEspeciais[j].descricao, j < m->quantidadeAtaquesEspeciais - 1 ? "|" : "");
            }
            fprintf(f, "\n");
        }

        fprintf(f, "defesasEspeciais=");
        if (m->quantidadeDefesasEspeciais == 0) fprintf(f, "NENHUMA\n");
        else {
            for (int j = 0; j < m->quantidadeDefesasEspeciais; j++) {
                const char* t = tipoEspecialParaTexto(m->defesasEspeciais[j].tipo) + 9; 
                fprintf(f, "%s:%d:%s%s", t, m->defesasEspeciais[j].valor, m->defesasEspeciais[j].descricao, j < m->quantidadeDefesasEspeciais - 1 ? "|" : "");
            }
            fprintf(f, "\n");
        }

        fprintf(f, "resistenciaMagia=%d\n", m->resistenciaMagia);
        fprintf(f, "tamanho=%s\n", tamanhoParaTexto(m->tamanho));
        fprintf(f, "altura=%.1f,%.1f\n", m->alturaMinima, m->alturaMaxima);
        fprintf(f, "moral=%d\n", m->moral);
        fprintf(f, "classificacaoMoral=%s\n", classificacaoMoralParaTexto(m->classificacaoMoral) + 6); /* Omite "MORAL_" */
        fprintf(f, "xp=%d\n", m->valorXP);
        
        fprintf(f, "poderBase=%d\n", m->poder.poderBase);
        fprintf(f, "modDia=%d\n", m->poder.modDia);
        fprintf(f, "modNoite=%d\n", m->poder.modNoite);
        
        fprintf(f, "modTerreno=");
        for (int j = 0; j < m->poder.quantidadeModTerrenos; j++) {
            fprintf(f, "%s:%d%s", terrenoParaTexto(m->poder.modTerreno[j].terreno), m->poder.modTerreno[j].modificador, j < m->poder.quantidadeModTerrenos - 1 ? "," : "");
        }
        fprintf(f, "\n");
        
        fprintf(f, "bonusPorAdicional=%d\n", m->poder.bonusPorAdicional);
        fprintf(f, "limiteGrupoGrande=%d\n", m->poder.limiteGrupoGrande);
        fprintf(f, "bonusGrupoGrande=%d\n", m->poder.bonusGrupoGrande);
        fprintf(f, "descricaoCombate=%s\n", m->descricaoCombate);
        fprintf(f, "habitatSociedade=%s\n", m->habitatSociedade);
        fprintf(f, "descricaoEcologia=%s\n", m->descricaoEcologia);
        
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

int listaMonstrosPorTerreno(const Bestiario *b, Terreno terreno, const Monstro *resultado[], int capacidadeResultado) {
    if (b == NULL || resultado == NULL) return 0;
    int count = 0;
    for (int i = 0; i < b->quantidade && count < capacidadeResultado; i++) {
        if (viveNoTerreno(&b->monstros[i], terreno)) {
            resultado[count++] = &b->monstros[i];
        }
    }
    return count;
}