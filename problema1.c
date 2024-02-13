#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* citește fiecare linie și o adaugă într-o matrice */
void citestelinie(char **linie, int i)
{
    char aux[102];

    fgets(aux, 102, stdin);
    linie[i] = malloc(strlen(aux) + 1);
    strcpy(linie[i], aux);
}

/* desparte linia în cuvinte și returnează numărul de cuvinte */
int desparte(char copie[], char *cuvant[])
{
    int k = 0;

    cuvant[k] = strtok(copie, " ");
    while (cuvant[k])
    {
        k++;
        cuvant[k] = strtok(NULL, " ");
    }

    return k;
}

/* verifică daca cuvântul curent trebuie highlight-at */
void verificasimplu(char liniecurenta[],
char cuvcurent[], char keywordsimplu[][11])
{
    int i, poz, este = 0;
    char *adr;

    for (i = 0; i < 9; i++)
    {
        // dacă trebuie highlight-at este înlocuit de _
        if (!strcmp(cuvcurent, keywordsimplu[i]))
        {
            adr = strstr(liniecurenta, cuvcurent);
            poz = adr - liniecurenta;

            while (liniecurenta[poz] != ' ' && liniecurenta[poz] != '\0')
            {
                liniecurenta[poz] = '_';
                poz++;
            }

            este = 1;
            break;
        }
    }

    // dacă nu trebuie highlight-at este înlocuit cu spații
    if (!este)
    {
        adr = strstr(liniecurenta, cuvcurent);
        poz = adr - liniecurenta;

        while (liniecurenta[poz] != ' ' && liniecurenta[poz] != '\0')
        {
            liniecurenta[poz] = ' ';
            poz++;
        }
    }
}

/* verifică dacă cuvântul curent trebuie doar el highlight-at sau
face parte dintr-o secvență ce trebuie highlight-ată, returnând 0 sau 1 */
int verificadublu(char liniecurenta[], char cuvcurent[],
char cuvurmator[], char keyworddublu[][11])
{
    int i, poz, este = 0, estedublu = 0;
    char *adr;

    for (i = 0; i < 4; i++)
    {
        if (!strcmp(cuvcurent, keyworddublu[i]))
        {
            // pentru cazurile "first of" și "list of"
            if ((!strcmp(cuvcurent, "first") || !strcmp(cuvcurent, "list"))
            && !strcmp(cuvurmator, "of"))
            {
                adr = strstr(liniecurenta, cuvcurent);
                poz = adr - liniecurenta;
                este = 1;

                liniecurenta[poz] = '_';
                poz++;

                while (liniecurenta[poz] != 'f')
                {
                    liniecurenta[poz] = '_';
                    poz++;
                }

                liniecurenta[poz] = '_';
                estedublu = 1;
            }

            // pentru cazurile "for" si "for each"
            else if (!strcmp(cuvcurent, "for"))
            {
                adr = strstr(liniecurenta, cuvcurent);
                poz = adr - liniecurenta;
                este = 1;

                while (liniecurenta[poz] != ' ' && liniecurenta[poz] != '\0')
                {
                    liniecurenta[poz] = '_';
                    poz++;
                }

                if (!strcmp(cuvurmator, "each"))
                {
                    while (liniecurenta[poz] != 'h')
                    {
                        liniecurenta[poz] = '_';
                        poz++;
                    }

                    liniecurenta[poz] = '_';
                    estedublu = 1;
                }
            }

            // pentru cazurile "is" si "is a"
            else if (!strcmp(cuvcurent, "is"))
            {
                adr = strstr(liniecurenta, cuvcurent);
                poz = adr - liniecurenta;
                este = 1;

                while (liniecurenta[poz] != ' ' && liniecurenta[poz] != '\0')
                {
                    liniecurenta[poz] = '_';
                    poz++;
                }

                if (!strcmp(cuvurmator, "a"))
                {
                    while (liniecurenta[poz] != 'a')
                    {
                        liniecurenta[poz] = '_';
                        poz++;
                    }

                    liniecurenta[poz] = '_';
                    estedublu = 1;
                }
            }

            break;
        }
    }

    // dacă nu trebuie highlight-at este înlocuit cu spații
    if (!este)
    {
        adr = strstr(liniecurenta, cuvcurent);
        poz = adr - liniecurenta;
        while (liniecurenta[poz] != ' ' && liniecurenta[poz] != '\0')
        {
            liniecurenta[poz] = ' ';
            poz++;
        }
    }

    // returnează dacă a fost o secvență highlightat-ă
    return estedublu;
}

int main()
{
    int n, i, j, k, estedublu;
    char **linie;
    char keywordsimplu[21][11] = {"from", "in", "unit",
    "or", "while", "int", "float", "double", "string"};
    char keyworddublu[21][11] = {"first", "list", "for", "is"};
    char *cuvant[101], copie[101], copie1[101], copie2[101];

    // citește numărul de linii
    scanf("%d", &n);
    getchar();

    // citește liniile
    linie = malloc(n * sizeof(char *));
    for (i = 0; i < n; i++)
    {
        citestelinie(linie, i);
        linie[i][strlen(linie[i]) - 1] = '\0';
    }

    // parcurge liniile
    for (i = 0; i < n; i++)
    {
        puts(linie[i]);

        // desparte linia curentă în cuvinte
        strcpy(copie, linie[i]);
        k = desparte(copie, cuvant);

        // subliniază cuvintele cheie simple găsite într-o copie a liniei
        strcpy(copie1, linie[i]);
        for (j = 0; j < k; j++)
        {
            verificasimplu(copie1, cuvant[j], keywordsimplu);
        }

        // subliniază cuvintele cheie găsite ce pot
        // aparține unei secvențe într-o copie a liniei
        strcpy(copie2, linie[i]);
        for (j = 0; j < k - 1; j++)
        {
            estedublu = verificadublu(copie2, cuvant[j],
            cuvant[j + 1], keyworddublu);

            if (estedublu)
            {
                j++;
            }
        }

        // dacă mai rămâne un cuvânt de verificat
        // ce poate aparține unei secvențe
        if (j == k - 1)
        {
            verificadublu(copie2, cuvant[j], "0", keyworddublu);
        }

        // combină cele două copii în linia originală
        for (j = 0; j < strlen(copie1); j++)
        {
            if (copie1[j] == '_' || copie2[j] == '_')
            {
                linie[i][j] = '_';
            }
            else
            {
                linie[i][j] = ' ';
            }
        }

        // afișează linia care este acum compusă doar din _ și spații
        puts(linie[i]);
    }

    return 0;
}
