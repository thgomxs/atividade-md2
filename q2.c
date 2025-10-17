// Questão 2 - “Chaves Periódicas”
// Eduardo Henrique Colacio de Sales - 232014290
// Thiago Gomes Pereira de Abreu - 232014333

#include <stdio.h>
typedef long long ll;

// Calcula o MDC utilizando o método de Euclides printando as etapas
ll mdc(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    printf("\n==============\n\nCalculo do mdc(%lld, %lld) pelo Algoritmo de Euclides:\n\n", a, b);
    while (b != 0) {
        ll q = a / b;
        ll r = a % b;
        printf("  %lld = %lld * %lld + %lld\n", a, b, q, r);
        a = b;
        b = r;
    }
    printf("-> mdc = %lld\n\n", a);
    printf("==============\n\n", a, b);
    return a;
}

// Calcula o MMC das chaves utilizando o MDC e printando as etapas
int mmc(int a, int b) {
    int resultado = (a * b) / mdc(a, b);
    printf("\n==============\n\nCalculo do mmc(%d, %d):\n\n", a, b);
    printf("mmc = (%d * %d) / mdc(%d, %d)\n\n", a, b, a, b);
    printf("-> mmc = %d\n\n", resultado);
    printf("==============\n\n", a, b);
    return resultado;
}

int main() {
    int N;

    printf("\n===== Questao 02 - Chaves Periodicas =====\n\n");

    printf("Digite o numero de chaves (1 <= N <= 10): ");
    if (scanf("%d", &N) != 1) {printf("Entrada invalida\n"); return 1; }

    if (N < 1 || N > 10 ) {
        printf("Erro: insira um numero de 1 a 10.\n");
        return 1;
    }

    printf("Insira as %d chaves: ", N);


    int ciclos[10];
    for (int i = 0; i < N; i++) {
        scanf("%d", &ciclos[i]);
    }

    // É realizado o cálculo do MMC de todas as chaves
    int resultado = ciclos[0];
    for (int i = 1; i < N; i++) {
        resultado = mmc(resultado, ciclos[i]);
    }

    // Ao fim, caso o MMC seja menor que 50 e maior que 0, é impresso o ano em que as chaves se sincronizam
    // Caso contrário, é informado que não é possível sincronizar em até 50 anos
    if (resultado > 50 || resultado < 1) {
        printf("Nao e possivel sincronizar dentro de 50 anos.\n");
    } else {
        printf("Resultado Final: ano %d\n", resultado);
    }

    printf("\n===== FIM DO CALCULO =====\n");

    return 0;
}
