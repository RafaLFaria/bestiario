#ifndef BESTIARIO_H
#define BESTIARIO_H

#include "monstro.h"

#define MAX_MONSTROS 100

/* Struct do Bestiário: guarda o vetor de monstros e quantos já foram cadastrados */
typedef struct
{
    Monstro monstros[MAX_MONSTROS];
    int quantidade;
} Bestiario;

/* Funções principais para gerenciar o bestiário e os arquivos */
void inicializaBestiario(Bestiario *b);
int carregaBestiario(Bestiario *b, const char *nomeArquivo);
int salvaBestiario(const Bestiario *b, const char *nomeArquivo);

/* Operações do mini-CRUD */
int insereMonstro(Bestiario *b, Monstro monstro);
Monstro *buscaMonstroPorId(Bestiario *b, int id);
const Monstro *consultaMonstroPorId(const Bestiario *b, int id);
Monstro *buscaMonstroPorNome(Bestiario *b, const char *nome);
int alteraMonstro(Bestiario *b, int id, Monstro novosDados);
int removeMonstro(Bestiario *b, int id);
void listaMonstros(const Bestiario *b);

/* Funções de consultas ecológicas exigidas */
int viveNoTerreno(const Monstro *m, Terreno terreno);
int ativoNoPeriodo(const Monstro *m, Atividade periodo);
int listaMonstrosPorTerreno(
    const Bestiario *b,
    Terreno terreno,
    const Monstro *resultado[],
    int capacidadeResultado
);

#endif