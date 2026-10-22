
//para cadastros no crud
#define MAX_MONSTROS 100
#include "monstro.h"
typedef struct
{
    Monstro monstros[MAX_MONSTROS];
    int quantidade;
} Bestiario;

void inicializaBestiario(Bestiario *b);
int carregaBestiario(Bestiario *b, const char *nomeArquivo);
int salvaBestiario(const Bestiario *b, const char *nomeArquivo);

//mini crud

int insereMonstro(Bestiario *b, Monstro monstro);
Monstro *buscaMonstroPorId(Bestiario *b, int id);
const Monstro *consultaMonstroPorId(const Bestiario *b, int id);
Monstro *buscaMonstroPorNome(Bestiario *b, const char *nome);
int alteraMonstro(Bestiario *b, int id, Monstro novosDados);
int removeMonstro(Bestiario *b, int id);
void listaMonstros(const Bestiario *b);

//consultas ecologicas

int viveNoTerreno(const Monstro *m, Terreno terreno);
int ativoNoPeriodo(const Monstro *m, Atividade periodo);
int listaMonstrosPorTerreno(
const Bestiario *b,
Terreno terreno,
Monstro *resultado[],
int capacidadeResultado
);