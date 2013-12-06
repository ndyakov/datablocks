#include "Cell.h"
#include <sstream>
#include <iostream>
#include "functions.cpp"
using namespace std;

/** \brief Конструктур за int тип данни.
 *
 * \param const int v - стойност на клетката.
 * \param Table* - указател към таблицата.
 * \param const int rowId - пореден номер на реда.
 * \param bool isEmpty - посочва дали клетката е празна, по подразбиране false.
 */
Cell::Cell(const int v, Table* tbl,const int rowId, bool isEmpty)
{
    this->cell_type = IntegerType;
    this->cell_value.as_integer = v;
    this->table = tbl;
    this->isEmpty = isEmpty;
    this->rowId = rowId;
}

/** \brief Конструктур за double тип данни.
 *
 * \param const double v - стойност на клетката.
 * \param Table* - указател към таблицата.
 * \param const int rowId - пореден номер на реда.
 * \param bool isEmpty - посочва дали клетката е празна, по подразбиране false.
 */
Cell::Cell(const double v, Table* tbl,const int rowId, bool isEmpty)
{
    this->cell_type = DoubleType;
    this->cell_value.as_double = v;
    this->table = tbl;
    this->isEmpty = isEmpty;
    this->rowId = rowId;

}

/** \brief Конструктур за string тип данни.
 *
 * Ако стринга започва с символ '=' то типа на клетката е FormulaType.
 * В противен случай е StringType.
 *
 * \param const string v - стойност на клетката.
 * \param Table* - указател към таблицата.
 * \param const int rowId - пореден номер на реда.
 * \param bool isEmpty - посочва дали клетката е празна, по подразбиране false.
 */
Cell::Cell(const string v,Table* tbl,const int rowId, bool isEmpty)
{
    if(v[0] != '=')
        this->cell_type = StringType;
    else
        this->cell_type = FormulaType;
    this->cell_value.as_string = new string(v);
    this->table = tbl;
    this->isEmpty = isEmpty;
    this->rowId = rowId;

}
/** \brief Деструктур
 * Ако клетката е от тип StringType или FormulaType
 * извикваме деструктура на стринга със стойността.
 *
 */
Cell::~Cell()
{
    if(this->cell_type == FormulaType || this->cell_type == StringType)
        delete cell_value.as_string;
}
/** \brief  Copy конструктор.
 *
 * Използва предефенирания оператор за равенство.
 *
 */
Cell::Cell(const Cell& other)
{
    *this = other;
}
/** \brief Връща стойността за извеждане на екрана.
 *
 * Обработва стойността на клетката по подходящ начин
 * за извеждеане на екрана.
 *
 * \return const string& - стойността на клетката за принтиране.
 */
const string& Cell::GetPrintValue()
{
    switch(this->cell_type)
    {
        case StringType:
            {
                this->print_value = *cell_value.as_string;
                size_t print_value_size = this->print_value.size(), found=0, i=0;
                if(this->print_value[0] == '"')
                {
                    this->print_value.erase(0,1);
                    print_value_size--;
                }
                if(this->print_value[this->print_value.size()-1] == '"')
                {
                    this->print_value.erase(this->print_value.size()-1);
                    print_value_size--;
                }
                while(i<print_value_size)
                {
                    found = this->print_value.find('\\',i);
                    if(found < print_value_size)
                    {
                        this->print_value.erase(found,1);
                        print_value_size--;
                    }
                    else
                        break;
                    i = found+1;
                }
            }
        break;
        case FormulaType:
            if(!this->s2d())
                this->print_value = "ERROR";
            else
                this->print_value = this->d2s();
        break;
        case IntegerType:
            this->print_value = this->i2s();
        break;
        case DoubleType:
            this->print_value = this->d2s();
        break;
    }
    return this->print_value;
}

/** \brief Връща стойността на клетката.
 *
 * Връща стойността на клетката без да я обработва.
 *
 * \return const string& - стойността на клетката.
 */
const string& Cell::GetValue()
{
     switch(this->cell_type)
    {
        case StringType:
        case FormulaType:
                this->print_value = *cell_value.as_string;
        break;
        case IntegerType:
            this->print_value = this->i2s();
        break;
        case DoubleType:
            this->print_value = this->d2s();
        break;
    }
    return this->print_value;
}


