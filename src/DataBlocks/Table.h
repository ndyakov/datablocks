#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED
#include "List.h"
#include "FormulaFactory.h"
class Cell;
class Row;
class Table : public List<Row>
{
    friend ostream &operator<<( ostream &out, const Table &t );
protected:
    string filename;
    int quoteNum;
    int rowPosition;
    int maxCells;
public:
    FormulaFactory* formulaFactory;

    Table(const string f): List(), filename(f), quoteNum(0),rowPosition(0),maxCells(0)
    {
        this->formulaFactory = FormulaFactory::GetInstance();
    };

    const string& GetFilename() const;
    void SetFilename(const string &);

    void ReadData();
    void SaveData();
    void FillRows();
    void CountWidth(int const columId = 0)const;
    void CountWidthByRow(Row*,const int columId = 0)const;
    bool ReadCell(string&, string&,const char, int&);
    double GetCellAsDouble(const string&) const;
    string GetCellFormulaValue(const string&) const;
    Cell * GetCell(const string&) const;
    bool EditCell(const string& cell);
};
#endif // TABLE_H_INCLUDED
