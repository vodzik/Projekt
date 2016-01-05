#include "sterownik.h"

/* konstruktor klasy */
sterownik::sterownik(QWidget *parent)
{
    iloscpolek=240;
    InicjalizujWektorPolek();
    InicjalizujMacierzStanu();
    OdswierzMacierzStanu();
    agent = new Smith (this);
}

/* "główna" funkcja wykonująca się w wątku sterownika */
/* póki co wysyła tylko macierz stanu co 200ms do interfejsu */
void sterownik::run()
{
    while(1)
    {
        //OdswierzMacierzStanu();
        emit Wyslijstan(stan);
        this->msleep(200);
    }
}

/* inicjalizacja wektora półek */
/* tworzy wertor półek o długości ustawionej na sztywno w konstruktorze */
/* definiuje położenia półek */
void sterownik::InicjalizujWektorPolek()
{
    polki=new shelf[iloscpolek];
    int i,x,y;
    x=1;
    y=1;
    for(i=0;i<iloscpolek;i++)
    {
        polki[i].polorzenie_bazowe.X=x;
        polki[i].polorzenie_bazowe.Y=y;
        polki[i].polorzenie_aktualne.X=x;
        polki[i].polorzenie_aktualne.Y=y;
        polki[i].id=i;
        x++;
        if(x==25)
        {
            x=1;
            y++;
            if(y%3==2) y++;
        }
    }
}

/* aktualizuje macierz stanu tablicy */
/* póki co wpisuje tylko które pola są zajęte przez pólkę */
/* TODO: trzeba też uwzględnić pozostałe możliwe stany */
/* najlepiej będzie aktualizować poszczególne pola w momencie kiedy będą rozpatrywane ruchy i przydzielanie zasobów */
void sterownik::OdswierzMacierzStanu()
{
    int i,j;
    for(i=0;i<21;i++)
    {
        for(j=0;j<26;j++)
        {
           stan[i][j]=0;
        }
    }
    for(i=0;i<iloscpolek;i++)
    {
        stan[polki[i].polorzenie_aktualne.Y][polki[i].polorzenie_aktualne.X]=2;
    }
}

void sterownik::InicjalizujMacierzStanu()
{
    int i;
    stan=new int*[21];
    for(i=0;i<21;i++)
    {
        stan[i]=new int[26];
    }
}

void sterownik::OdbierzZadanie(int npolki, int nstanowiska)
{
    stan[polki[npolki].polorzenie_bazowe.Y][polki[npolki].polorzenie_bazowe.X] = nstanowiska;
}
