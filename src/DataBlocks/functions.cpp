#ifndef FUNCTIONS_CPP_INCLUDED
#define FUNCTIONS_CPP_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "Cell.h"
#include "Row.h"
#include "Table.h"
#define MAXCOLUMS 51

extern int columWidth[MAXCOLUMS];
using namespace std;
/** \brief Проверява дали файла съществува.
 *
 * \param const string& filename - име на файла.
 * \return bool True - ако файла съществува.
 *              False - ако файла не съществува.
 */
inline bool exists(const string& filename)
{
  ifstream ifile(filename.c_str());
  return ifile;
}
/** \brief Принтира менюто и приема число за избор.
 *
 * \param int& choice
 * \return void
 */
inline void menu(int& choice)
{
    cout<<"--Menu--"<<endl;
    cout<<"[1] Edit"<<endl;
    cout<<"[2] Exit"<<endl;
    cout<<"--------"<<endl;
    cout<<"[] : ";
    cin>>choice;
}


/** \brief Проверява за липсваща запетая.
 *
 *  Проверява за whitespace в стринга.
 *
 * \param const strint& str
 * \return bool True - ако липсва запетая.
 *              False - ако не лиспва.
 */
inline bool hasMissingComma(const string& str)
{
    size_t tab = str.find('\t');
    size_t space = str.find(" ");
    return (space < str.size() || tab < str.size());
}

/** \brief Изчиства whitespace в началото и края на стринг.
 *
 *  Изчиства ' ' и '\t' в началото и края на стринг.
 *
 * \param const strint& str
 * \param const char whitespace - символ за изчистване, по подразбиране ' '
 * \return string - изчистения стринг
 */
inline string trim(const string& str,const char whitespace=' ')
{
    string result;
    size_t strBegin = str.find_first_not_of(whitespace);
    if (strBegin == string::npos)
        return "";
    size_t strEnd = str.find_last_not_of(whitespace);
    size_t strRange = strEnd - strBegin + 1;
    result = str.substr(strBegin, strRange);
    if(whitespace == ' ')
    {
        result = trim(result,'\t');
    }
    else if (whitespace == '\t')
    {
        result = trim(result,'\n');
    }
    return result;
}

/** \brief Намира най-малкото число в масив.
 *
 *  Намира най-малкото число в масив с пет елемента.
 *
 * \param size_t possitions[] - масив с елементи
 * \return size_t - най-малкият елемент от масива
 */
inline size_t closest(size_t possitions[])
{
    size_t minimal = possitions[0];
    for(int i=1;i<5;i++)
    {
        if(possitions[i]<minimal)
            minimal = possitions[i];
    }
    return minimal;
}

/** \brief Проверява дали стойността на стринг е число (int)
 *
 *
 * \param const string& str
 * \return bool  True - Ако съдържанието на стринга отговаря на тип int.
 *               False - В противен случай.
 */
inline bool isInt(const string& str)
{
    for(unsigned int i = 0; i < str.size(); i++)
    {
        if( (i == 0 && str[i] != '+' && str[i] != '-' && (str[i] < '0' ||  str[i] > '9') ) ||
            (i != 0 && ( str[i] < '0' || str[i] > '9') ) )
        {
            return false;
        }
    }
    return true;
}

/** \brief Проверява дали стойността на стринг е число (double)
 *
 *
 * \param const string& str
 * \return bool  True - Ако съдържанието на стринга отговаря на тип double.
 *               False - В противен случай.
 */
inline bool isDouble(const string& str)
{
   bool hasDot = false;
    for(unsigned int i = 0; i < str.size(); i++)
    {
        if(i != 0 && str[i] == '.' && !hasDot)
            hasDot = true;
        else  if( (i == 0 && str[i] != '+' && str[i] != '-' && (str[i] < '0' ||  str[i] > '9') ) ||
            (i != 0 && ( str[i] < '0' || str[i] > '9') ) )
        {
            return false;
        }
    }
    return true;
}

/** \brief Проверява дали стойността на стринг е референция към клетка.
 *
 *
 * \param const string& str
 * \return bool  True - Ако съдържанието на стринга е референция към клетка.
 *               False - В противен случай.
 */
inline bool isReference(const string& str)
{
    size_t c_pos = str.find('C',1);
    return (str[0] == 'R' && c_pos && isInt(str.substr(1,c_pos-1)) && isInt(str.substr(c_pos+1)));
}
inline bool isNumeric(const string& str)
{
   return isDouble(str);
}

