#ifndef FORMULAFACTORY_H_INCLUDED
#define FORMULAFACTORY_H_INCLUDED
#include <string>
#include "Expression.h"
using namespace std;
struct Thread
{
    string cell;
    Expression* thread;
    Thread* next = NULL;
};

class FormulaFactory
{
    string cache;
    bool isEmptyCache;
    Thread* t_first;
    Thread* t_last;

    static FormulaFactory* instance;
    FormulaFactory();
    FormulaFactory(const FormulaFactory &);
    void operator=(const FormulaFactory &);
    bool isValidFormula(const string& formula);


    bool RemoveThread(const string&);

public:
    static FormulaFactory* GetInstance();
    string GetResult(const string&,const string&,Thread*);

    int SearchCache(const string&) const;
    bool UpdateCache(const string&,const string&);
    bool DumpCache();

    Thread* SearchThreads(const string&) const;
    Thread* NewThread(const string&);
};

#endif // FORMULAFACTORY_H_INCLUDED
