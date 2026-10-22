#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "monstro.h"


//CONVERSÕES
//DADOS

int converteTextoDado(const char *texto, Dado *dado) {
    // Inicializa com zero, garantindo que expressões sem modificador (ex: "4d10") fiquem corretas
    dado->modificador = 0;

    // O sscanf tenta ler os 3 inteiros e retorna a quantidade de leituras bem-sucedidas
    int lidos = sscanf(texto, "%dd%d%d", &dado->quantidade, &dado->faces, &dado->modificador);

    // Se leu pelo menos a quantidade e as faces, a string era válida
    if (lidos >= 2) {
        return 1; // Sucesso
    }
    return 0; // Erro de formatação
}

//ENUMS

static const char *const nomesFrequencia[] = {
    "MUITO_RARO", "RARO", "INCOMUM", "COMUM", "MUITO_COMUM"
};

static const char *const nomesClima[] = {
    "QUALQUER", "FRIO", "TEMPERADO", "TROPICAL", "ARIDO"
};

static const char *const nomesTerreno[] = {
    "CAVERNA", "RUINAS", "FLORESTA", "MONTANHA", "PLANICIE", "DESERTO",
    "URBANO", "PANTANO"
};

static const char *const nomesOrganizacao[] = {
    "SOLITARIO", "CASAL", "BANDO", "MATILHA", "TRIBO", "CLA", "COLONIA",
    "COMPANHIA", "NINHO", "ENXAME", "GUARDA", "HORDA", "CIRCULO", "CORTE"
};

static const char *const nomesAtividade[] = {
    "DIURNO", "NOTURNO", "QUALQUER_HORARIO"
};

static const char *const nomesDieta[] = {
    "CARNIVORO", "HERBIVORO", "ONIVORO", "PARASITA", "NAO_SE_ALIMENTA"
};

static const char *const nomesEixoOrdem[] = {
    "LEAL", "NEUTRO_ORDEM", "CAOTICO"
};

static const char *const nomesEixoMoral[] = {
    "BOM", "NEUTRO_MORAL", "MAU"
};

static const char *const nomesTipoMovimento[] = {
    "TERRESTRE", "VOO", "NATACAO", "ESCAVACAO"
};

static const char *const nomesManobrabilidade[] = {
    "SEM_MANOBRABILIDADE", "MANOBRA_A", "MANOBRA_B", "MANOBRA_C",
    "MANOBRA_D", "MANOBRA_E"
};

static const char *const nomesTipoEspecial[] = {
    "ESPECIAL_NENHUM", "ESPECIAL_ARMADILHA", "ESPECIAL_FURIA",
    "ESPECIAL_FORMACAO", "ESPECIAL_RASTREAR", "ESPECIAL_EMBOSCADA",
    "ESPECIAL_DERRUBAR", "ESPECIAL_ABRACO", "ESPECIAL_INVESTIDA",
    "ESPECIAL_ORIENTACAO", "ESPECIAL_AGARRAR", "ESPECIAL_VENENO",
    "ESPECIAL_TEIA", "ESPECIAL_MERGULHO", "ESPECIAL_DRENAR_SANGUE",
    "ESPECIAL_DRENAR_NIVEL", "ESPECIAL_DRENAR_FORCA", "ESPECIAL_DISSOLVER",
    "ESPECIAL_PARALISIA", "ESPECIAL_PETRIFICACAO", "ESPECIAL_SOPRO",
    "ESPECIAL_MAGIA", "ESPECIAL_LINGUA", "ESPECIAL_ECOLOCALIZACAO",
    "ESPECIAL_CANTO", "ESPECIAL_CANTO_ABISSAL", "ESPECIAL_MUSICA",
    "ESPECIAL_ARREMESSAR_PEDRA", "ESPECIAL_ARQUEARIA", "ESPECIAL_ODOR",
    "ESPECIAL_INCENDIAR", "ESPECIAL_ENVOLVER", "ESPECIAL_TREMOR",
    "ESPECIAL_REDEMOINHO", "ESPECIAL_CORROSAO", "ESPECIAL_PASSO_ARBOREO",
    "ESPECIAL_REGENERACAO", "ESPECIAL_IMUNIDADE",
    "ESPECIAL_RESISTENCIA_MAGICA", "ESPECIAL_IMATERIAL", "ESPECIAL_MEDO",
    "ESPECIAL_DOENCA", "ESPECIAL_ENCANTO", "ESPECIAL_INVISIBILIDADE",
    "ESPECIAL_ILUSAO", "ESPECIAL_TENACIDADE", "ESPECIAL_CAMUFLAGEM",
    "ESPECIAL_ESCALADA", "ESPECIAL_CARAPACA", "ESPECIAL_VISAO_AGUCADA",
    "ESPECIAL_RESPIRACAO_AQUATICA", "ESPECIAL_PELE_PEDRA",
    "ESPECIAL_TRANSPARENTE", "ESPECIAL_OUTRO"
};

