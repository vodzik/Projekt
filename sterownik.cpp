#include "sterownik.h"

/* konstruktor klasy */
sterownik::sterownik(QWidget *parent)
{

    flaga_nowych_zadan=0;
    iloscpolek=240;
    InicjalizujWektorPolek();
    InicjalizujMacierzStanu();
    OdswierzMacierzStanu();
    agent = new Smith (this);
    connect(agent,SIGNAL(clock(int,int)),this,SLOT(OdbierzZegar(int,int)));
    connect(this,SIGNAL(polecenie_dla_agenta(int,int)),agent,SLOT(slot1(int,int)));
    connect(agent,SIGNAL(sygnal1(int)),this,SLOT(sygnal_od_agenta(int)));
    agent->start();
    InicjujMacierzZadan();
    InicjujStanRobotow();

    czas_rochu=500;
    czas_zaladunku=1500;
    czas_obslogi=5000;
    czas_rozladunku=1500;
    czas_wyjscia=500;
    kolejka[0]=0;
    kolejka[1]=0;
    kolejka[2]=0;
    kolejka[3]=0;

}

/* "główna" funkcja wykonująca się w wątku sterownika */
/* póki co wysyła tylko macierz stanu co 200ms do interfejsu */
void sterownik::run()
{
    while(1)
    {

        OdswierzMacierzStanu();
        ObslurzUsuwanie();
        OdswierzMacierzStanu();
        ObslurzZadania5();
        ObslurzZadania4();
        ObslurzZadania3();
        ObslurzZadania2();

        ObslurzZadania1();
        if(flaga_nowych_zadan)WyslijZadania();
        flaga_nowych_zadan=0;



    }
}


void sterownik::ObslurzUsuwanie()
{
    Mutex_zadania.lock();
    Mutex_stan.lock();

    int i;


    for(i=99;i>=0;i--)
    {
        if(tablicazadan[i].stan==6)
        {

            flaga_nowych_zadan=1;

            UsunZadanie(i);
            licznikzadanaktywnych--;
        }
    }


    Mutex_stan.unlock();
    Mutex_zadania.unlock();

}


void sterownik::ObslurzZadania1()
{
    int i=0;
    int j=0;
    QString Logi;
    Mutex_zadania.lock();
    Mutex_stan.lock();
    while(i<1000&&tablicazadan[i].stan!=0)
    {
       if((i<1000&&tablicazadan[i].stan==1)&&(stan[15][25]==0)&&(licznikzadanaktywnych<20)&&(polki[tablicazadan[i].numerPolki].zarezerwowana==0)&&(kolejka[tablicazadan[i].numerStanowiska-1]<8))
       {
       polki[tablicazadan[i].numerPolki].zarezerwowana=1;
       while(Roboty[j].polorzenie.active)j++;
       tablicazadan[i].id_robota=j;
       Roboty[j].polorzenie.active=1;
       tablicazadan[i].stan=2;
       Roboty[j].polorzenie.X=25;
       Roboty[j].polorzenie.Y=15;
       Roboty[j].przeladunek=0;
       Roboty[j].zaladowany=0;
       Roboty[j].przetwarzanie=0;
       Roboty[j].przetworzone=0;
       Roboty[j].id_polki=tablicazadan[i].numerPolki;
       Roboty[j].usuwanie=0;
       Roboty[j].usun=0;
       stan[15][25]=3;
       licznikzadanaktywnych++;
       kolejka[tablicazadan[i].numerStanowiska-1]++;
       Logi=QString::number(time(),'g',6)+"s";
       Logi+=" Robot nr. "+ QString::number(j)+" otrzymał zadanie nr. "+QString::number(tablicazadan[i].id_wyswietlane)+ " i wszedł do systemu." ;
       WyslijLogi(Logi);
       //WyslijZadania();
       flaga_nowych_zadan=1;
       i=1000;
       }
    i++;
    }
    Mutex_stan.unlock();
    Mutex_zadania.unlock();

}


