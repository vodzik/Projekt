#include "sterownik.h"

sterownik::sterownik(QWidget *parent)
{

    iloscpolek=240;
    InicjalizujWektorPolek();
    InicjalizujMacierzStanu();


}

void sterownik::run()
{
    while(1)
    {
        OdswierzMacierzStanu();
        emit Wyslijstan(stan);
        this->msleep(200);


    }
}

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