static const char *const nomesTamanho[] = {
    "PEQUENO", "MEDIO", "GRANDE", "ENORME", "GIGANTESCO"
};

static const char *const nomesClassificacaoMoral[] = {
    "MORAL_INSTAVEL", "MORAL_MEDIO", "MORAL_ESTAVEL", "MORAL_CAMPEAO",
    "MORAL_FANATICO"
};

static const char *enumParaTexto(int valor, const char *const nomes[],
                                 size_t quantidade, int valorPadrao) {
    if (valor < 0 || (size_t)valor >= quantidade) {
        valor = valorPadrao;
    }
    return nomes[valor];
}

static int textoParaEnum(const char *texto, const char *const nomes[],
                         size_t quantidade, int valorPadrao) {
    if (texto == NULL) {
        return valorPadrao;
    }

    for (size_t i = 0; i < quantidade; i++) {
        if (strcmp(texto, nomes[i]) == 0) {
            return (int)i;
        }
    }
    return valorPadrao;
}

#define QUANTIDADE_DE(nomes) (sizeof(nomes) / sizeof((nomes)[0]))

const char *frequenciaParaTexto(Frequencia frequencia) {
    return enumParaTexto(frequencia, nomesFrequencia,
                         QUANTIDADE_DE(nomesFrequencia), COMUM);
}

Frequencia textoParaFrequencia(const char *texto) {
    return (Frequencia)textoParaEnum(texto, nomesFrequencia,
                                     QUANTIDADE_DE(nomesFrequencia), COMUM);
}

const char *climaParaTexto(Clima clima) {
    return enumParaTexto(clima, nomesClima, QUANTIDADE_DE(nomesClima), QUALQUER);
}

Clima textoParaClima(const char *texto) {
    return (Clima)textoParaEnum(texto, nomesClima, QUANTIDADE_DE(nomesClima), QUALQUER);
}

const char *terrenoParaTexto(Terreno terreno) {
    return enumParaTexto(terreno, nomesTerreno, QUANTIDADE_DE(nomesTerreno), CAVERNA);
}

Terreno textoParaTerreno(const char *texto) {
    return (Terreno)textoParaEnum(texto, nomesTerreno, QUANTIDADE_DE(nomesTerreno), CAVERNA);
}

const char *organizacaoParaTexto(Organizacao organizacao) {
    return enumParaTexto(organizacao, nomesOrganizacao,
                         QUANTIDADE_DE(nomesOrganizacao), SOLITARIO);
}

Organizacao textoParaOrganizacao(const char *texto) {
    return (Organizacao)textoParaEnum(texto, nomesOrganizacao,
                                      QUANTIDADE_DE(nomesOrganizacao), SOLITARIO);
}

const char *atividadeParaTexto(Atividade atividade) {
    return enumParaTexto(atividade, nomesAtividade,
                         QUANTIDADE_DE(nomesAtividade), QUALQUER_HORARIO);
}

Atividade textoParaAtividade(const char *texto) {
    return (Atividade)textoParaEnum(texto, nomesAtividade,
                                    QUANTIDADE_DE(nomesAtividade), QUALQUER_HORARIO);
}

const char *dietaParaTexto(Dieta dieta) {
    return enumParaTexto(dieta, nomesDieta, QUANTIDADE_DE(nomesDieta), ONIVORO);
}

Dieta textoParaDieta(const char *texto) {
    return (Dieta)textoParaEnum(texto, nomesDieta, QUANTIDADE_DE(nomesDieta), ONIVORO);
}

const char *eixoOrdemParaTexto(EixoOrdem eixo) {
    return enumParaTexto(eixo, nomesEixoOrdem, QUANTIDADE_DE(nomesEixoOrdem),
                         NEUTRO_ORDEM);
}

EixoOrdem textoParaEixoOrdem(const char *texto) {
    return (EixoOrdem)textoParaEnum(texto, nomesEixoOrdem,
                                   QUANTIDADE_DE(nomesEixoOrdem), NEUTRO_ORDEM);
}

const char *eixoMoralParaTexto(EixoMoral eixo) {
    return enumParaTexto(eixo, nomesEixoMoral, QUANTIDADE_DE(nomesEixoMoral),
                         NEUTRO_MORAL);
}

EixoMoral textoParaEixoMoral(const char *texto) {
    return (EixoMoral)textoParaEnum(texto, nomesEixoMoral,
                                   QUANTIDADE_DE(nomesEixoMoral), NEUTRO_MORAL);
}