void sterownik::ObslurzZadania2()
{
    int i=0;
    int j=0;

    QString Logi;
    coordinates temp;
    int nr_polki;
    Mutex_zadania.lock();
    Mutex_stan.lock();
    while(i<1000&&tablicazadan[i].stan!=0)
    {
       if((i<1000&&tablicazadan[i].stan==2))
       {

           j=tablicazadan[i].id_robota;
           temp.active=0;
           temp.X=0;
           temp.Y=0;
           nr_polki=tablicazadan[i].numerPolki;
           if(Roboty[j].zaladowany==0)
           {
               if(Roboty[j].canorder())
               {


                   //if pierwsza prosta
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X==25&&Roboty[j].najdalszy_zaplanowany().Y>polki[nr_polki].AlejkaBazowa())
                   {
                       temp.X=25;
                       temp.Y=Roboty[j].najdalszy_zaplanowany().Y-1;
                       temp.active=1;

                   }

                   //if droga prosta
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X>polki[nr_polki].polorzenie_bazowe.X&&Roboty[j].najdalszy_zaplanowany().Y==polki[nr_polki].AlejkaBazowa())
                   {
                       temp.X=Roboty[j].najdalszy_zaplanowany().X-1;
                       temp.Y=Roboty[j].najdalszy_zaplanowany().Y;
                       temp.active=1;
                   }


                   //if zjazd po polke
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X==polki[nr_polki].polorzenie_bazowe.X&&Roboty[j].najdalszy_zaplanowany().Y==polki[nr_polki].AlejkaBazowa())
                   {
                       temp.X=Roboty[j].najdalszy_zaplanowany().X;
                       temp.Y=polki[nr_polki].polorzenie_bazowe.Y;
                       temp.active=1;
                   }


                   if((stan[temp.Y][temp.X]==0||stan[temp.Y][temp.X]==2)&&(temp.active==1))
                   {
                       Roboty[j].addorder(temp);
                       stan[temp.Y][temp.X]=1;
                   }


               }



               if(Roboty[j].polorzenie.X==polki[nr_polki].polorzenie_bazowe.X&&Roboty[j].polorzenie.Y==polki[nr_polki].polorzenie_bazowe.Y&&Roboty[j].pracuje==0)
                {
                Roboty[j].pracuje=1;
                Roboty[j].przeladunek=1;
                emit polecenie_dla_agenta(j,czas_zaladunku);

                }
               else if(Roboty[j].cango())
                {
                Roboty[j].pracuje=1;
                emit polecenie_dla_agenta(j,czas_rochu);
                }

           }

           else
           {
               flaga_nowych_zadan=1;
               tablicazadan[i].stan=3;

               Logi=QString::number(time(),'g',6)+"s";
               Logi+=" Robot nr. "+ QString::number(j)+" załadował półkę nr. "+QString::number(tablicazadan[i].numerPolki)+ " (zandnie "+ QString::number(tablicazadan[i].id_wyswietlane) +")." ;
               WyslijLogi(Logi);

           }

       }

    i++;
    }
    Mutex_stan.unlock();
    Mutex_zadania.unlock();

}


