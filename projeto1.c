#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_AMOSTRAS 100

void carregar_iniciais(float velocidades[][2], float sensores_frontais[][3], float sensores_laterais[][2], int inicio) {
    for (int i = inicio; i < inicio + 50; i++) {
        velocidades[i][0] = rand() % 121;
        velocidades[i][1] = rand() % 121;

        sensores_frontais[i][0] = (rand() % 1000) / 10.0; 
        sensores_frontais[i][1] = (rand() % 1000) / 10.0;
        sensores_frontais[i][2] = (rand() % 1000) / 10.0;

        sensores_laterais[i][0] = (rand() % 26) / 10.0;
        sensores_laterais[i][1] = (rand() % 26) / 10.0;
    }
}

void fusao_sensores(float sensores_frontais[][3], float processamento[][2], int total_amostras) {
    for (int i = 0; i < total_amostras; i++) {
        float radar = sensores_frontais[i][0];
        float lidar = sensores_frontais[i][1];
        float camera = sensores_frontais[i][2];
        float mediana;

        if ((radar >= lidar && radar <= camera) || (radar <= lidar && radar >= camera)) {
            mediana = radar;
        } else if ((lidar >= radar && lidar <= camera) || (lidar <= radar && lidar >= camera)) {
            mediana = lidar;
        } else {
            mediana = camera; 
        }

        processamento[i][0] = mediana;
    }
}

void distancia_segura_freio(float velocidades[][2], float processamento[][2], float atrito, int sensibilidade, int total_amostras) {
    float tempo_reacao = 1.5; 
    if (sensibilidade == 1) {
        tempo_reacao = 1.0;
    } else if (sensibilidade == 3) {
        tempo_reacao = 2.0;
    }

    for (int i = 0; i < total_amostras; i++) {
        float v_ms = velocidades[i][0] / 3.6;
        float espaco_reacao = v_ms * tempo_reacao;
        float espaco_frenagem = (v_ms * v_ms) / (2.0 * atrito * 9.81);
        
        float d_segura = espaco_reacao + espaco_frenagem;
        processamento[i][1] = d_segura;
    }
}

void analise_risco_frontal(float velocidades[][2], float processamento[][2], int status[][3], int total_amostras) {
    for (int i = 0; i < total_amostras; i++) {
        float v_atual = velocidades[i][0];
        float v_frente = velocidades[i][1];
        float validada = processamento[i][0];
        float segura = processamento[i][1];

        if ((v_atual - v_frente) <= 0) {
            status[i][0] = 0; // Seguro
        } else {
            if (validada >= segura) {
                status[i][0] = 0; // Seguro
            } else if (validada >= (segura * 0.5)) {
                status[i][0] = 1; // Atencao
            } else {
                status[i][0] = 2; // Risco de Colisao - AEB Acionado
            }
        }
    }
}

void assistente_faixa(float velocidades[][2], float sensores_laterais[][2], int status[][3], int total_amostras) {
    for (int i = 0; i < total_amostras; i++) {
        float v_atual = velocidades[i][0];
        float margem = 0.50;
        
        if (v_atual > 80.0) {
            margem = margem + (v_atual - 80.0) * 0.01;
        }

        // Faixa Esquerda (Coluna 1)
        if (sensores_laterais[i][0] < margem) {
            status[i][1] = 2; // Perigo de Invasao
        } else if (sensores_laterais[i][0] < (margem + 0.20)) {
            status[i][1] = 1; // Atencao
        } else {
            status[i][1] = 0; // Normal
        }

        // Faixa Direita (Coluna 2)
        if (sensores_laterais[i][1] < margem) {
            status[i][2] = 2; // Perigo de Invasao
        } else if (sensores_laterais[i][1] < (margem + 0.20)) {
            status[i][2] = 1; // Atencao
        } else {
            status[i][2] = 0; // Normal
        }
    }
}

int main() {
    float velocidades[MAX_AMOSTRAS][2]; 
    float sensores_frontais[MAX_AMOSTRAS][3]; 
    float sensores_laterais[MAX_AMOSTRAS][2]; 
    float processamento[MAX_AMOSTRAS][2]; 
    int status[MAX_AMOSTRAS][3];
    
    int total_amostras = 0;
    float atrito;
    int sensibilidade, opcao;

    srand(time(NULL));

    printf("\n");
    printf("   _____       ____     ____       _             \n");
    printf("  / ___/____ _/ __/____/ __ \\_____(_)   _____   \n");
    printf("  \\__ \\/ __ `/ /_/ _ \\/ / / / ___/ / | / / _ \\ \n");
    printf(" ___/ / /_/ / __/  __/ /_/ / /  / /| |/ /  __/   \n");
    printf("/____/\\__,_/_/  \\___/_____/_/  /_/ |___/\\___/  \n");
    printf("=================================================\n\n");

    printf("Atrito da via: ");
    scanf("%f", &atrito);
    printf("Sensibilidade (1-Esportivo, 2-Normal, 3-Seguro): ");
    scanf("%d", &sensibilidade);

    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Gerar 50 dados aleatorios\n");
        printf("2. Inserir dado manualmente\n");
        printf("3. Processar e ver relatorio\n");
        printf("4. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            if (total_amostras + 50 <= MAX_AMOSTRAS) {
                carregar_iniciais(velocidades, sensores_frontais, sensores_laterais, total_amostras);
                printf("50 dados gerados!\n");
                total_amostras += 50; 
            } else {
                printf("Espaco insuficiente!\n");
            }

        } else if (opcao == 2) {
            if (total_amostras < MAX_AMOSTRAS) {
                printf("Velocidade atual e do carro da frente (km/h): ");
                scanf("%f %f", &velocidades[total_amostras][0], &velocidades[total_amostras][1]);

                printf("Sensores frontais - Radar, Lidar, Camera (metros): ");
                scanf("%f %f %f", &sensores_frontais[total_amostras][0], &sensores_frontais[total_amostras][1], &sensores_frontais[total_amostras][2]);

                printf("Sensores laterais - Esquerda e Direita (metros): ");
                scanf("%f %f", &sensores_laterais[total_amostras][0], &sensores_laterais[total_amostras][1]);

                printf("Dado inserido com sucesso!\n");
                total_amostras += 1; 
            } else {
                printf("Memoria cheia!\n");
            }

        } else if (opcao == 3) {
            if (total_amostras == 0) {
                printf("Nenhum dado cadastrado!\n");
            } else {
                printf("Processando %d amostras...\n", total_amostras);
                
                fusao_sensores(sensores_frontais, processamento, total_amostras);
                distancia_segura_freio(velocidades, processamento, atrito, sensibilidade, total_amostras);
                analise_risco_frontal(velocidades, processamento, status, total_amostras);
                assistente_faixa(velocidades, sensores_laterais, status, total_amostras);
                
                printf("-> Todos os calculos e analises concluídos com sucesso!\n");
            }
        } else if (opcao == 4) {
            printf("Saindo...\n");
        } else {
            printf("Opcao invalida!\n");
        }

    } while (opcao != 4);

    return 0;
}