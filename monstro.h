#ifndef MONSTRO_H
#define MONSTRO_H

// Limites máximos definidos pelo enunciado para os vetores internos da ficha
#define MAX_CLIMAS 5
#define MAX_TERRENOS 8
#define MAX_MOVIMENTOS 4
#define MAX_ATAQUES 6
#define MAX_ESPECIAIS 6

/* Enums para padronizar as características ecológicas e de combate dos monstros */
typedef enum
{
    MUITO_RARO,
    RARO,
    INCOMUM,
    COMUM,
    MUITO_COMUM
} Frequencia;

typedef enum
{
    QUALQUER,
    FRIO,
    TEMPERADO,
    TROPICAL,
    ARIDO
} Clima;

typedef enum
{
    CAVERNA,
    RUINAS,
    FLORESTA,
    MONTANHA,
    PLANICIE,
    DESERTO,
    URBANO,
    PANTANO
} Terreno;

typedef enum
{
    SOLITARIO,
    CASAL,
    BANDO,
    MATILHA,
    TRIBO,
    CLA,
    COLONIA,
    COMPANHIA,
    NINHO,
    ENXAME,
    GUARDA,
    HORDA,
    CIRCULO,
    CORTE
} Organizacao;

typedef enum
{
    DIURNO,
    NOTURNO,
    QUALQUER_HORARIO
} Atividade;

typedef enum
{
    CARNIVORO,
    HERBIVORO,
    ONIVORO,
    PARASITA,
    NAO_SE_ALIMENTA
} Dieta;

typedef enum
{
    LEAL,
    NEUTRO_ORDEM,
    CAOTICO
} EixoOrdem;

typedef enum
{
    BOM,
    NEUTRO_MORAL,
    MAU
} EixoMoral;

typedef enum
{
    TERRESTRE,
    VOO,
    NATACAO,
    ESCAVACAO
} TipoMovimento;

typedef enum
{
    SEM_MANOBRABILIDADE,
    MANOBRA_A,
    MANOBRA_B,
    MANOBRA_C,
    MANOBRA_D,
    MANOBRA_E
} Manobrabilidade;

typedef enum
{
    ESPECIAL_NENHUM,
    ESPECIAL_ARMADILHA,
    ESPECIAL_FURIA,
    ESPECIAL_FORMACAO,
    ESPECIAL_RASTREAR,
    ESPECIAL_EMBOSCADA,
    ESPECIAL_DERRUBAR,
    ESPECIAL_ABRACO,
    ESPECIAL_INVESTIDA,
    ESPECIAL_ORIENTACAO,
    ESPECIAL_AGARRAR,
    ESPECIAL_VENENO,
    ESPECIAL_TEIA,
    ESPECIAL_MERGULHO,
    ESPECIAL_DRENAR_SANGUE,
    ESPECIAL_DRENAR_NIVEL,
    ESPECIAL_DRENAR_FORCA,
    ESPECIAL_DISSOLVER,
    ESPECIAL_PARALISIA,
    ESPECIAL_PETRIFICACAO,
    ESPECIAL_SOPRO,
    ESPECIAL_MAGIA,
    ESPECIAL_LINGUA,
    ESPECIAL_ECOLOCALIZACAO,
    ESPECIAL_CANTO,
    ESPECIAL_CANTO_ABISSAL,
    ESPECIAL_MUSICA,
    ESPECIAL_ARREMESSAR_PEDRA,
    ESPECIAL_ARQUEARIA,
    ESPECIAL_ODOR,
    ESPECIAL_INCENDIAR,
    ESPECIAL_ENVOLVER,
    ESPECIAL_TREMOR,
    ESPECIAL_REDEMOINHO,
    ESPECIAL_CORROSAO,
    ESPECIAL_PASSO_ARBOREO,
    ESPECIAL_REGENERACAO,
    ESPECIAL_IMUNIDADE,
    ESPECIAL_RESISTENCIA_MAGICA,
    ESPECIAL_IMATERIAL,
    ESPECIAL_MEDO,
    ESPECIAL_DOENCA,
    ESPECIAL_ENCANTO,
    ESPECIAL_INVISIBILIDADE,
    ESPECIAL_ILUSAO,
    ESPECIAL_TENACIDADE,
    ESPECIAL_CAMUFLAGEM,
    ESPECIAL_ESCALADA,
    ESPECIAL_CARAPACA,
    ESPECIAL_VISAO_AGUCADA,
    ESPECIAL_RESPIRACAO_AQUATICA,
    ESPECIAL_PELE_PEDRA,
    ESPECIAL_TRANSPARENTE,
    ESPECIAL_OUTRO
} TipoEspecial;