void sterownik::ObslurzZadania3()
{
    int i=0;
    int j=0;

    QString Logi;
    coordinates temp;
    int nr_polki;
    int nr_stanowiska;
    Mutex_zadania.lock();
    Mutex_stan.lock();
    while(i<1000&&tablicazadan[i].stan!=0)
    {
       if((i<1000&&tablicazadan[i].stan==3))
       {

           j=tablicazadan[i].id_robota;
           temp.active=0;
           temp.X=0;
           temp.Y=0;
           nr_stanowiska=tablicazadan[i].numerStanowiska;
           nr_polki=tablicazadan[i].numerPolki;
           if(Roboty[j].przetworzone==0)
           {
               if(Roboty[j].canorder())
               {


                   //if powrot do alejki
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X==polki[nr_polki].polorzenie_bazowe.X&&Roboty[j].najdalszy_zaplanowany().Y==polki[nr_polki].polorzenie_bazowe.Y)
                   {
                       temp.X=polki[nr_polki].polorzenie_bazowe.X;
                       temp.Y=polki[nr_polki].AlejkaBazowa();
                       temp.active=1;

                   }

                   //if wyjazd z alejki
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X>0&&Roboty[j].najdalszy_zaplanowany().Y==polki[nr_polki].AlejkaBazowa())
                   {
                       temp.X=Roboty[j].najdalszy_zaplanowany().X-1;
                       temp.Y=Roboty[j].najdalszy_zaplanowany().Y;
                       temp.active=1;
                   }


                   //if zjazd do alejki przeladowczje
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X==0&&Roboty[j].najdalszy_zaplanowany().Y<16)
                   {
                       temp.X=Roboty[j].najdalszy_zaplanowany().X;
                       temp.Y=Roboty[j].najdalszy_zaplanowany().Y+1;
                       temp.active=1;
                   }

                   //dojazd do kolejki stanowiska
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X<AdresStanowsika(nr_stanowiska).X-1&&Roboty[j].najdalszy_zaplanowany().Y==16)
                   {
                       temp.X=Roboty[j].najdalszy_zaplanowany().X+1;
                       temp.Y=Roboty[j].najdalszy_zaplanowany().Y;
                       temp.active=1;
                   }

                   //wjazd w kolejke
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X==AdresStanowsika(nr_stanowiska).X-1&&Roboty[j].najdalszy_zaplanowany().Y<20)
                   {
                       temp.X=Roboty[j].najdalszy_zaplanowany().X;
                       temp.Y=Roboty[j].najdalszy_zaplanowany().Y+1;
                       temp.active=1;
                   }

                   //Przesunięcie w bok w kolejce
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X==AdresStanowsika(nr_stanowiska).X-1&&Roboty[j].najdalszy_zaplanowany().Y==20)
                   {
                       temp.X=Roboty[j].najdalszy_zaplanowany().X+1;
                       temp.Y=Roboty[j].najdalszy_zaplanowany().Y;
                       temp.active=1;
                   }

                   //w gore kolejki
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X==AdresStanowsika(nr_stanowiska).X&&Roboty[j].najdalszy_zaplanowany().Y>17)
                   {
                       temp.X=Roboty[j].najdalszy_zaplanowany().X;
                       temp.Y=Roboty[j].najdalszy_zaplanowany().Y-1;
                       temp.active=1;
                   }



                   if((stan[temp.Y][temp.X]==0)&&(temp.active==1))
                   {
                       Roboty[j].addorder(temp);
                       stan[temp.Y][temp.X]=1;
                   }


               }



               if(Roboty[j].polorzenie.X==AdresStanowsika(nr_stanowiska).X&&Roboty[j].polorzenie.Y==AdresStanowsika(nr_stanowiska).Y&&Roboty[j].pracuje==0)
                {
                Roboty[j].pracuje=1;
                Roboty[j].przetwarzanie=1;
                emit polecenie_dla_agenta(j,czas_obslogi);
                }
               else if(Roboty[j].cango())
                {
                Roboty[j].pracuje=1;
                emit polecenie_dla_agenta(j,czas_rochu);
                }

           }

           else
           {
               Logi=QString::number(time(),'g',6)+"s";
               Logi+=" Obsłużono półkę nr. "+QString::number(tablicazadan[i].numerPolki)+ " (zandnie "+ QString::number(tablicazadan[i].id_wyswietlane) +")." ;
               WyslijLogi(Logi);

               kolejka[tablicazadan[i].numerStanowiska-1]--;
               tablicazadan[i].stan=4;
               flaga_nowych_zadan=1;

           }

       }

    i++;
    }
    Mutex_stan.unlock();
    Mutex_zadania.unlock();

}


