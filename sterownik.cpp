#include "sterownik.h"

sterownik::sterownik(QWidget *parent)
{

}

void sterownik::run()
{
    int i,j;
    stan=new int*[21];
    for(i=0;i<21;i++)  //definicja stanu poczatkowego
    {
        stan[i]=new int[26];
        for(j=0;j<26;j++)
        {
            if((i==1||i==3||i==4||i==6||i==7||i==9||i==10||i==12||i==13||i==15)&&j>0&&j<25) stan[i][j]=2;
            else stan[i][j]=0;
        }
    }
    while(1)
    {
        emit Wyslijstan(stan);
        this->msleep(2000);

        for(i=0;i<21;i++)
        {
            for(j=0;j<26;j++)
            {
               stan[i][j]=(stan[i][j]+1)%6;
            }
        }
    }
}