typedef enum
{
    PEQUENO,
    MEDIO,
    GRANDE,
    ENORME,
    GIGANTESCO
} Tamanho;

typedef enum
{
    MORAL_INSTAVEL,
    MORAL_MEDIO,
    MORAL_ESTAVEL,
    MORAL_CAMPEAO,
    MORAL_FANATICO
} ClassificacaoMoral;

/* Structs auxiliares para dados, movimentos, ataques e tendências */
typedef struct
{
    int quantidade;
    int faces;
    int modificador;
} Dado;

typedef struct
{
    TipoMovimento tipo;
    int deslocamento;
    Manobrabilidade manobrabilidade;
} Movimento;

typedef struct
{
    char nome[40];
    Dado dano;
} Ataque;

typedef struct
{
    TipoEspecial tipo;
    int valor;
    char descricao[120];
} HabilidadeEspecial;

typedef struct
{
    EixoOrdem ordem;
    EixoMoral moral;
} Tendencia;

typedef struct {
    Terreno terreno;
    int modificador;
} ModificadorTerreno;

typedef struct {
    int poderBase;
    int modDia;
    int modNoite;
    ModificadorTerreno modTerreno[MAX_TERRENOS];
    int quantidadeModTerrenos;
    int bonusPorAdicional;
    int limiteGrupoGrande;
    int bonusGrupoGrande;
} PoderEncontro;

/* A struct principal que guarda a ficha inteira de um monstro */
typedef struct
{
    int id;
    char nome[50];
    char descricao[500];
    char descricaoCombate[500];
    char habitatSociedade[500];
    char descricaoEcologia[500];

    Frequencia frequencia;
    Dado numeroEncontrado;
    int minimoCovil, maximoCovil;

    Clima climas[MAX_CLIMAS];
    int quantidadeClimas;
    Terreno terrenos[MAX_TERRENOS];
    int quantidadeTerrenos;

    Organizacao organizacao;
    Atividade atividade;
    Dieta dieta;
    int inteligencia;
    char tesouro[80];
    Tendencia tendencia;

    int classeArmadura;
    Movimento movimentos[MAX_MOVIMENTOS];
    int quantidadeMovimentos;
    Dado dadosVida;
    int thac0;

    Ataque ataques[MAX_ATAQUES];
    int quantidadeAtaques;
    HabilidadeEspecial ataquesEspeciais[MAX_ESPECIAIS];
    int quantidadeAtaquesEspeciais;
    HabilidadeEspecial defesasEspeciais[MAX_ESPECIAIS];
    int quantidadeDefesasEspeciais;

    int resistenciaMagia;
    Tamanho tamanho;
    float alturaMinima, alturaMaxima;
    int moral;
    ClassificacaoMoral classificacaoMoral;
    int valorXP;

    PoderEncontro poder;
} Monstro;

/* Funções públicas do módulo de monstro (conversões, validação e print) */
int converteTextoDado(const char *texto, Dado *dado);
void dadoParaTexto(Dado dado, char *destino, int tamanho);
int validaDado(Dado dado);

const char *frequenciaParaTexto(Frequencia frequencia);
Frequencia textoParaFrequencia(const char *texto);
const char *climaParaTexto(Clima clima);
Clima textoParaClima(const char *texto);
const char *terrenoParaTexto(Terreno terreno);
Terreno textoParaTerreno(const char *texto);
const char *organizacaoParaTexto(Organizacao organizacao);
Organizacao textoParaOrganizacao(const char *texto);
const char *atividadeParaTexto(Atividade atividade);
Atividade textoParaAtividade(const char *texto);
const char *dietaParaTexto(Dieta dieta);
Dieta textoParaDieta(const char *texto);
const char *eixoOrdemParaTexto(EixoOrdem eixo);
EixoOrdem textoParaEixoOrdem(const char *texto);
const char *eixoMoralParaTexto(EixoMoral eixo);
EixoMoral textoParaEixoMoral(const char *texto);
const char *tipoMovimentoParaTexto(TipoMovimento tipo);
TipoMovimento textoParaTipoMovimento(const char *texto);
const char *manobrabilidadeParaTexto(Manobrabilidade manobrabilidade);
Manobrabilidade textoParaManobrabilidade(const char *texto);
const char *tipoEspecialParaTexto(TipoEspecial tipo);
TipoEspecial textoParaTipoEspecial(const char *texto);
const char *tamanhoParaTexto(Tamanho tamanho);
Tamanho textoParaTamanho(const char *texto);
const char *classificacaoMoralParaTexto(ClassificacaoMoral classificacao);
ClassificacaoMoral textoParaClassificacaoMoral(const char *texto);

int validaMonstro(const Monstro *m);
void exibeMonstro(const Monstro *m);

#endif