void sterownik::ObslurzZadania4()
{
    int i=0;
    int j=0;

    QString Logi;
    coordinates temp;
    int nr_polki;
    Mutex_zadania.lock();
    Mutex_stan.lock();
    while(i<1000&&tablicazadan[i].stan!=0)
    {
       if((i<1000&&tablicazadan[i].stan==4))
       {

           j=tablicazadan[i].id_robota;
           temp.active=0;
           temp.X=0;
           temp.Y=0;
           nr_polki=tablicazadan[i].numerPolki;
           if(Roboty[j].zaladowany==1)
           {
               if(Roboty[j].canorder())
               {



                   //wyjazd z przeladunku
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().Y==17)
                   {
                       temp.X=Roboty[j].najdalszy_zaplanowany().X;
                       temp.Y=Roboty[j].najdalszy_zaplanowany().Y-1;
                       temp.active=1;

                   }

                   //przejazd do konca alejki przeladunkwej

                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X<25&&Roboty[j].najdalszy_zaplanowany().Y==16)
                   {
                       temp.X=Roboty[j].najdalszy_zaplanowany().X+1;
                       temp.Y=Roboty[j].najdalszy_zaplanowany().Y;
                       temp.active=1;

                   }




                   //if pierwsza prosta
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X==25&&Roboty[j].najdalszy_zaplanowany().Y>polki[nr_polki].AlejkaBazowa())
                   {
                       temp.X=25;
                       temp.Y=Roboty[j].najdalszy_zaplanowany().Y-1;
                       temp.active=1;

                   }

                   //if droga prosta
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X>polki[nr_polki].polorzenie_bazowe.X&&Roboty[j].najdalszy_zaplanowany().Y==polki[nr_polki].AlejkaBazowa())
                   {
                       temp.X=Roboty[j].najdalszy_zaplanowany().X-1;
                       temp.Y=Roboty[j].najdalszy_zaplanowany().Y;
                       temp.active=1;
                   }


                   //if zjazd po polke
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X==polki[nr_polki].polorzenie_bazowe.X&&Roboty[j].najdalszy_zaplanowany().Y==polki[nr_polki].AlejkaBazowa())
                   {
                       temp.X=Roboty[j].najdalszy_zaplanowany().X;
                       temp.Y=polki[nr_polki].polorzenie_bazowe.Y;
                       temp.active=1;
                   }


                   if((stan[temp.Y][temp.X]==0||stan[temp.Y][temp.X]==2)&&(temp.active==1))
                   {
                       Roboty[j].addorder(temp);
                       stan[temp.Y][temp.X]=1;
                   }


               }



               if(Roboty[j].polorzenie.X==polki[nr_polki].polorzenie_bazowe.X&&Roboty[j].polorzenie.Y==polki[nr_polki].polorzenie_bazowe.Y&&Roboty[j].pracuje==0)
                {
                Roboty[j].pracuje=1;
                Roboty[j].przeladunek=1;
                emit polecenie_dla_agenta(j,czas_rozladunku);

                }
               else if(Roboty[j].cango())
                {
                Roboty[j].pracuje=1;
                emit polecenie_dla_agenta(j,czas_rochu);
                }

           }

           else
           {

               Logi=QString::number(time(),'g',6)+"s";
               Logi+=" Robot nr. "+ QString::number(j)+" wyładował półkę nr. "+QString::number(tablicazadan[i].numerPolki)+ " (zandnie "+ QString::number(tablicazadan[i].id_wyswietlane) +")." ;
               WyslijLogi(Logi);

               tablicazadan[i].stan=5;
               flaga_nowych_zadan=1;


           }

       }

    i++;
    }
    Mutex_stan.unlock();
    Mutex_zadania.unlock();

}




