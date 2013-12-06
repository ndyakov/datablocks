#include <iostream>
#include "functions.cpp"
#include "Table.h"
using namespace std;

/*! \mainpage Проект : DataBlocks
 *
 * \section intro_sec Интродукция
 *
 * Програма за прочитане на таблица от CSV файл, зареждане на данните в паметта,
 * пресмятане на формулите в таблицата и възможност за редакция на клетки.
 *
 * \section used_sec Използвани библиотеки
 *
 *  За реализирането са използвани библиотеките:
 *  string, iostream, sstream, cmath, fstream и iomanip.
 *
 * \author   Недялко Дяков Ф.Н 71389
 *
 */
int main(int argc,char *argv[])
{
    Table * t;
    int choice=0;
    if(argc>=2 && exists(argv[1]))
    {
        t = new Table(argv[1]);
    }
    else
    {
        string filename;
        while(!exists(filename))
        {
            cout<<" Enter valid filename : ";
            getline(cin,filename);
        }
        t = new(nothrow) Table(filename);
    }
    if(!t)
    {
        cerr<<endl<<" No enough memory! " <<endl;
        return 1;
    }
    try
    {
        t->ReadData();
    }
    catch(int errorn)
    {
        switch(errorn)
        {   case 1:
                cerr<<endl<<"Please check your data file ! "<<endl;
            break;
            case 2:
                 cerr<<endl<<"Error : No enough memory! "<<endl;
            break;
        }
        return 1;
    }
    while(choice != 2)
    {
        cout<<*t;
        menu(choice);
        switch(choice)
        {
            case 1:
                {
                    string cell;
                    cout<<endl<<"Enter Cell Reference ( like RxCx )  :  ";
                    cin>>cell;
                    t->EditCell(cell);
                }
            break;
        }
    }
    delete t;
    return 0;
}
