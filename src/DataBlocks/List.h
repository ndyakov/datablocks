#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include "Node.h"
template <typename NT>
class List
{
protected:
    NT * l_first;
    NT * l_last;
    void RemoveFirst();
    void RemoveLast();
public:
    int l_count;

    List(NT * f = NULL);
    ~List();

    void SetFirst(NT *);
    NT * GetFirst() const;

    void SetLast(NT * last = NULL);
    NT * GetLast();

    void AddToTail(NT *);
    void AddToHead(NT *);

    NT * FindNodeById(const int) const;
    bool IsEmpty() const;
    void FreeList();
};
using namespace std;

/** \brief Конструктор
 *
 *  Конструктор , при подаден параметър  - указател на елемент,
 *  създава лист със съдържание само този елемент.
 *
 *  \param NT * f - указател на елемент, по подразбиране NULL
 */
template <typename NT>
List<NT>::List(NT * f)
{
    this->l_first = f;
    this->l_last = f;
    this->l_count = 0;
    if(f != NULL)
        this->l_count++;
}

/** \brief Десктруктор
 * Деструктор - извиква FreeList();
 */
template <typename NT>
List<NT>::~List()
{
    this->FreeList();
}

/** \brief Задава елемент като начало на списъка
 *
 * Задава елемент като начало на списъка и задава 1 за ID на елемента.
 *
 * \param NT * f - указател към елемента.
 * \return  void
 *
 */
template <typename NT>
void List<NT>::SetFirst(NT * f)
{
    this->l_first = f;
    f->SetId(1);
}

/** \brief Връща първия елемент от списъка.
 *
 * \return  NT * - указател към първия елемент от списъка.
 *
 */
template <typename NT>
NT * List<NT>::GetFirst() const
{
    return this->l_first;
}

/** \brief Намира елемент по подадено id.
 *
 * \param const int id - ID на търсения елемент.
 * \return  NT * - указател към търсения елемент от списъка или NULL ако не е намерен такъв.
 *
 */
template <typename NT>
NT * List<NT>::FindNodeById(const int id) const
{
    NT * temp;
    for(temp = this->l_first; temp; temp = temp->GetNext())
    {
        if(temp->GetId() == id)
            return temp;
    }
    return NULL;
}

/** \brief Проверява дали списъка е празен;
 *
 * \return  bool ( true ако е празен, false ако първия елемент не е NULL ).
 *
 */
template <typename NT>
bool List<NT>::IsEmpty() const
{
    return (this->l_first == NULL) ? true : false;
}

/** \brief Връща последния елемент от списъка.
 *
 * Ако последния елемент е NULL или следващия в списъка след последния не е NULL,
 * извиква метода SetLast , за да оправи подредбата.
 *
 * \return  NT * - указател към последния елемент от списъка.
 *
 */
template <typename NT>
NT * List<NT>::GetLast()
{
    if(this->l_last == NULL || this->l_last->GetNext() != NULL)
    {
        this->SetLast(NULL);
    }
    return this->l_last;
}

/** \brief Задава указател към последния елемент от списъка
 *
 * Ако подадения параметър е NULL, списъка не е празен и съществува елемент след текущия последен,
 * то преминава през целия списък, за да оправи подредбата на елементите
 * ( l_last да сочи към последния елемент от списъка).
 *
 * \param  NT * last - указател към елемент, NULL по подразбиране.
 * \return void
 *
 */
template <typename NT>
void List<NT>::SetLast(NT * last)
{
    if(last != NULL)
    {
        this->l_last = last;
    }
    else if(this->IsEmpty())
    {
        this->l_last = NULL;
    }
    else if(this->l_last->GetNext() == NULL)
    {
        return;
    }
    else
    {
        NT * temp;
        for(temp = this->l_first; temp->GetNext(); temp = temp->GetNext())
        {;}
        this->l_last = temp;
    }
}

/** \brief Изтрива всички елементи от списъка.
 *
 * Премахва първия елемент от списъка, докато списъка не е празен.
 *
 * \return void
 *
 */
template <typename NT>
void List<NT>::FreeList()
{
    while (!this->IsEmpty())
    {
        this->RemoveFirst();
    }
}

/** \brief Премахва първия елемент от списъка.
 *
 * Премахва първия елемент от списъка, ако списъка не е празен.
 * Намаля брояча за броя на елементите след премахване.
 *
 * \return void
 *
 */
template <typename NT>
void List<NT>::RemoveFirst()
{
    if(!this->IsEmpty())
    {
        NT * temp;
        temp = this->l_first;
        this->l_first = temp->GetNext();
        delete temp;
        this->l_count--;
    }
}
/** \brief Премахва последния елемент от списъка.
 *
 * Премахва последния елемент от списъка, ако списъка не е празен.
 * Намаля брояча за броя на елементите след премахване.
 *
 * \return void
 *
 */
template <typename NT>
void List<NT>::RemoveLast()
{
    if(this->IsEmpty())
    {
        return;
    }
    if(this->l_first == this->l_last)
    {
        this->RemoveFirst();
        this->l_count--;
    }
    else
    {
        NT * temp;
        if(this->l_first->GetNext())
        {
            for(temp = this->l_first; temp->GetNext()->GetNext(); temp = temp->GetNext())
            {;}
        }
        delete this->l_last;
        this->l_last = temp;
        this->l_last->SetNext(NULL);
        this->l_count--;
    }
}
/** \brief Добавя елемент към края на списъка.
 *
 * Добавя елемент към края на списъка, преди добавянето извиква
 * SetLast, за да оправи евентуална грешка при подредбата.
 * След добавяне увеличава брояча на елементите.
 *
 * \param NT * temp - указател към елемента за добавяне.
 * \return void
 *
 */
template <typename NT>
void List<NT>::AddToTail(NT * temp)
{
        this->SetLast();
        if(temp != NULL)
        {
            if(this->l_last != NULL)
                this->l_last->SetNext(temp);
            else
            {
                this->l_first = temp;
            }
                this->l_last = temp;
                this->l_count++;
        }
}

/** \brief Добавя елемент към началото на списъка.
 *
 * Добавя елемент към началото на списъка.
 * Ако списъка е празен, то този елемент става първи.
 * След добавянето увеличава брояча на елементите.
 *
 * \param NT * temp - указател към елемента за добавяне.
 * \return void
 *
 */
template <typename NT>
void List<NT>::AddToHead(NT * temp)
{
    if(this->IsEmpty())
        this->l_first = temp;
    else if(temp != NULL)
    {
        temp->SetNext(this->l_first);
        this->l_first = temp;
    }
    if(temp != NULL)
        this->l_count++;
}


#endif // LIST_H_INCLUDED