/** \brief Проверява дали стойността на стринг е формула.
 *
 *
 * \param const string& str
 * \return bool  True - Ако съдържанието на стринга е формула.
 *               False - В противен случай.
 */
inline bool isFormula(const string& str)
{
    size_t nextEqual = str.find('=',1);
    if(str[0] == '=' && nextEqual > str.size())
        return true;
    return false;
}

/** \brief Проверява дали стойността на стринг отговаря на условията за тип Стринг.
 *
 *
 * \param const string& str
 * \return bool  True - Ако съдържанието на стринга отговаря на условията.
 *               False - В противен случай.
 */
inline bool isString(const string& str)
{
    int quoteNum = 0;
    size_t lastQuote = str.rfind('"');
    size_t lastNotWhitespace = str.find_last_not_of(" ");
    if(lastNotWhitespace <= lastQuote || lastNotWhitespace > str.size())
        lastNotWhitespace = str.find_last_not_of('\t');
    if(str[0]=='"')
    {
        quoteNum = 1;
        for(unsigned int i=1; i< str.size(); i++)
        {
         if(str[i] == '"' && ( (i>1 && str[i-1] != '\\') || (i>2 && str[i-1] == '\\' && str[i-2] == '\\') ) )
            quoteNum++;
        }
        if(quoteNum == 2 && (lastNotWhitespace <= lastQuote || lastNotWhitespace > str.size() ))
            return true;
    }
    return false;
}
/** \brief Конвертира стринг към числов тип
 *
 *  Конвертира стринг към числов тип спрямо условията.
 *
 * \param  string str
 * \return Numeric (double,int).
 */
template <typename Numeric>
Numeric StringToNumeric(string str)
{
    if(str[0] == '"')
        str.erase(0,1);
    if(str[str.size()-1] == '"')
        str.erase(str.size() - 1);
    Numeric tmp=0;
    if(!isNumeric(str))
        return tmp;

    bool hasSign = false;
    int sign = 1;
    if(str[0] == '-')
    {
        hasSign = true;
        sign = -1;
    }
    else if(str[0] == '+')
    {
        hasSign = true;
    }
    if(hasSign)
    {
        str.erase(str.begin());
    }
    istringstream (str) >> tmp;
    tmp = tmp*sign;
    return tmp;
}

/** \brief Конвертира Double към String
 *
 *  Каства Double към String с помощта на stringstream.
 *
 * \param  const double d
 * \return string
 */
inline string DoubleToString(const double d)
{
     ostringstream ss;
     ss << d;
     return ss.str();
}


/** \brief Предефениран оператор << за изход на Таблица към поток.
 *
 * \param ostream &out - потока където ще принтираме.
 * \param const Table &t - таблицата за принтиране.
 * \return ostream&
 */
inline ostream &operator<<( ostream &out, const Table &t )
{
    out<<endl<<"------------Table------------"<<endl;
    Row * temp_row;
    for(temp_row = t.l_first; temp_row; temp_row = temp_row->GetNext())
    {
        out << *temp_row;
    }
    out<<endl<<"-----------------------------"<<endl;
    return out;
}

/** \brief Предефениран оператор << за изход на Row към поток.
 *
 *  Подравнява клетките спрямо максималната широчина на колоните.
 *
 * \param ostream &out - потока където ще принтираме.
 * \param const Row &r - реда за принтиране.
 * \return ostream&
 */
inline ostream &operator<<( ostream &out, const Row &r )
{
    Cell * temp_cell;
    out << '|';
    for(temp_cell = r.l_first; temp_cell; temp_cell = temp_cell->GetNext())
    {
        out << setw(columWidth[temp_cell->GetId()]);
        out << *temp_cell;
        out<<'|';
    }
    out << '\n';
    return out;
}

/** \brief Предефениран оператор << за изход на Cell към поток.
 *
 *
 *
 * \param ostream &out - потока където ще принтираме.
 * \param const Cell &c - клетката за принтиране.
 * \return ostream&
 */
inline ostream &operator<<( ostream &out,  Cell &c )
{
    if(c.IsEmpty())
        out<<" ";
    else
        out<< c.GetPrintValue();
    return out;
}

#endif //FUNCTIONS_CPP_INCLUDED
