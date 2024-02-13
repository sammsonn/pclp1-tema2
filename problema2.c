#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct dictionary_entry
{
    char *word;
    int priority;
};

/* citește fiecare cuvânt */
void citeste(char **cuvant)
{
    char aux[22];

    scanf("%s", aux);
    *cuvant = malloc(strlen(aux) + 1);
    strcpy(*cuvant, aux);
}

/* caută dacă cuvântul corespunde unui cuvânt
din dicționar și returnează 0 sau 1 */
int cautare(struct dictionary_entry *intrare, char *decautat, int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (strstr(intrare[i].word, decautat) == intrare[i].word)
        {
            return 1;
        }
    }

    return 0;
}

/* verifică dacă cuvântul nu este un semn de punctuație și returnează 0 sau 1 */
int trbcautat(char *cuvant)
{
    char semne[11][11] = {",", ".", ":", "!", "?", ";"};
    int i;

    for (i = 0; i < 6; i++)
    {
        if (!strcmp(cuvant, semne[i]))
        {
            return 0;
        }
    }

    return 1;
}

/* adaugă un cuvânt nou in dicționar */
void adaugacuvant(struct dictionary_entry **intrare, int *marime, char *cuvant)
{
    struct dictionary_entry nou;

    nou.word = malloc(strlen(cuvant) + 1);
    strcpy(nou.word, cuvant);
    nou.priority = 1;

    *marime = *marime + 1;
    *intrare = realloc(*intrare, *marime * sizeof(struct dictionary_entry));

    (*intrare)[*marime - 1] = nou;
}

/* verifică dacă cuvântul există deja în dicționar și returnează 0 sau 1*/
int exista(struct dictionary_entry **intrare, char *decautat, int *marime)
{
    int i;

    for (i = 0; i < *marime; i++)
    {
        if (!strcmp((*intrare)[i].word, decautat))
        {
            // dacă există deja îi crește prioritatea
            (*intrare)[i].priority = (*intrare)[i].priority + 1;
            return 1;
        }
    }

    return 0;
}

/* găsește cuvântul cel mai potrivit din dicționar pentru cuvântul dat */
void potrivire(struct dictionary_entry **intrare,
char *decautat, int n, char *cuvbun)
{
    char potrivit[5002][22], minpotrivit[22], bun[5002][22];
    int prioritatepotrivit[5002] = {0}, i, k = 0, maxprioritate, j = 0;

    // parcurge toate cuvintele din dicționar și
    // le păstrează pe cele cu care se potrivește
    for (i = 0; i < n; i++)
    {
        if (strstr((*intrare)[i].word, decautat) == (*intrare)[i].word)
        {
            strcpy(potrivit[k], (*intrare)[i].word);
            prioritatepotrivit[k] = (*intrare)[i].priority;
            k++;
        }
    }

    // parcurge cuvintele ce se potrivesc și află cea mai mare prioritate
    maxprioritate = prioritatepotrivit[0];
    for (i = 1; i < k; i++)
    {
        if (prioritatepotrivit[i] > maxprioritate)
        {
            maxprioritate = prioritatepotrivit[i];
        }
    }

    // parcurge cuvintele ce se potrivesc și le
    // salvează pe cele care au cea mai mare prioritate
    for (i = 0; i < k; i++)
    {
        if (prioritatepotrivit[i] == maxprioritate)
        {
            strcpy(bun[j], potrivit[i]);
            j++;
        }
    }

    // parcurge cuvintele cu prioritate maximă ce se potrivesc
    strcpy(minpotrivit, bun[0]);
    for (i = 1; i < j; i++)
    {
        if (strcmp(bun[i], minpotrivit) < 0)
        {
            // îl păstrează pe cel mai mic lexicografic
            strcpy(minpotrivit, bun[i]);
        }
    }

    strcpy(cuvbun, minpotrivit);

    // crește prioritatea cuvântului găsit
    for (i = 0; i < n; i++)
    {
        if (!strcmp((*intrare)[i].word, minpotrivit))
        {
            (*intrare)[i].priority = (*intrare)[i].priority + 1;
        }
    }
}

int main()
{
    int n, m, i, corespunde, steluta;
    char cuvant[5002][22], cuvbun[22];
    struct dictionary_entry *intrare;

    // citește numărul de cuvinte din dicționarul inițial 
    scanf("%d", &n);
    getchar();

    intrare = malloc(n * sizeof(struct dictionary_entry));

    // citește cuvintele din dicționarul inițial
    for (i = 0; i < n; i++)
    {
        citeste(&intrare[i].word);
        intrare[i].priority = 0;
    }

    // citește numărul de cuvinte introduse de utilizator
    scanf("%d", &m);
    getchar();

    // citește cuvintele introduse de utilizator
    for (i = 0; i < m; i++)
    {
        scanf("%s", cuvant[i]);
        getchar();
    }

    // parcurge cuvintele introduse de utilizator
    for (i = 0; i < m; i++)
    {
        // verifică dacă are * la final
        steluta = 0;
        if (cuvant[i][strlen(cuvant[i]) - 1] == '*')
        {
            steluta = 1;
            cuvant[i][strlen(cuvant[i]) - 1] = '\0';
        }

        // dacă nu este semn de punctuație
        if (trbcautat(cuvant[i]))
        {
            // verifică dacă corespunde măcar unui cuvânt din dicționar
            corespunde = cautare(intrare, cuvant[i], n);

            // dacă nu corespunde niciunuia și nu are * la final
            if (!corespunde && !steluta)
            {
                adaugacuvant(&intrare, &n, cuvant[i]);

                // este afișat așa cum e
                printf("%s ", cuvant[i]);
            }

            // dacă are * la final
            if (steluta)
            {
                // dacă nu există deja în dicționar
                if (!exista(&intrare, cuvant[i], &n))
                {
                    adaugacuvant(&intrare, &n, cuvant[i]);
                }

                // este afișat așa cum e
                printf("%s ", cuvant[i]);
            }

            // dacă corespunde măcar unui cuvânt
            // din dicționar și nu are * la final
            if (corespunde && !steluta)
            {
                potrivire(&intrare, cuvant[i], n, cuvbun);

                // este afișat cuvântul din dicționar cel mai potrivit
                printf("%s ", cuvbun);
            }
        }
        // dacă e semn de punctuație
        else
        {
            // este afișat așa cum e
            printf("%s ", cuvant[i]);
        }
    }

    return 0;
}