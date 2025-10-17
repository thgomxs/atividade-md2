// Questão 1 - Sistema RSA com Fatoração ρ de Pollard e Aplicação de Teoremas Modulares em Três Etapas.
// Eduardo Henrique Colacio de Sales - 232014290
// Thiago Gomes Pereira de Abreu - 232014333

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef long long ll;


// Calcula o MDC utilizando o método de Euclides
ll mdc(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    printf("Calculo do mdc(%lld, %lld) pelo Algoritmo de Euclides:\n", a, b);
    while (b != 0) {
        ll q = a / b;
        ll r = a % b;
        printf("  %lld = %lld * %lld + %lld\n", a, b, q, r);
        a = b;
        b = r;
    }
    printf("-> mdc = %lld\n\n", a);
    return a;
}
// Calcula o MDC utilizando o método de Euclides sem mensagens printadas para a parte da decodificação e codificação
ll mdc_sem_mensagem(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        ll q = a / b;
        ll r = a % b;
        a = b;
        b = r;
    }
    return a;
}



// Euclides Estendido (retorna d = mdc(a,b) e x,y tais que ax + by = d) //
ll emdc(ll a, ll b, ll *x, ll *y) {
    printf("Executando Euclides Estendido para (%lld, %lld):\n", a, b);
    ll old_r = a, r = b;
    ll old_s = 1, s = 0;
    ll old_t = 0, t = 1;
    while (r != 0) {
        ll q = old_r / r;
        ll tmp;
        printf("  q = %lld | %lld = %lld * %lld + %lld\n", q, old_r, q, r, old_r - q * r);
        tmp = r; r = old_r - q * r; old_r = tmp;
        tmp = s; s = old_s - q * s; old_s = tmp;
        tmp = t; t = old_t - q * t; old_t = tmp;
    }
    *x = old_s; *y = old_t;
    printf("Resultado: mdc = %lld, x = %lld, y = %lld  (pois %lld*%lld + %lld*%lld = %lld)\n\n",
           old_r, *x, *y, a, *x, b, *y, old_r);
    return old_r;
}

// Calcula o MMC utilizando o MDC
int mmc(int a, int b) {
    return (a * b) / mdc(a, b);
}

