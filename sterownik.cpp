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
    InicjujMacierzZadan();
    InicjujStanRobotow();

}

/* "główna" funkcja wykonująca się w wątku sterownika */
/* póki co wysyła tylko macierz stanu co 200ms do interfejsu */
void sterownik::run()
{
    while(1)
    {
        OdswierzMacierzStanu();
        ObslurzZadania1();
        //this->msleep(10);

    }
}


void sterownik::ObslurzZadania1()
{
    int i=0;
    int j=0;
    QString Logi;
    Mutex_zadania.lock();
    while(i<100&&tablicazadan[i].stan!=0)
    {
       if((i<100&&tablicazadan[i].stan==1)&&(stan[15][25]==0)&&(licznikzadanaktywnych<20))
       {
       while(RobotZajety[j])j++;
       tablicazadan[i].id_robota=j;
       RobotZajety[j]=1;
       tablicazadan[i].stan=2;
       tablicRobotow[j].X=25;
       tablicRobotow[j].Y=15;
       Logi=QString::number(time(),'g',6)+"s";
       Logi+=" Robot nr. "+ QString::number(j)+" otrzymał zadanie nr. "+QString::number(tablicazadan[i].id_wyswietlane)+ " i wszedł do systemu" ;
       WyslijLogi(Logi);
       WyslijZadania();
       i=100;
       }
       i++;
    }
    Mutex_zadania.unlock();

}










void sterownik::InicjujStanRobotow()
{
    int i;
    for(i=0;i<20;i++)
    {
        RobotZajety[i]=0;
    }
}



void sterownik::InicjujMacierzZadan()
{
    Mutex_zadania.lock();
    int i;
    for(i=0;i<100;i++)
    {
        tablicazadan[i].stan=0;
        tablicazadan[i].numerPolki=0;
        tablicazadan[i].numerStanowiska=0;
        tablicazadan[i].id_wyswietlane=0;
    }
    licznikzadan=0;
    licznikzadanaktywnych=0;
    WyslijZadania();
    Mutex_zadania.unlock();
}

bool sterownik::DodajZadanie(int nrPolki, int nrStanowiska)
{
    Mutex_zadania.lock();
    int i=0;
    while(i<100&&tablicazadan[i].stan!=0) i++;
    if (i==100)
    {
        Mutex_zadania.unlock();
        return 1;
    }
    licznikzadan++;
    tablicazadan[i].stan=1;
    tablicazadan[i].numerPolki=nrPolki;
    tablicazadan[i].numerStanowiska=nrStanowiska;
    tablicazadan[i].id_wyswietlane=licznikzadan;
    WyslijZadania();
    Mutex_zadania.unlock();
    return 0;
}


void sterownik::UsunZadanie(int real_id)
{
    Mutex_zadania.lock();
    int i=real_id;
    while(i<99)
    {
        tablicazadan[i].id_wyswietlane=tablicazadan[i+1].id_wyswietlane;
        tablicazadan[i].numerPolki=tablicazadan[i+1].numerPolki;
        tablicazadan[i].numerStanowiska=tablicazadan[i+1].numerStanowiska;
       tablicazadan[i].stan=tablicazadan[i+1].stan;
        i++;
    }
    tablicazadan[i].stan=0;
    WyslijZadania();
    Mutex_zadania.unlock();
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
    Mutex_stan.lock();
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
    Mutex_zadania.lock();
    i=0;
    while(i<100&&tablicazadan[i].stan!=0)
    {
        if(tablicazadan[i].stan==2||tablicazadan[i].stan==5)
        {

            coordinates temp;
            temp.X=tablicRobotow[tablicazadan[i].id_robota].X;
            temp.Y=tablicRobotow[tablicazadan[i].id_robota].Y;
            stan[temp.Y][temp.X]=3;

        }
        i++;
    }


    Mutex_zadania.unlock();
   Mutex_stan.unlock();
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

    QString log;
    log = QString::number(time(),'g',6);
    // wrzucenie zadania na listę
    if(DodajZadanie(npolki,nstanowiska)==0)log=log + "s Dodano zadanie transportu polki " + QString::number(npolki)+ " na stanowisko "  + QString::number(nstanowiska)+ ". Id="+QString::number(licznikzadan);
    else log=log+ "s Zadanie pominiente. Przekroczono limit ilości zadań";
    emit WyslijLogi(log);
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

void sterownik::WyslijZadania()
{

    QString zadania="";
    int i=0;
    while(i<100&&tablicazadan[i].stan!=0)
    {
        zadania+="id_zadania="+QString::number(tablicazadan[i].id_wyswietlane)+" numer_półki="+QString::number(tablicazadan[i].numerPolki)+" numer_stanowiska="+QString::number(tablicazadan[i].numerStanowiska)+" status=";
        if(tablicazadan[i].stan==1) zadania+="Oczekujące";
        if(tablicazadan[i].stan==2) zadania+="Wysłane po półkę";
        if(tablicazadan[i].stan==3) zadania+="Wysłane do przeładunku";
        if(tablicazadan[i].stan==4) zadania+="Powrut półki";
        if(tablicazadan[i].stan==5) zadania+="Powrut robota";
        zadania+='\n';

        i++;
    }
emit WyslijZadania(zadania);
}


void sterownik::OdbierzRefreshera()
{
Mutex_stan.lock();
    emit Wyslijstan(stan,time());
Mutex_stan.unlock();
}
