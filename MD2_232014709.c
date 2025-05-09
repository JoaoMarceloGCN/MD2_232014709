    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>

    #ifdef _WIN32  
    #include <windows.h>
    #endif

    // Arrays auxiliares para cálculo do inverso //precisei de ajuda para chegar nesse Raciócinio!!
    int numsA[100], numsB[100], residuos[100], divs[100];

    // Declarando variáveis que vou usar durante o resto do códiggo
    int resultadoFinal, numeroH, moduloZ, geradorG, expoenteX, moduloN, inversoG;

    int calcularMDC(int a, int b) {     // Função para calcular o MDC utilizando Euclides
        int resto = a % b;
        printf("MDC(%d, %d) -> %d\n", a, b, resto);
        if (resto == 0)
            return b;
        return calcularMDC(b, resto);
    }

    int calcularInverso(int a, int mod) {       // Função pra retornar o inverso de um número
        int i = 0, resto = 1, aux = mod;

        while (resto != 0) {
            resto = a % aux;
            numsA[i] = a;
            numsB[i] = aux;
            residuos[i] = resto;
            divs[i] = a / aux;
            a = aux;
            aux = resto;
            i++;
        }

        int x = 0, y = 1, temp;
        for (int j = i - 2; j >= 0; j--) {
            temp = x;
            x = y;
            y = temp - divs[j] * y;
        }

        int inverso = (x % mod + mod) % mod;
        return inverso;
    }

    int multiplicacaoModular(int a, int b, int mod) {       // Multiplicação modular
        printf("\n-> Multiplicação modular: (%d * %d) mod %d\n", a, b, mod);
        return (a * b) % mod;
    }

    int ehPrimo(int n) {        // Testando se primo
        if (n <= 3) return n > 1;
        if (n % 2 == 0 || n % 3 == 0) return 0;

        for (int i = 5; i * i <= n; i += 6)
            if (n % i == 0 || n % (i + 2) == 0) return 0;

        return 1;
    }

    int calcularPhi(int n) {    // Calculando o Fi de N
        int resultado = n;
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                while (n % i == 0)
                    n /= i;
                resultado -= resultado / i;
            }
        }
        if (n > 1)
            resultado -= resultado / n;
        return resultado;
    }

    int potenciaModular(int base, int exp, int mod) {   // Exponenciação modula
        int resultado = 1;
        base %= mod;

        while (exp > 0) {
            if (exp % 2 == 1)
                resultado = (resultado * base) % mod;

            exp /= 2;
            base = (base * base) % mod;
        }

        return resultado;
    }

    int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8); // Codificação para acentuação/ Tive problema com acentos :( e não sabia resolver 
    #endif

        printf("========================================\n");
        printf("      SISTEMA DE CONGRUÊNCIA MODULAR\n");
        printf("========================================\n");

        printf("Insira os valores para resolver a congruência:\n\n");

        printf("H: ");
        scanf("%d", &numeroH);

        printf("G: ");
        scanf("%d", &geradorG);

        printf("Zn: ");
        scanf("%d", &moduloZ);

        printf("x (expoente de G): ");
        scanf("%d", &expoenteX);

        printf("n (módulo final): ");
        scanf("%d", &moduloN);

        printf("\n----------------------------------------\n");
        printf("         CÁLCULO DO INVERSO\n");
        printf("----------------------------------------\n");

        // Verificando se existe inverso
        if (calcularMDC(geradorG, moduloZ) != 1) {
            printf("\nMDC(%d, %d) != 1, então não há inverso.\n", geradorG, moduloZ);
            return 0;
        }

        inversoG = calcularInverso(geradorG, moduloZ);
        printf("O inverso de %d mod %d é %d\n", geradorG, moduloZ, inversoG);

        printf("\n----------------------------------------\n");
        printf("       REDUÇÃO DA BASE H × G⁻¹\n");
        printf("----------------------------------------\n");

        int base = multiplicacaoModular(numeroH, inversoG, moduloZ);
        printf("Base reduzida: %d\n", base);

        printf("\n----------------------------------------\n");
        printf("     VERIFICAÇÃO DE COPRIMIDADE\n");
        printf("----------------------------------------\n");

        int expoenteReducao;
        if (calcularMDC(base, moduloN) == 1) {
            if (ehPrimo(moduloN)) {
                printf("-> Aplicando Pequeno Teorema de Fermat.\n");
                expoenteReducao = moduloN - 1;
            } else {
                printf("-> Aplicando Teorema de Euler.\n");
                expoenteReducao = calcularPhi(moduloN);
            }
        } else {
            printf("%d e %d não são coprimos. Não é possível usar os teoremas.\n", base, moduloN);
            return 0;
        }

        int quociente = expoenteX / expoenteReducao;
        int resto = expoenteX % expoenteReducao;

        printf("\n----------------------------------------\n");
        printf("    CÁLCULO DA EXPONENCIAÇÃO MODULAR\n");
        printf("----------------------------------------\n");

        printf("%d^%d ≡ ((%d^%d)^%d × %d^%d) mod %d\n\n",
            base, expoenteX, base, expoenteReducao, quociente, base, resto, moduloN);

        int potenciaReducao = potenciaModular(base, expoenteReducao, moduloN);
        int parte1 = potenciaModular(potenciaReducao, quociente, moduloN);
        int parte2 = potenciaModular(base, resto, moduloN);

        resultadoFinal = (parte1 * parte2) % moduloN;

        printf("Resultado final: %d\n", resultadoFinal);

        printf("\n========================================\n");
        printf("       FIM DA EXECUÇÃO DO PROGRAMA\n");
        printf("========================================\n");

        return resultadoFinal;

        //estilização aí dos prints para tentar deixar mais organizado
    }