const char *tipoMovimentoParaTexto(TipoMovimento tipo) {
    return enumParaTexto(tipo, nomesTipoMovimento,
                         QUANTIDADE_DE(nomesTipoMovimento), TERRESTRE);
}

TipoMovimento textoParaTipoMovimento(const char *texto) {
    return (TipoMovimento)textoParaEnum(texto, nomesTipoMovimento,
                                        QUANTIDADE_DE(nomesTipoMovimento), TERRESTRE);
}

const char *manobrabilidadeParaTexto(Manobrabilidade manobrabilidade) {
    return enumParaTexto(manobrabilidade, nomesManobrabilidade,
                         QUANTIDADE_DE(nomesManobrabilidade), SEM_MANOBRABILIDADE);
}

Manobrabilidade textoParaManobrabilidade(const char *texto) {
    return (Manobrabilidade)textoParaEnum(texto, nomesManobrabilidade,
                                         QUANTIDADE_DE(nomesManobrabilidade),
                                         SEM_MANOBRABILIDADE);
}

const char *tipoEspecialParaTexto(TipoEspecial tipo) {
    return enumParaTexto(tipo, nomesTipoEspecial,
                         QUANTIDADE_DE(nomesTipoEspecial), ESPECIAL_NENHUM);
}

TipoEspecial textoParaTipoEspecial(const char *texto) {
    return (TipoEspecial)textoParaEnum(texto, nomesTipoEspecial,
                                       QUANTIDADE_DE(nomesTipoEspecial),
                                       ESPECIAL_NENHUM);
}

const char *tamanhoParaTexto(Tamanho tamanho) {
    return enumParaTexto(tamanho, nomesTamanho, QUANTIDADE_DE(nomesTamanho), MEDIO);
}

Tamanho textoParaTamanho(const char *texto) {
    return (Tamanho)textoParaEnum(texto, nomesTamanho, QUANTIDADE_DE(nomesTamanho), MEDIO);
}

const char *classificacaoMoralParaTexto(ClassificacaoMoral classificacao) {
    return enumParaTexto(classificacao, nomesClassificacaoMoral,
                         QUANTIDADE_DE(nomesClassificacaoMoral), MORAL_MEDIO);
}

ClassificacaoMoral textoParaClassificacaoMoral(const char *texto) {
    return (ClassificacaoMoral)textoParaEnum(texto, nomesClassificacaoMoral,
                                             QUANTIDADE_DE(nomesClassificacaoMoral),
                                             MORAL_MEDIO);
}

#undef QUANTIDADE_DE


//validações e exibição

int validaMonstro(const Monstro *m) {
    if (m == NULL) return 0;

    // ID positivo[cite: 8]
    if (m->id <= 0) return 0;

    // Resistência à magia entre 0 e 100[cite: 8]
    if (m->resistenciaMagia < 0 || m->resistenciaMagia > 100) return 0;

    // Pelo menos um terreno, um movimento e um ataque[cite: 8]
    if (m->quantidadeTerrenos < 1 || m->quantidadeMovimentos < 1 || m->quantidadeAtaques < 1) return 0;

    // Altura mínima não negativa e não superior à altura máxima[cite: 8]
    if (m->alturaMinima < 0 || m->alturaMinima > m->alturaMaxima) return 0;

    // Faixa do covil não negativa e ordenada (mínimo não pode ser maior que máximo)[cite: 8]
    if (m->minimoCovil < 0 || m->minimoCovil > m->maximoCovil) return 0;

    // Opcional: Checar se a quantidade de ataques listada não ultrapassa o máximo do array[cite: 8]
    if (m->quantidadeAtaques > MAX_ATAQUES) return 0;

    return 1; // Se não esbarrou em nenhum IF, o registro é totalmente válido
}


//ficha

void exibeMonstro(const Monstro *m) {
    if (m == NULL) return;

    printf("\n============================================================\n");
    printf("[%03d] %s\n", m->id, m->nome);
    printf("============================================================\n");
    printf("Descricao : %s\n", m->descricao);
    printf("Frequencia: %s\n", frequenciaParaTexto(m->frequencia));
    printf("Ecologia  : %s\n", m->descricaoEcologia);
    printf("Sociedade : %s\n", m->habitatSociedade);
    printf("------------------------------------------------------------\n");
    printf("XP Base   : %d\n", m->valorXP);
    printf("Armadura  : %d\n", m->classeArmadura);

    // Imprime no formato XdY ou XdY+Z
    printf("Dados Vida: %dd%d", m->dadosVida.quantidade, m->dadosVida.faces);
    if (m->dadosVida.modificador != 0) {
        printf("%+d", m->dadosVida.modificador);
    }

    printf("\nTHACO     : %d\n", m->thac0);
    printf("Res. Magia: %d%%\n", m->resistenciaMagia);
    printf("============================================================\n\n");
}