void sterownik::ObslurzZadania5()
{
    int i=0;
    int j=0;

    QString Logi;
    coordinates temp;
    int nr_polki;
    int nr_stanowiska;
    Mutex_zadania.lock();
    Mutex_stan.lock();
    while(i<1000&&tablicazadan[i].stan!=0)
    {
       if((i<1000&&tablicazadan[i].stan==5))
       {

           j=tablicazadan[i].id_robota;
           temp.active=0;
           temp.X=0;
           temp.Y=0;
           nr_stanowiska=tablicazadan[i].numerStanowiska;
           nr_polki=tablicazadan[i].numerPolki;
           if(Roboty[j].usun==0)
           {

               if(Roboty[j].canorder())
               {


                   //if powrot do alejki
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X==polki[nr_polki].polorzenie_bazowe.X&&Roboty[j].najdalszy_zaplanowany().Y==polki[nr_polki].polorzenie_bazowe.Y)
                   {
                       temp.X=polki[nr_polki].polorzenie_bazowe.X;
                       temp.Y=polki[nr_polki].AlejkaBazowa();
                       temp.active=1;

                   }

                   //if wyjazd z alejki
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X>0&&Roboty[j].najdalszy_zaplanowany().Y==polki[nr_polki].AlejkaBazowa())
                   {
                       temp.X=Roboty[j].najdalszy_zaplanowany().X-1;
                       temp.Y=Roboty[j].najdalszy_zaplanowany().Y;
                       temp.active=1;
                   }


                   //if zjazd do alejki przeladowczje
                   if(temp.active==0&&Roboty[j].najdalszy_zaplanowany().X==0&&Roboty[j].najdalszy_zaplanowany().Y<15)
                   {
                       temp.X=Roboty[j].najdalszy_zaplanowany().X;
                       temp.Y=Roboty[j].najdalszy_zaplanowany().Y+1;
                       temp.active=1;
                   }



                   if((stan[temp.Y][temp.X]==0)&&(temp.active==1))
                   {
                       Roboty[j].addorder(temp);
                       stan[temp.Y][temp.X]=1;
                   }


               }



               if(Roboty[j].polorzenie.X==0&&Roboty[j].polorzenie.Y==15&&Roboty[j].pracuje==0)
                {
                Roboty[j].pracuje=1;
                Roboty[j].usuwanie=1;
                emit polecenie_dla_agenta(j,czas_wyjscia);
                }
               else if(Roboty[j].cango())
                {
                Roboty[j].pracuje=1;
                emit polecenie_dla_agenta(j,czas_rochu);
                }

           }
           else
           {

               Logi=QString::number(time(),'g',6)+"s";
               Logi+=" Robot nr. "+ QString::number(j)+" wyjechał z systemu. Koniec zadania nr. "+ QString::number(tablicazadan[i].id_wyswietlane) +"." ;
               WyslijLogi(Logi);

               tablicazadan[i].stan=6;
               flaga_nowych_zadan=1;

           }


       }

    i++;
    }
    Mutex_stan.unlock();
    Mutex_zadania.unlock();

}


void sterownik::InicjujStanRobotow()
{
    int i;
    for(i=0;i<20;i++)
    {
        Roboty[i].polorzenie.active=0;
        Roboty[i].rezerwacja_1.active=0;
        Roboty[i].rezerwacja_2.active=0;
        Roboty[i].pracuje=0;
        Roboty[i].id_polki=0;
        Roboty[i].przetwarzanie=0;
        Roboty[i].przetworzone=0;
        Roboty[i].usuwanie=0;
    }
}