// Verifica se n é primo
int eh_primo(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (int i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
    return 1;
}

// Função g(x) = (x^2 + 1) mod n
 ll g_func(ll x, ll n) {
    // (x*x + 1) % n, evitando overflow com long long (n <= 9999^2)
    return ( ((x * x) + 1 ) % n );
}

// Algoritmo ρ de Pollard para fatoração com prints
ll pollard_rho(ll n) {
    // if (n % 2 == 0) return 2;
    ll x = 2;
    ll y = 2;
    ll d = 1;
    int i = 0;
    while (d == 1) {
        x = g_func(x, n);
        y = g_func(g_func(y, n), n);
        ll diff = x - y; if (diff < 0) diff = -diff;
        d = mdc_sem_mensagem(diff, n);
        printf("=> Pollard Rho (Iteracao %d): x = %lld, y = %lld, |x-y| = %lld, mdc(|x-y|, n) = %lld\n\n", i+1, x, y, diff, d);
        i++;
    }
    if (d == n) return -1; // Falha
    return d;
}


// Inverso modular (a^-1 mod m) usando Euclides Estendido
// Retorna -1 se inverso não existir
ll modinv(ll a, ll m) {
    ll x, y;
    ll d = emdc(a, m, &x, &y);
    if (d != 1) {
        printf("Inverso modular nao existe: mdc(%lld, %lld) = %lld\n\n", a, m, d);
        return -1;
    }
    ll inv = x % m;
    if (inv < 0) inv += m;
    printf("Inverso modular: %lld (pois %lld * %lld congruente a 1 mod %lld)\n\n", inv, a, inv, m);
    return inv;
}

// Pré-codificação: A=11 ... Z=36 ; Espaço = 00 
int precodifica(char c) {
    if (c == ' ') return 0; // 00
    if (c >= 'A' && c <= 'Z') {
        return 11 + (c - 'A'); // A->11
    }
    if (c >= 'a' && c <= 'z') {
        return 11 + (c - 'a'); // tratamos minusculas igual
    }
    // Pontuação e outros => ignorar ou tratar como espaço
    return 0;
}

// Decodifica o codigo para caractere (assume dois dígitos)
char decode_code(int code) {
    if (code == 0) return ' ';
    if (code >= 11 && code <= 36) {
        return 'A' + (code - 11);
    }
    return '?';
}

// Redução de expoente segundo "teorema" escolhido (com prints) //
ll reduzir_expoente_segurado(ll M, ll E, ll N, ll phiN, char *metodo_aplicado) {
    // Decide e aplica redução, retorna expoente reduzido e escreve metodo em metodo_aplicado
    // Verifica primeiro se N é primo (Fermat), depois se mdc(M,N)=1 (Euler), caso contrário realiza a Divisão Euclidiana
    if (eh_primo(N)) {
        strcpy(metodo_aplicado, "Fermat");
        ll r = E % (N - 1);
        printf("FERMAT => N = %lld é primo -> aplicando Pequeno Teorema de Fermat: usar E mod (N-1)\n", N);
        printf("FERMAT =>  E = %lld ≡ %lld (mod %lld)\n\n", E, r, N-1);
        return r;
    } else if (mdc_sem_mensagem(M, N) == 1) {
        strcpy(metodo_aplicado, "Euler");
        ll r = E % phiN;
        printf("TEOREMA DE EULER => mdc(M, N) = 1 -> aplicando Teorema de Euler: usar E mod z(n)\n");
        printf("TEOREMA DE EULER =>  z(n) = %lld\n", phiN);
        printf("TEOREMA DE EULER =>  E = %lld congruente a %lld (mod %lld)\n\n", E, r, phiN);
        return r;
    } else {
        strcpy(metodo_aplicado, "Divisao Euclidiana");
        ll q = E / N;
        ll r = E % N;
        if (r < 0) { r += N; q -= 1; }
        printf("DIVISAO EUCLIDIANA => Nenhuma condicao anterior -> aplicando Divisao Euclidiana para reduzir expoente\n");
        printf("DIVISAO EUCLIDIANA => E = %lld = %lld * %lld + %lld  => usar expoente reduzido r = %lld\n\n", E, N, q, r, r);
        return r;
    }
}

// Exponenciação modular com print dos passos (binário)
ll modexp(ll base, ll expo, ll mod) {
    printf("== Exponenciacao modular: %lld^%lld mod %lld ==\n", base, expo, mod);
    base %= mod;
    ll resultado = 1;
    ll b = base;
    ll e = expo;
    int step = 0;
    while (e > 0) {
        step++;
        printf("=>  passo %d: base = %lld, expoente = %lld, resultado = %lld\n", step, b, e, resultado);
        if (e & 1) {
            resultado = resultado * b % mod;
            printf("=>    bit = 1 -> resultado = resultado * base mod %lld = %lld\n", mod, resultado);
        } else {
            printf("=>    bit = 0 -> resultado mantem = %lld\n", resultado);
        }
        e >>= 1;
        b = b * b % mod;
        printf("=>    base -> base^2 mod %lld = %lld\n", mod, b);
    }
    printf("=> Exponenciacao modular (resultado final) = %lld\n\n", resultado);
    return resultado;
}

int main(void){
  printf("\n===== Questao 1 - Sistema RSA com Fatoracao p de Pollard e Aplicacao de Teoremas Modulares em Tres Etapas =====\n\n");

  printf("\n===== Etapa 1: Fatoracao Interativa (Metodo p de Pollard) =====\n\n");

  printf("Insira dois numeros (N1 e N2)\n");
  printf("Cada Ni deve ter 3 ou 4 digitos: 100 ate 9999\n");
  printf("Cada Ni deve ser produto de primos distintos para que o metodo p de Pollard seja eficiente.\n\n");

  int N1, N2;
  printf("Digite N1: ");
  if (scanf("%d", &N1) != 1) {printf("Entrada invalida\n"); return 1; }
  printf("Digite N2: ");
  if (scanf("%d", &N2) != 1) { printf("Entrada invalida\n"); return 1; }

  if (N1 < 100 || N1 > 9999 || N2 < 100 || N2 > 9999) {
      printf("Erro: Cada Ni deve ter 3 ou 4 digitos (100 ate 9999)\n");
      return 1;
  }

  printf("\n=== Encontrando fator p de N1 ===\n\n");
  // Descobrir o fator p de N1
  ll p = pollard_rho(N1);
  if (p == -1) { printf("Pollard falhou para N1\n"); return 1; }
  printf("==> Fator p de N1 encontrado = %lld\n\n", p);

  printf("\n=== Encontrando fator q de N2 ===\n\n");
  // Descobrir o fator q de N2
  ll q = pollard_rho(N2);
  if (q == -1) { printf("Pollard falhou para N2\n"); return 1; }
  printf("==> Fator q de N2 encontrado = %lld\n\n", q);


  printf("\n===== Etapa 2: Geracao das Chaves RSA =====\n\n");

  ll n = p * q;
  ll z = (p - 1) * (q - 1);
  printf("Calculo do modulo: n = p * q = %lld * %lld = %lld\n", p, q, n);
  printf("Totiente de Euler: z(n) = (p-1) x (q-1) = %lld\n\n", z);

  printf("\n=== Escolhendo um E > 1 e E < n tal que mdc(E, z(n)) = 1 ===\n\n");
  // Escolher menor E > 1 e E < n tal que mdc(E, z(n)) = 1
  ll E = 2;
  while (E < n) {
      if (mdc(E, z) == 1) break;
      E++;
  }
  if (E >= n) { printf("Nao foi possivel encontrar E adequado\n"); return 1; }
  printf("Expoente publico escolhido: E = %lld (menor E > 1 com mdc(E, z(n)) = 1)\n\n", E);

  // Calcular D = modinv(E, z(n)) usando Euclides Estendido
  ll D = modinv(E, z);
  if (D == -1) { printf("Nao foi possivel calcular D\n"); return 1; }

  printf("Chave publica: (n = %lld, e = %lld)\n", n, E);
  printf("Chave privada: (n = %lld, d = %lld)\n\n", n, D);

  // Limpar newline antes de fgets
  getchar();

  printf("\n===== Etapa 3 - Codificacao (Criptografia) e Decodificacao (Descriptografia) =====\n\n");

  printf("Insira a mensagem (apenas letras e espacos; pontuacao sera tratada como espaco):\n");
  char mensagem[1024];
  if (!fgets(mensagem, sizeof(mensagem), stdin)) { printf("Erro leitura\n"); return 1; }
  // retirar newline
  size_t len = strlen(mensagem);
  if (len && mensagem[len-1] == '\n') mensagem[len-1] = 0;

  // Pré-codificar
  printf("\n=== Pre-codificacao (A=11 ... Z=36, espaco=00): ===\n\n");
  int codes[1024];
  int codes_len = 0;
  for (size_t i = 0; i < strlen(mensagem); ++i) {
      int code = precodifica(mensagem[i]);
      codes[codes_len++] = code;
      printf("  '%c' -> %02d\n", mensagem[i] ? mensagem[i] : ' ', code);
  }
  printf("\n");

  // Verificar que cada bloco M < n (aqui bloco = 2 digitos -> 0..36)
  for (int i = 0; i < codes_len; ++i) {
      if (codes[i] >= n) {
          printf("Erro: codigo %d >= n (%lld). N muito pequeno para cifrar blocos de 2 digitos.\n", codes[i], n);
          return 1;
      }
  }


   // Codificação: C = M^E mod n
    ll Ciphers[1024];
    printf("=== Codificacao (cada bloco M = 2 digitos) ===\n\n");
    for (int i = 0; i < codes_len; ++i) {
        ll M = codes[i];
        char metodo[64] = "";
        printf("== Bloco %d: M = %02lld \n\n", i+1, M);
        // reduzir expoente conforme regras
        ll E_reduz = reduzir_expoente_segurado(M, E, n, z, metodo);
        printf("Metodo aplicado para reduzir expoente: %s\n", metodo);
        // expoencial modular com steps
        ll C = modexp(M, E_reduz, n);
        Ciphers[i] = C;
        printf("=> Cifrado: C = %lld\n\n", C);
    }

    // Decodificação: M' = C^D mod n
    ll Decoded[1024];
    printf("\n=== Decodificacao ===\n\n");
    for (int i = 0; i < codes_len; ++i) {
        ll C = Ciphers[i];
        char metodo[64] = "";
        printf("== Bloco %d: C = %lld \n\n", i+1, C);
        ll D_reduz = reduzir_expoente_segurado(C, D, n, z, metodo);
        printf("=> Metodo aplicado para reduzir expoente: %s\n\n", metodo);
        ll Mprime = modexp(C, D_reduz, n);
        Decoded[i] = Mprime;
        printf("=> Decifrado M' = %lld\n\n", Mprime);
    }

    // Reconversao para texto
    char mensagem_decod[1024];
    for (int i = 0; i < codes_len; ++i) {
        mensagem_decod[i] = decode_code((int)Decoded[i]);
    }
    mensagem_decod[codes_len] = 0;

    printf("\n=== Resultado Final ===\n");
    printf("Mensagem original: \"%s\"\n", mensagem);
    printf("Mensagem decifrada: \"%s\"\n", mensagem_decod);
    if (strcmp(mensagem, mensagem_decod) == 0) {
        printf("Confirma: a mensagem decifrada e identica a original.\n");
    } else {
        printf("Aviso: a mensagem decifrada NAO identica a original.\n");
    }

    printf("\n===== FIM DO CALCULO =====\n");

  return 0;
}