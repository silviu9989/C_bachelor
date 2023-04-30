#include "stdio.h"
#include "math.h"
//y = x^3 + 4x + 20
struct punct
{
    int coord_x;
    int coord_y;
};

typedef struct punct punct;
int mod(int x, int y);
int modinv(int x, int y);
punct adunare_punct(punct P, punct Q, int m);
int numar_puncte(punct P, int m);
punct inmultire_punct(int k, punct P, int m);
void afisare_puncte(punct P, int m);

int main(void)
{
    int p = 29;
    int a = 4;
    int b = 20;
    punct initial1;
    initial1.coord_x = 1;
    initial1.coord_y = 5;


    punct QA, QB, RA, RB;
    afisare_puncte(initial1, p);
    QA = inmultire_punct(181, initial1, p);//20 este cheia privata a lui Alice
    QB = inmultire_punct(43, initial1, p);//7 este cheia privata a lui Alice
    RA = inmultire_punct(181, QB, p);//punctul rezultat al lui Alice
    RB = inmultire_punct(43, QA, p);//punctul rezultat al lui Bob
    printf("Punctul RA are coordonatele (%i, %i)\nPunctul RB are coordonatele (%i, %i)", RA.coord_x, RA.coord_y, RB.coord_x, RB.coord_y);
}

mod(int x, int y)
{
    if (x % y < 0)
        return x % y+y;
    return x % y;
}

int power(int x, int y, int m)
{
    if (y == 0)
        return 1;
    int p = power(x, y / 2, m) % m;
    p = (p * p) % m;

    if (y % 2 == 0)
        return p;
    return (x * p) % m;
}

int modinv(int x, int y)
{
    //stim ca x^(y-1)==1 mod y; inmultim cu x^-1 si obtinem ca x^-1 = x^y-2 mod y
    return mod(power(x, y - 2, y), y);
}

punct adunare_punct(punct P, punct Q, int m)
{
    punct R;
    if (P.coord_x == Q.coord_x && P.coord_y == m - Q.coord_y)
    {
        R.coord_x = 0;
        R.coord_y = 0;
        return R;
    }
    if (P.coord_x == 0 && P.coord_y == 0)
    {
        R.coord_x = Q.coord_x;
        R.coord_y = Q.coord_y;
        return R;
    }
    if (Q.coord_x == 0 && Q.coord_y == 0)
    {
        R.coord_x = P.coord_x;
        R.coord_y = P.coord_y;
        return R;
    }
    int aux;
    if (P.coord_x == Q.coord_x && P.coord_y == Q.coord_y)
        aux = mod((3 * P.coord_x * P.coord_x + 4) * modinv(2, m) * modinv(P.coord_y, m), m);
    else
        aux = mod((Q.coord_y - P.coord_y) * modinv(Q.coord_x - P.coord_x, m), m);

    R.coord_x = mod(-P.coord_x - Q.coord_x + aux * aux, m);
    R.coord_y = mod(-P.coord_y + aux * (P.coord_x - R.coord_x), m);
    return R;
}

int numar_puncte(punct P, int m)
{
    //aplicabil doar in cazul curbelor ciclice (p83 - structura de grup) e doar aproximativ daca grupul nu e perfect ciclic (almost cyclic - p83)
    //calculeaza nr puncte si le si afiseaza!
    int aux=2;
    punct R = adunare_punct(P, P, m);
    //printf("\n%i %i\n%i %i", P.coord_x, P.coord_y, R.coord_x, R.coord_y); // a se decomenta daca se doreste afisarea punctelor
    while ((R.coord_x != P.coord_x || R.coord_y != P.coord_y))
    {
        R = adunare_punct(R, P, m);
        //printf("\n%i %i", R.coord_x, R.coord_y); //a se decomenta daca se doreste afisarea punctelor
        aux++;
    }
    //printf("\nPunctul de la infinit\n\n\n\n"); //a se decomenta daca se doreste afisarea punctelor
    return aux-1;
}

void afisare_puncte(punct P, int m)
{
    //aplicabil doar in cazul curbelor ciclice (p83 - structura de grup) e doar aproximativ daca grupul nu e perfect ciclic (almost cyclic - p83)
    int aux = 2;
    punct R = adunare_punct(P, P, m);
    printf("Punctul de la infinit\n");
    printf("%i, %i\n%i, %i\n", P.coord_x, P.coord_y, R.coord_x, R.coord_y);
    while (!(R.coord_x == P.coord_x && R.coord_y == P.coord_y))
    {
        R = adunare_punct(R, P, m);
        if ((R.coord_x != 0 || R.coord_y != 0) && (!(R.coord_x == P.coord_x && R.coord_y == P.coord_y)))
        {
            printf("%i, %i\n", R.coord_x, R.coord_y);
            aux++;
        }
        /*else if (!(R.coord_x == P.coord_x && R.coord_y == P.coord_y))
        {
            printf("\nPunctul de la infinit");
            aux++;
        }*/
    }
    printf("\n\n");
}

punct inmultire_punct(int k, punct P, int m)
{
    int aux = numar_puncte(P, m);
    k = k % aux;
    aux = sqrt(aux);
    punct* vector;
    vector = (punct*)malloc(aux * sizeof(punct));
    vector[0].coord_x = P.coord_x;
    vector[0].coord_y = P.coord_y;
    int i;
    for (i = 1; i < aux; i++)
    {
        vector[i] = adunare_punct(vector[i - 1], vector[i - 1], m);
    }
    punct rez;
    int steag=0;
    int z = 0;
    for (i = 0; i < aux; i++)
    {
        z = pow(2, i);
        if ((k / z) % 2 == 1 && steag == 0)
        {
            rez = vector[i]; steag = 1;
        }
        else if ((k / z) % 2 == 1 && steag == 1)
            rez = adunare_punct(rez, vector[i], m);
    }
    return rez;
}