void sterownik::InicjujMacierzZadan()
{
    Mutex_zadania.lock();
    int i;
    for(i=0;i<1000;i++)
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
    while(i<1000&&tablicazadan[i].stan!=0) i++;
    if (i==1000)
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
    Roboty[tablicazadan[real_id].id_robota].clear();
    polki[tablicazadan[real_id].numerPolki].zarezerwowana=0;

    int i=real_id;
    while(i<98)
    {
        tablicazadan[i].id_wyswietlane=tablicazadan[i+1].id_wyswietlane;
        tablicazadan[i].numerPolki=tablicazadan[i+1].numerPolki;
        tablicazadan[i].numerStanowiska=tablicazadan[i+1].numerStanowiska;
       tablicazadan[i].stan=tablicazadan[i+1].stan;
       tablicazadan[i].id_robota=tablicazadan[i+1].id_robota;
        i++;
    }
    tablicazadan[i].stan=0;
    WyslijZadania();

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
        polki[i].zarezerwowana=0;
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
    coordinates temp;
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
    while(i<1000&&tablicazadan[i].stan!=0)
    {
        if(tablicazadan[i].stan==2||tablicazadan[i].stan==5)
        {


            temp.X=Roboty[tablicazadan[i].id_robota].polorzenie.X;
            temp.Y=Roboty[tablicazadan[i].id_robota].polorzenie.Y;
            if(stan[temp.Y][temp.X]==2) stan[temp.Y][temp.X] =4;
            else  stan[temp.Y][temp.X]=3;

        }

        if(tablicazadan[i].stan==3||tablicazadan[i].stan==4)
        {


            temp.X=Roboty[tablicazadan[i].id_robota].polorzenie.X;
            temp.Y=Roboty[tablicazadan[i].id_robota].polorzenie.Y;
            stan[temp.Y][temp.X] =5;

        }
        i++;
    }

    for(i=0;i<20;i++)
    {
        if(Roboty[i].polorzenie.active)
        {
            if(Roboty[i].rezerwacja_1.active)
            {
                temp.X=Roboty[i].rezerwacja_1.X;
                temp.Y=Roboty[i].rezerwacja_1.Y;
                stan[temp.Y][temp.X]=1;
                if(Roboty[i].rezerwacja_2.active)
                {
                    temp.X=Roboty[i].rezerwacja_2.X;
                    temp.Y=Roboty[i].rezerwacja_2.Y;
                    stan[temp.Y][temp.X]=1;
                }
            }
        }
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
    if(DodajZadanie(npolki,nstanowiska)==0)log=log + "s Dodano zadanie transportu półki " + QString::number(npolki)+ " na stanowisko "  + QString::number(nstanowiska)+ ". Id="+QString::number(licznikzadan);
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
    while(i<1000&&tablicazadan[i].stan!=0)
    {
        zadania+="id_zadania="+QString::number(tablicazadan[i].id_wyswietlane)+" numer_półki="+QString::number(tablicazadan[i].numerPolki)+" numer_stanowiska="+QString::number(tablicazadan[i].numerStanowiska)+" status=";
        if(tablicazadan[i].stan==1) zadania+="Oczekujące";
        if(tablicazadan[i].stan==2) zadania+="Wysłane po półkę";
        if(tablicazadan[i].stan==3) zadania+="Wysłane do przeładunku";
        if(tablicazadan[i].stan==4) zadania+="Powrót półki";
        if(tablicazadan[i].stan==5) zadania+="Powrót robota";
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

void sterownik::sygnal_od_agenta(int id)
{

   bool zrobiono=0;

    Mutex_stan.lock();


    if(Roboty[id].usuwanie && !zrobiono)
    {
        Roboty[id].usuwanie=0;
        Roboty[id].usun=1;
        Roboty[id].pracuje=0;
        zrobiono=1;
    }

    if(Roboty[id].przeladunek && !zrobiono)
    {
        Roboty[id].przeladunek=0;
        Roboty[id].pracuje=0;
        Roboty[id].zaladowany= !Roboty[id].zaladowany;
        zrobiono=1;
    }

    if(Roboty[id].przetwarzanie && !zrobiono)
    {
        Roboty[id].przetwarzanie=0;
        Roboty[id].pracuje=0;
        Roboty[id].przetworzone=1;
        zrobiono=1;

    }


    if(!zrobiono)
    {
        Roboty[id].replace();
        Roboty[id].pracuje=0;
        if(Roboty[id].zaladowany)
        {
            polki[Roboty[id].id_polki].polorzenie_aktualne.X=Roboty[id].polorzenie.X;
            polki[Roboty[id].id_polki].polorzenie_aktualne.Y=Roboty[id].polorzenie.Y;
        }
        zrobiono=1;
    }

    Mutex_stan.unlock();




}

