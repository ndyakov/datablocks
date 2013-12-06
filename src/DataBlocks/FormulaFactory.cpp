#include "FormulaFactory.h"
#include <sstream>
#include <assert.h>
#include <iostream>
using namespace std;
FormulaFactory* FormulaFactory::instance = 0;

/** \brief Конструктур
 *
 * Конструктур, който е private.
 *
 */
FormulaFactory::FormulaFactory()
{
    this->cache = "";
    this->isEmptyCache = true;
    this->t_first = NULL;
    this->t_last = NULL;
}

/** \brief Статичен метод за взимане на инстанция на класа.
 *
 * Връща инстанция на класа, ако такава не съществува
 * създава нова.
 *
 * \return FormulaFactory* - инстанция на класа.
 */
FormulaFactory* FormulaFactory::GetInstance()
{
    if(!instance)
    {
        instance = new(nothrow) FormulaFactory();
        if(!instance)
            throw 2;
    }
    return instance;
}

/** \brief Изчиства кеша от изчислените формули.
 *
 * Изчиства кеша от изчислените формули и вдига флаг,
 * че кеша е празен.
 *
 * \return bool
 */
bool FormulaFactory::DumpCache()
{
    if(this->isEmptyCache)
        return true;
    this->cache.clear();
    this->isEmptyCache = true;
    return true;
}

/** \brief Добавя елемент към кеша.
 *
 * Добавя елемент към кеша с резултати от изчислените функции.
 * Вида на добавения елемент е Референция=Резултат.
 *
 * \param const string& cell - референция на клетката с формулата.
 * \param const string& result - резултат на изчислената формула.
 * \return bool
 */
bool FormulaFactory::UpdateCache(const string& cell,const string& result)
{
    const size_t before = this->cache.size();
    if(!this->isEmptyCache)
        this->cache.append(",");
    this->cache.append(cell);
    this->cache.append("=");
    this->cache.append(result);
    const size_t after = this->cache.size();
    if(after > before)
    {
        this->isEmptyCache = false;
        return true;
    }
    else
    {
        if(after == 0)
        {
            this->isEmptyCache = true;
        }
        return false;
    }
    return false;
}

/** \brief Претърва кеша за елемент.
 *
 * Претърсва кеша за елемент по подадена референция към клетка.
 *
 * \param const string& cell - референция на клетката.
 * \return int - Индекса на резултата спрямо стринга cache.
 *               Или -1 ако не е намерило нищо.
 */
int FormulaFactory::SearchCache(const string& cell) const
{
    size_t cache_size = this->cache.size() , found = this->cache.find(cell);

    if(this->isEmptyCache || found > cache_size )
        return -1;
    else
        return found;
}

/** \brief Претърва формулите, които в момента се изчисляват.
 *
 * Претърсва списъка с формули, които в момента се изчисляват
 * по референция към клетка.
 *
 * \param const string& cell - референция на клетката.
 * \return Thread* - указател към намерената структура или
 *                   NULL ако не е намерена такава.
 */
Thread* FormulaFactory::SearchThreads(const string& cell) const
{
    Thread * temp_thread;

    if(this->t_first == NULL)
        return NULL;
    for(temp_thread = this->t_first; temp_thread; temp_thread = temp_thread->next)
    {
        if(cell.compare(temp_thread->cell) == 0)
            break;
    }


    return temp_thread;
}

/** \brief Проверява дали формулата е валидна след нормализация.
 *
 * Проверява дали формулата е валидна и готова за изчисляване.
 *
 * \param const string& formula
 * \return bool  True - Ако е валидна.
 *               False - В противен случай.
 */
bool FormulaFactory::isValidFormula(const string& formula)
{
    int right=0,left=0,operations=0;
    bool inbrackets = false;
    for(size_t i = 1; i< formula.size(); i++)
    {
        switch(formula[i])
        {
            case '{':
                left++;
                inbrackets = true;
            break;
            case '}' :
                right++;
                inbrackets = false;
            break;
            case '+':
            case '-':
            case '/':
            case '*':
            case '^':
                if(!inbrackets)
                    operations++;
            break;
            case ' ':
            case '\t':
            case '.':
                continue;
            break;
            default:
                if(formula[i] < '0' && formula[i] > '9' )
                    return false;
            break;
        }
    }
    if(left > MAXELEMENTS)
    {
        cout<<endl<< "Maximum arguments reached in formula : "<<formula<<endl;
        return false;
    }
    return (formula[0] == '=' && formula[1] == '{' && left==right && (left-1) == operations);
}

/** \brief Връща пресметнат резултат от нормализирана формула.
 *
 * Ако резултата е вече пресметнат и кеширан - направо го връща.
 * В противен случай се опитва да пресметне формулата, запазва
 * резултата и в кеша и го връща.
 *
 * \param const string& cell - референция към клетка.
 * \param const string& formula - нормализирана формула.
 * \param Thread* my_thread - указател към структурата на "процеса" за изчисляване.
 * \return string - Резултат във формата на double или ERROR при грешка.
 */

string FormulaFactory::GetResult(const string& cell ,const string& formula, Thread* my_thread)
{
    string result = "ERROR";
    int c_start = this->SearchCache(cell);
    size_t c_end, c_range;
    if(!this->isEmptyCache && c_start != -1 && c_start < (int)(this->cache.size()))
    {
        c_start = this->cache.find("=",c_start);
        c_end = this->cache.find(",",c_start);
        c_range = c_end - (c_start+1);
        result = this->cache.substr(c_start+1, c_range);
    }
    else if(formula.compare("cached") != 0 && formula.size() != 0 && this->isValidFormula(formula) )
    {
        my_thread->thread = new Expression(formula);
        if(!my_thread->thread)
            throw 2;
        result = my_thread->thread->ResultAsString();
        this->UpdateCache(cell,result);
        delete my_thread->thread;
    }
    this->RemoveThread(cell);
    return result;
}

/** \brief Създава "процес" за изчисляване на формула.
 *
 * Създава "процес" за изчисляване на формула и го поставя в
 * свързан списък с текущите процеси.
 *
 * \param const string& cell - референция към клетка.
 * \return Thread* - указател към структурата.
 */
Thread* FormulaFactory::NewThread(const string& cell)
{
    Thread* new_thread = new(nothrow) Thread;
    if(!new_thread)
        throw 2;
    new_thread->cell = cell;
    if(this->t_first == NULL)
    {
        this->t_first = new_thread;
        this->t_last = new_thread;
    }
    else
    {
        this->t_last->next = new_thread;
        this->t_last = new_thread;
    }
    return new_thread;
}

/** \brief Премахва "процес" за изчисляване на формула.
 *
 * Премахва "процес" за изчисляване на формула от
 * свързан списък с текущите процеси.
 *
 * \param const string& cell - референция към клетка.
 * \return bool - True - Ако успее да го премахне.
 *                False - Ако не намери такъв "процес".
 */
bool FormulaFactory::RemoveThread(const string& cell)
{
    Thread* temp_thread = this->t_first;
    if(cell.compare(temp_thread->cell)==0)
    {
        this->t_first = temp_thread->next;
        delete temp_thread;
        return true;
    }
    else
    {
        Thread* second_thread;
        for(; temp_thread->next; temp_thread = temp_thread->next)
        {
            if(cell.compare(temp_thread->next->cell)==0)
            {
                second_thread = temp_thread->next;
                if(second_thread == this->t_last)
                {
                    temp_thread->next = NULL;
                    this->t_last = temp_thread;
                }
                else
                {
                    temp_thread->next = second_thread->next;
                }
                delete second_thread;
                return true;
            }
        }
    }
    return false;
}
