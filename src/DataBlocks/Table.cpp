#include "Table.h"
#include "Row.h"
#include "Cell.h"
#include <fstream>
#include <string>
#include "functions.cpp"
#define MAXCOLUMS 51
using namespace std;
int columWidth[MAXCOLUMS];

/** \brief Връща името на файла, от който е заредена информацията.
 *
 * \return const string&  - името на файла.
 *
 */
const string& Table::GetFilename() const
{
    return this->filename;
}

/** \brief Задава име на файла.
 *
 * \param const string& v -  име на файл.
 * \return void
 *
 */
void Table::SetFilename(const string& v)
{
    this->filename = v;
}

/** \brief Прочита стойност на клетка от подаден ред.
 *
 * Прочита стойността на клетка от подаден ред.
 * Преброява не ескейпнатите кавички до момента.
 *
 * \param string& row - прочетеният ред.
 * \param string& cell - клетката, която ще прочетем.
 * \param const char separator - разделител на клетките.
 * \param int& i - позицията от реда, на която сме.
 * \return bool (true ако нещо е прочетено, false ако края на реда е достигнат)
 *
 */
bool Table::ReadCell(string & row, string & cell,const char separator, int& i)
{
    if(row.length() == 0 || row[i] == ',' )
    {
        i++;
        return true;
    }
    if(cell != "")
    {
        cell.append(",");
    }
    while(row[i] != ',')
    {
        if(row[i] == '\0')
            break;
        if(row[i] == '"')
        {
            if( ( i>1 && row[i-1] != '\\' ) || ( i>2 && row[i-1] == '\\' && row[i-2] == '\\') || i==0 )
                this->quoteNum++;
        }
        cell.append(1,row[i]);
        i++;
    }
    if(row[i] == '\0')
    {
        if(cell=="")
        {
            if( i>0 && row[i-1] == ',')
            {
                row[i-1] = ' ';
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else
        i++;
    return true;
}
/** Връща стойност на клетка като double.
 *
 * Връща стойност на клетка като double. За целта взима адреса на клетката
 * чрез GetCell метода и след това я връща като double.
 *
 * \param const string& cell - референция към клетката.
 * \return double - стойността на клетката като double.
 */
double Table::GetCellAsDouble(const string& cell) const
{
    Cell * temp_cell = this->GetCell(cell);
    if(temp_cell)
        return *temp_cell;
    else
        return 0;
}

/** Връща стойност на клетка в контекста на формула.
 *
 * Връща стойност на клетка в контекста на формула.
 * За целта взима адреса на клетката чрез GetCell метода
 * и след това я връща като стринг съдържащ double число.
 * Ако стойността на клетката е "ERROR" , то връща стринга "ERROR".
 *
 * \param const string& cell - референция към клетката.
 * \return string - стойността на клетката като стринг съдържащ double число.
 */
string Table::GetCellFormulaValue(const string& cell) const
{
     Cell * temp_cell = this->GetCell(cell);
     string err = "ERROR";
     if(err.compare(temp_cell->GetPrintValue())==0)
     {
        return err;
     }
     double d = this->GetCellAsDouble(cell);
     ostringstream ss;
     ss << d;
     return ss.str();
}

/** Връща адреса на клетка от таблицата по зададена референция.
 *
 * Връща адреса на клека от таблицата по зададена референция.
 *
 * \param const string& cell - референция към клетката.
 * \return Cell * - указател към клетката.
 */
Cell * Table::GetCell(const string& cell) const
{
    size_t row_pos = cell.find("R");
    size_t cell_pos = cell.find("C");
    Cell * temp_cell;
    Row * temp_row;
    int row_id;
    int cell_id;
    istringstream (cell.substr(row_pos+1)) >> row_id;
    istringstream (cell.substr(cell_pos+1)) >> cell_id;
    temp_row = this->FindNodeById(row_id);
    if(temp_row)
       temp_cell = temp_row->FindNodeById(cell_id);
    else
        return NULL;
    if(temp_cell)
        return temp_cell;
    else
        return NULL;
}

/** Промяна на стойността на клетката по зададена референция.
 *
 * Служи за промяна на стойността на клетката по зададена референция.
 * С помощта на GetCell взима указател към клетката, изписва текущата стойност
 * и очаква вход с нова стойност за клетката.
 * След успешна промяна на клетката изчиства кешираните разултати за
 * изчислените формули и пресмята наново широчината на колоната.
 *
 * \param const string& cell - референция към клетката.
 * \return bool (true - ако промяната е успешна иначе false)
 */
bool Table::EditCell(const string& cell)
{
    string input;
    Cell * temp_cell = GetCell(cell);
    if(!temp_cell)
    {
        cout<<" There is no such cell ! "<<endl;
        return false;
    }
    cout<<"The current value of cell "<<cell<<" is : "<<endl<<temp_cell->GetValue();
    cin.ignore();
    cout<<endl<<"Insert new value: ";
    getline(cin,input);
    input = trim(input);
    if(input.length() == 0)
    {
        *temp_cell = 0;
        if(!temp_cell->IsEmpty())
            temp_cell->IsEmpty(true);
    }
    else if(isString(input) || input[0] == '=')
    {
        *temp_cell = input;
    }
    else if(isInt(input))
    {
        *temp_cell = StringToNumeric<int>(input);
    }
    else if(isDouble(input))
    {
        *temp_cell = StringToNumeric<double>(input);
    }
    else
    {
        cout<<"Error:  "<<input<<" is unknown data type"<<endl;
        return false;
    }
    this->formulaFactory->DumpCache();
    this->CountWidth(temp_cell->GetId());
    return true;
}

/** \brief Запълва редовете с празни клетки.
 *
 *  Запълва редовете с празни клетки до броя на най-дългия ред.
 *
 *  \return void
 */
void Table::FillRows()
{
    Row * temp_row;
    Cell * temp_cell;
    for(temp_row = this->l_first; temp_row; temp_row = temp_row->GetNext())
    {
        while(temp_row->l_count < this->maxCells)
        {
            temp_cell = new(nothrow) Cell(0,this,temp_row->GetId(),1);
            if(!temp_cell)
                throw 2;
            temp_row->AddToTail(temp_cell);
        }
        if(this->maxCells > MAXCOLUMS)
            cout<<endl<<" Maximum colums reached , printing without formating!" <<endl;
        else
            this->CountWidthByRow(temp_row);
    }
}

/** \brief Пресмята широчината на колона.
 *
 *  Пресмята широчината на колона, ако е зададен параметър
 *  смята широчината само на определената колона.
 *  Извивка CountWidthByRow за всеки ред от таблицата.
 *
 *  \param const int columId - ID на колоната, по подразбиране 0
 *  \return void
 */
void Table::CountWidth(const int columId) const
{
    Row * temp_row;
    for(temp_row = this->l_first; temp_row; temp_row = temp_row->GetNext())
        CountWidthByRow(temp_row,columId);
}

/** \brief Пресмята широчината на колона по подаден ред.
 *
 *  Пресмята широчината на колона, ако е зададен параметър
 *  смята широчината само на определената колона.
 *
 *  \param Row * temp_row - текущия ред.
 *  \param const int columId - ID на колоната, по подразбиране 0
 *  \return void
 */
void Table::CountWidthByRow(Row * temp_row,const int columId) const
{
    Cell * temp_cell;
    for(temp_cell = temp_row->GetFirst() ; temp_cell ; temp_cell = temp_cell->GetNext())
    {
        int cellId = temp_cell->GetId();
        if(columId && columId != cellId)
            continue;
        int cellW = temp_cell->GetCellWidth();
        if(temp_row->GetId() == 1)
        {
            columWidth[cellId] = cellW;
        }
        else
        {
            if(columWidth[cellId] < cellW)
                columWidth[cellId] = cellW;
        }
    }
}

/** \brief Прочита данни от файл.
 *
 *  Прочита данните от подадения файл и ги зарежда в
 *  паметта във вида на свързан списък от редове и клетки.
 *
 *  \return void
 */
void Table::ReadData()
{
    string line;
    Row *temp_row;
    Cell *temp_cell;
    int RowNum = 0, CellNum = 0;
    ifstream file (this->filename.c_str());
    while(getline(file,line))
    {
        temp_row = new(nothrow) Row();
        if(!temp_row)
            throw 2;
        RowNum++;
        CellNum = 0;
        this->AddToTail(temp_row);
        string cell ="";
        this->rowPosition = 0;
        this->quoteNum = 0;
        while(this->ReadCell(line,cell, ',',this->rowPosition))
        {
           if((this->quoteNum)%2 != 0 )
                continue;
            cell = trim(cell);
            if(cell.length() == 0)
            {
                temp_cell = new(nothrow) Cell(0,this,temp_row->GetId(), 1);
            }
            else if(isString(cell) || isFormula(cell))
            {
                temp_cell = new(nothrow) Cell(cell,this,temp_row->GetId());
            }
            else if(isInt(cell))
            {
                temp_cell = new(nothrow) Cell(StringToNumeric<int>(cell),this,temp_row->GetId());
            }
            else if(isDouble(cell))
            {
                temp_cell = new(nothrow) Cell(StringToNumeric<double>(cell),this,temp_row->GetId());
            }
            else if(hasMissingComma(cell))
            {
                size_t startPos = this->rowPosition - cell.size();
                cerr<<"ERROR: on row"<<RowNum<<" between "<<startPos<<" and "<<this->rowPosition<<" is missing comma."<<endl;
                throw 1;
            }
            else
            {
                    cerr<<"ERROR: row "<<RowNum<<", col "<<CellNum<<", "<<cell<<" is unknown data type!"<<endl;
                    throw 1;
            }
            if(!temp_cell)
                throw 2;
            temp_row->AddToTail(temp_cell);
            CellNum++;
            cell = "";
        }


        if(CellNum > this->maxCells)
            this->maxCells = CellNum;
    }
    this->FillRows();
}
