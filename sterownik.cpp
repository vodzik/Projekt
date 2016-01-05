#include "sterownik.h"

/* konstruktor klasy */
sterownik::sterownik(QWidget *parent)
{
    iloscpolek=240;
    InicjalizujWektorPolek();
    InicjalizujMacierzStanu();
    OdswierzMacierzStanu();
    agent = new Smith (this);
    connect(agent,SIGNAL(clock(int,int)),this,SLOT(OdbierzZegar(int,int)));
    agent->start();
}

/* "główna" funkcja wykonująca się w wątku sterownika */
/* póki co wysyła tylko macierz stanu co 200ms do interfejsu */
void sterownik::run()
{
    while(1)
    {
        OdswierzMacierzStanu();
        emit Wyslijstan(stan,time());
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
/* TODO2: najlepiej będzie aktualizować poszczególne pola w momencie kiedy będą rozpatrywane ruchy i przydzielanie zasobów */
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

/* inicjalizuje macierz stanu o sztywno zdefioniownych wymiarach */
/* dokładniej tworzy pustą macierz */
/* dodatkowo stworzy mapę obszaru */
void sterownik::InicjalizujMacierzStanu()
{
    int i;
    stan=new int*[21];
    mapa = new int * [21];
    for(i=0;i<21;i++)
    {
        stan[i]=new int[26];
        mapa[i]=new int[26];
    }
}


/* zwraca adres stanowiska */
coordinates sterownik::AdresStanowsika(int s)
{
    // adresy stanowisk ustawione są na sztywno
    coordinates adres;
    adres.Y = 17;
    switch(s)
    {
    case 1: adres.X = 1; break;
    case 2: adres.X = 8; break;
    case 3: adres.X = 15; break;
    case 4: adres.X = 22; break;
    default: break;
    }
    return adres;
}

/* slot służy do odebrania zadania z wątku w którym działa interfejs */
/* w slocie wywoływana jest funkcja dodająca nowe zadanie */
void sterownik::OdbierzZadanie(int npolki, int nstanowiska)
{
    zadanie * z1;
    coordinates p, s;

    // pobranie danych do zadania
    p.X = polki[npolki].polorzenie_bazowe.X;
    p.Y = polki[npolki].polorzenie_bazowe.Y;
    s = AdresStanowsika(nstanowiska);

    // stworzenie zadania w wersji z podziałem na kroki
    z1 = new zadanie(npolki,nstanowiska, p, s);
    z1->GenerujScierzke();

    // wrzucenie zadania na listę
    listaZadan.push_back(z1);

    QString log;

    log = QString::number(time(),'g',6);
    log=log + "s Odebrano zlecenie transportu półki " + QString::number(npolki)+ " na stanowisko "  + QString::number(nstanowiska)+ ".";

    WyslijLogi(log);

    //stan[polki[npolki].polorzenie_bazowe.Y][polki[npolki].polorzenie_bazowe.X] = nstanowiska;
}

void sterownik::OdbierzZegar(int a, int b)
{
    t=a;
    dt=b;
}


double sterownik::time() // zwraca aktualny czas symulacji
{
    return  (double)t * (double)dt / 1000;
}