/** \brief Пресмята стойността на формула.
 *
 * Подава формулата за пресмятане или взима пресметната стойност от кеша
 * ако тя е пресметната след последната промяна на таблицата.
 * Проверява за делене на нула и връща "ERROR" като стойност, ако е нужно.
 * Записва резулата като стойност на клетката от тип double. (cell_value.as_double)
 *
 * \return bool - ( true - ако е успяло да пресметне/вземе стойността,
 *                  false - ако е срещнало грешка или стойността е "ERROR")
 */
bool Cell::EvaluateFormula()
{
    ostringstream cell;
    string n_formula;
    string result;
    string err = "ERROR";
    cell << "R" <<this->rowId<<"C"<<this->n_id;
    if(this->table->formulaFactory->SearchThreads(cell.str()))
    {
        return false;
    }
    Thread* my_thread = this->table->formulaFactory->NewThread(cell.str());
    if(this->table->formulaFactory->SearchCache(cell.str()) != -1)
    {
       n_formula = "cached";
    }
    else
    {
        n_formula = this->NormalizeFormula();
    }
    size_t zerodevision = n_formula.find("/{0}");
    if((zerodevision && zerodevision < n_formula.size() ) || n_formula.compare(err) == 0)
    {
        this->table->formulaFactory->UpdateCache(cell.str(),err);
        return false;
    }
    result = this->table->formulaFactory->GetResult(cell.str(),n_formula,my_thread);
    if(result.compare(err) == 0)
        return false;
    this->cell_value.as_double = StringToNumeric<double>(result);
    return true;
}

/** \brief Нормализира формулата, подготвя я за пресмятане.
 *
 * Нормализира вида на формулата. Превръща всички референции и стрингове
 * към числа и слага къдреви скоби ( {} ) около аргументите.
 *
 * \return string - Нормализираната формула или "ERROR" ако срещне грешка.
 */
string Cell::NormalizeFormula()
{
    string formula = *cell_value.as_string;
    string n_formula="=", argument;
    size_t next_operation=0, range=0, operations[5] , formula_size = formula.size();
    if(formula[0] != '=')
    {
            n_formula = "ERROR";
            return n_formula;
    }
    size_t i=1;
    while( i <= (formula_size-1) )
    {
        argument.clear();
        operations[0] = formula.find('+',i);
        operations[1] = formula.find('-',i);
        operations[2] = formula.find('/',i);
        operations[3] = formula.find('*',i);
        operations[4] = formula.find('^',i);
        next_operation = closest(operations);
        next_operation = (next_operation && next_operation < formula_size) ? next_operation : formula_size;
        range = next_operation - i;
        argument = formula.substr(i,range);
        argument = trim(argument);
        n_formula.append("{");
        if(isNumeric(argument))
        {
             n_formula.append(argument);
        }
        else if(isReference(argument))
        {
            string test;
            test = this->table->GetCellFormulaValue(argument);
            if(test.compare("ERROR") == 0)
            {
                n_formula.clear();
                n_formula.append(test);
                return n_formula;
            }
            n_formula.append(test);
        }
        else if(isString(argument))
        {
            n_formula.append(DoubleToString(StringToNumeric<double>(argument)));
        }
        else
        {
            n_formula = "ERROR";
            break;
        }
        n_formula.append("}");
        if(next_operation < (formula_size-1) )
        {

            n_formula.append(formula.substr(next_operation,1));
            i = next_operation+1;
        }
        else
        {
            break;
        }
    }
    return n_formula;
}

/** \brief Връща дължината на стойността за принтиране.
 *
 * Връща size() на стойността от GetPrintValue()
 *
 * \return size_t - дължината на стойността за принтиране.
 */
size_t Cell::GetCellWidth()
{
    return this->GetPrintValue().size();
}

/** \brief Конвертира int към double.
 *
 * Конвертира int към double във стойносттие на клетката.
 *
 * \return void
 */
void Cell::i2d()
{

    this->cell_value.as_double = (double) this->cell_value.as_integer;
}

/** \brief Конвертира int към string.
 *
 * Конвертира int към string с помощта на stringstream.
 *
 * \return string - поличения резултат след конвертиране.
 */
string Cell::i2s()
{
     ostringstream ss;
     ss << this->cell_value.as_integer;
     return ss.str();
}

/** \brief Конвертира double към int.
 *
 * Конвертира double към int във стойносттие на клетката.
 *
 * \return void
 */
void Cell::d2i()
{
    this->cell_value.as_integer = (int) this->cell_value.as_double;
}

/** \brief Конвертира double към string.
 *
 * Конвертира double към string с помощта на stringstream.
 *
 * \return string - поличения резултат след конвертиране.
 */
