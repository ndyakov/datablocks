#ifndef CELL_H_INCLUDED
#define CELL_H_INCLUDED
#include "Node.h"
#include <sstream>
#include "Table.h"
class Cell : public Node<Cell>
{
    friend ostream &operator<<( ostream &out, Cell &c );
    enum CellType
    {
        IntegerType,
        DoubleType,
        StringType,
        FormulaType,
    }cell_type;
    struct
    {
       int  as_integer;
       double as_double;
       string*  as_string;
    }cell_value;
    string print_value;
    int rowId;
    bool isEmpty;


    Table* table;
    void i2d();
    string i2s();
    void d2i();
    string d2s();
    bool s2i();
    bool s2d();

    bool EvaluateFormula();
    string NormalizeFormula();
public:
    Cell(const int v,Table *,const int rowId, bool isEmpty = 0);
    Cell(const double,Table *,const int rowId, bool isEmpty = 0);
    Cell(const string,Table *,const int rowId, bool isEmpty = 0);
    Cell(const Cell&);
    ~Cell();
    size_t GetCellWidth();
    const string& GetPrintValue ();
    const string& GetValue();
    bool IsEmpty(bool toggleEmpty = false);
    Cell& operator = (const Cell&);
    Cell& operator = (const int);
    Cell& operator = (const double);
    Cell& operator = (const string&);
    operator int();
    operator double();
    operator string();


};



#endif // CELL_H_INCLUDED
