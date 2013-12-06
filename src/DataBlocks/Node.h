#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
#include <string>
using namespace std;

template <typename NT> class Node
{
protected:
    NT * n_next;
    int n_id;
    Node(): n_next(NULL),n_id(1){};
public:
    void SetId(const int id);
    const int GetId() const;
    ~Node(){};

    void SetNext(NT *);
    NT * GetNext() const;
};

/** \brief Задава ID на елемента.
 *
 * Задава ID на елемента равно на подадения параметър.
 *
 * \param const int id
 * \return void
 *
 */
template <typename NT>
void Node<NT>::SetId(const int id )
{
    this->n_id = id;
}

/** \brief Взема id-то на елемента.
 *
 * Връща стойността на n_id - пореден номер
 * на елемента от списъка.
 *
 * \return int
 *
 */
template <typename NT>
const int Node<NT>::GetId() const
{
    return this->n_id;
}

/** \brief Задава следващия елемент в списъка.
 *
 * Задава следващия елемент в списъка,
 * указателя n_next на текущия елемент е равен на подадения параметър.
 *
 * \param NT * n - указател към следващия елемент.
 * \return void
 *
 */
template <typename NT>
void Node<NT>::SetNext(NT * n)
{
    this->n_next = n;
    n->SetId(this->n_id+1);
}

/** \brief Взема следващия елемент от списъка
 *
 * Връща стойността на n_next - указател сочещ към
 * следващия елемент от списъка.
 *
 * \return NT *
 *
 */
template <typename NT>
NT * Node<NT>::GetNext() const
{
    return this->n_next;
}

#endif // NODE_H_INCLUDED
