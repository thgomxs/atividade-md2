// Questão 3 - “A Razão de Eficiência de um Número”
// Eduardo Henrique Colacio de Sales - 232014290
// Thiago Gomes Pereira de Abreu - 232014333

#include <stdio.h>
#include <math.h>

// Função para calcular potência
long long potencia(long long base, int exp) {
    long long resultado = 1;
    for (int i = 0; i < exp; i++) {
        resultado *= base;
    }
    return resultado;
}

int main() {
    long long N;
    printf("\n===== Questao 03 - Razao de Eficiencia de um Numero =====\n\n");
    
    printf("Digite o numero N (1 <= N <= 105): ");
    if (scanf("%lld", &N) != 1) {printf("Entrada invalida\n"); return 1; }

    if (N < 1 || N > 105 ) {
        printf("Erro: insira um numero de 1 a 105.\n");
        return 1;
    }


    printf("\nIniciando fatoracao prima de %lld...\n", N);
    long long originalN = N;
    long long fatores[100];
    int expoentes[100];
    int qtd = 0;

    // Trial Division (tentativa e erro)
    for (long long p = 2; p * p <= N; p++) {
        if (N % p == 0) {
            int e = 0;
            while (N % p == 0) {
                N /= p;
                e++;
            }
            fatores[qtd] = p;
            expoentes[qtd] = e;
            qtd++;
        }
    }
    if (N > 1) {
        fatores[qtd] = N;
        expoentes[qtd] = 1;
        qtd++;
    }

    printf("\n-> Fatores primos e expoentes:\n");
    if (qtd == 0) {
        printf("   N = 1 (não possui fatoracao prima)\n");
    } else {
        for (int i = 0; i < qtd; i++) {
            printf("   %lld^%d\n", fatores[i], expoentes[i]);
        }
    }

    // Cálculo de tau(N)
    printf("\nCalculando TAU(N): numero de divisores\n");
    long long tau = 1;
    if (qtd == 0) {
        tau = 1;
        printf("   τ(1) = 1 (unico divisor e ele mesmo)\n");
    } else {
        for (int i = 0; i < qtd; i++) {
            int termo = expoentes[i] + 1;
            printf("   Para o fator %lld^%d: (expoente + 1) = %d\n", fatores[i], expoentes[i], termo);
            tau *= termo;
        }
    }
    printf("-> Resultado intermediario: τ(%lld) = %lld\n", originalN, tau);

    // Cálculo de sigma(N)
    printf("\nCalculando SIGMA(N): soma dos divisores\n");
    long long sigma = 1;
    if (qtd == 0) {
        sigma = 1;
        printf("   σ(1) = 1 (unico divisor e ele mesmo)\n");
    } else {
        for (int i = 0; i < qtd; i++) {
            long long p = fatores[i];
            int e = expoentes[i];
            long long numerador = potencia(p, e + 1) - 1;
            long long denominador = p - 1;
            long long termo = numerador / denominador;
            printf("   Para p=%lld, e=%d: ((%lld^(%d+1) - 1) / (%lld - 1)) = %lld\n",
                   p, e, p, e, p, termo);
            sigma *= termo;
        }
    }
    printf("-> Resultado intermediario: sigma(%lld) = %lld\n", originalN, sigma); //σ 

    // Razão de Eficiência
    printf("\nCalculando Razao de Eficiencia R = sigma(N) / τ(N)\n");
    double razao = (double)sigma / (double)tau;
    printf("-> Razao de Eficiencia = %.2lf\n", razao);

    printf("\n===== FIM DO CALCULO =====\n");
    return 0;
}