string Cell::d2s()
{
     return DoubleToString(this->cell_value.as_double);
}

/** \brief Конвертира string към int.
 *
 * Конвертира string към int.
 * Ако клетката е от тип FormulaType, взима резултата след пресмятане.
 * Записва резултата във стойността на клетката като Int (cell_value.as_integer)
 *
 * \return bool - Дали конвертирането е успешно
 *                Служи предимно при конвертиране на формула към число.
 */
bool Cell::s2i()
{
    if(this->cell_type == StringType)
    {
        if(isNumeric(*(this->cell_value.as_string)))
            this->cell_value.as_integer = StringToNumeric<int>(*(this->cell_value.as_string));
        else
            this->cell_value.as_integer = 0;
        return true;
    }
    else if(this->cell_type == FormulaType)
    {
        if(this->EvaluateFormula())
        {
          this->d2i();
          return true;
        }
    }
    return false;
}

/** \brief Конвертира string към double.
 *
 * Конвертира string към double.
 * Ако клетката е от тип FormulaType, взима резултата след пресмятане.
 * Записва резултата във стойността на клетката като double (cell_value.as_double)
 *
 * \return bool - Дали конвертирането е успешно
 *                Служи предимно при конвертиране на формула към число.
 */
bool Cell::s2d()
{
    if(this->cell_type == StringType)
    {
        if(isNumeric(*(this->cell_value.as_string)))
            this->cell_value.as_double = StringToNumeric<double>(*(this->cell_value.as_string));
        else
            this->cell_value.as_double = 0;
        return true;
    }
    else if(this->cell_type == FormulaType)
    {
        if(this->EvaluateFormula())
        {
          return true;
        }
    }
    return false;
}

/** \brief Проверява дали клетката е празна.
 *
 * Проверява дали клетката е празна. При подаден параметър true,
 * променя нейното състояние.
 *
 * \param bool toggleEmpty - Дали да се променя състоянието на клетката.
 *                           По подразбиране - false.
 * \return bool - (true - ако клетката е празна,
                   false - ако клетката не е празна.)
 */
bool Cell::IsEmpty(bool toggleEmpty)
{
    if(toggleEmpty)
        this->isEmpty = !(this->isEmpty);
    return this->isEmpty;
}


/** -----
 *
 * Оператори
 *
 */
Cell& Cell::operator = (const Cell& other)
{
    if(this != &other)
    {
        switch(other.cell_type){
            case FormulaType:
            case StringType:
                    this->cell_value.as_string = new string(*(other.cell_value.as_string));
            break;
            case IntegerType:
                    this->cell_value.as_integer = other.cell_value.as_integer;
            break;
            case DoubleType:
                    this->cell_value.as_double = other.cell_value.as_double;
            break;
        }
        this->cell_type = other.cell_type;
        this->table = other.table;
    }
    return *this;
}

Cell& Cell::operator = (const int v)
{
    if(StringType == cell_type || FormulaType == cell_type){
        delete cell_value.as_string;
    }
    cell_type = IntegerType;
    cell_value.as_integer = v;
    this->isEmpty = false;
    return *this;
}

Cell& Cell::operator = (const double v)
{
    if(StringType == cell_type || FormulaType == cell_type){
        delete cell_value.as_string;
    }
    cell_type = DoubleType;
    cell_value.as_double = v;
    this->isEmpty = false;
    return *this;
}

Cell& Cell::operator = (const string& v)
{
    if(StringType == cell_type || FormulaType == cell_type){
        delete cell_value.as_string;
    }
    if(v[0] == '=')
        cell_type = FormulaType;
    else
        cell_type = StringType;
    cell_value.as_string= new string(v);
    this->isEmpty = false;
    return *this;
}

Cell::operator int()
{
    switch(this->cell_type)
    {
        case StringType:
        case FormulaType:
          if(!this->s2i())
                return 0;
        break;
        case IntegerType:
        break;
        case DoubleType:
            this->d2i();
        break;
    }
    return cell_value.as_integer;

}


Cell::operator double()
{
    switch(this->cell_type)
    {
        case StringType:
        case FormulaType:
            if(!this->s2d())
                return 0;
        break;
        case IntegerType:
            this->i2d();
        break;
        case DoubleType:
        break;
    }
    return cell_value.as_double;
}

Cell::operator string()
{
    switch(this->cell_type)
    {
        case StringType:
        case FormulaType:
        break;
        case IntegerType:
            this->i2s();
        break;
        case DoubleType:
            this->d2s();
        break;
    }
    return *cell_value.as_string;
}



