#include <stdio.h>
#include <math.h>

/* transformă numărul primit în baza 10 și îl returnează */
int transfbaza10(int nr[], int k)
{
    int i, baza10 = 0;

    for (i = k - 1; i >= 0; i--)
    {
        baza10 = baza10 + nr[i] * powf(2, k - 1 - i);
    }

    return baza10;
}

/* transformă numărul primit în baza 2 și returnează lungimea lui */
int transfbaza2(int nr, int baza2[])
{
    int k = 0, i, aux;

    while (nr)
    {
        baza2[k] = nr % 2;
        k++;
        nr = nr / 2;
    }

    for (i = 0; i < k / 2; i++)
    {
        aux = baza2[i];
        baza2[i] = baza2[k - 1 - i];
        baza2[k - 1 - i] = aux;
    }

    return k;
}

/* adună doua numere și returnează suma lor */
int adunare(int nr1, int nr2)
{
    int suma = nr1, urmator = 0;

    while (nr2 != 0)
    {
        suma = nr1 ^ nr2;
        urmator = nr1 & nr2;
        nr2 = urmator << 1;
        nr1 = suma;
    }

    // pentru a păstra doar 4 biți
    suma = suma & 15;

    return suma;
}

/* interschimbă biții după cerință și returnează numărul modificat */
int interschimbare(int nr1, int nr2)
{
    int bit1, bit2, p1[4], p2[4], poz1, poz2, baza2[5], k, baza2bun[5], i, j;

    k = transfbaza2(nr2, baza2);

    // în cazul în care numărul în baza 2 are mai puțin de 4 cifre
    if (k < 4)
    {
        for (i = 0; i < 4 - k; i++)
        {
            baza2bun[i] = 0;
        }

        j = 0;
        for (i = 4 - k; i < 4; i++)
        {
            baza2bun[i] = baza2[j];
            j++;
        }

        p1[0] = baza2bun[0];
        p1[1] = baza2bun[1];
        p2[0] = baza2bun[2];
        p2[1] = baza2bun[3];
    }
    else
    {
        p1[0] = baza2[0];
        p1[1] = baza2[1];
        p2[0] = baza2[2];
        p2[1] = baza2[3];
    }

    // pozițiile biților ce trebuie interschimbați
    poz1 = 3 - transfbaza10(p1, 2);
    poz2 = 3 - transfbaza10(p2, 2);

    bit1 = (nr1 >> poz1) & 1;
    bit2 = (nr1 >> poz2) & 1;

    // dacă biții ce trebuie interschimbați sunt diferiți
    if (bit1 != bit2)
    {
        // se face schimbarea
        nr1 = nr1 ^ ((1 << poz1) | (1 << poz2));
    }

    return nr1;
}

/* rotește la stânga numărul 1 de numărul 2 ori și îl returnează */
int rotatie(int nr1, int nr2)
{
    int rotit;

    if (nr2 > 4)
    {
        nr2 = nr2 % 4;
    }

    rotit = (nr1 << nr2) | (nr1 >> (4 - nr2));

    // pentru a păstra doar 4 cifre
    rotit = rotit & 15;

    return rotit;
}

/* aplică xor între două numere și returnează rezultatul */
int xor (int nr1, int nr2)
{
    int x, y;

    x = nr1 | nr2;
    y = ~(nr1 & nr2);

    return x & y;
}

int main()
{
    int n, m, baza2[101], i = 0, numar[101][101],
    k = 0, j, operatie[101][101], nrcif, op, rezultat;
    int (*oper[4])(int, int) = {adunare, interschimbare, rotatie, xor};

    // se citesc numărul de operații și numărul ce trebuie transformat
    scanf("%d", &n);
    getchar();
    scanf("%d", &m);

    nrcif = transfbaza2(m, baza2);

    // se vor salva doar ultimele n*6+4 cifre
    i = nrcif - n * 6 - 4;

    while (i < nrcif - 4)
    {
        // se salvează numerele (fără ultimul)
        for (j = 0; j < 4; j++)
        {
            numar[k][j] = baza2[i];
            i++;
        }

        // se salvează operațiile
        for (j = 0; j < 2; j++)
        {
            operatie[k][j] = baza2[i];
            i++;
        }

        k++;
    }

    // se salvează și ultimul număr
    for (j = 0; j < 4; j++)
    {
        numar[k][j] = baza2[i];
        i++;
    }

    // rezultatul primei operații
    rezultat = transfbaza10(numar[0], 4);

    for (i = 0; i < k; i++)
    {
        // se efectuează și restul operațiilor
        // cu rezultatul operației anterioare
        op = transfbaza10(operatie[i], 2);
        rezultat = oper[op](rezultat, transfbaza10(numar[i + 1], 4));
    }

    // se afișează rezultatul final
    printf("%d", rezultat);

    return 0;
}