#ifndef ROW_H_INCLUDED
#define ROW_H_INCLUDED
#include "Node.h"
#include "List.h"
class Cell;
class Row : public List<Cell>, public Node<Row>
{
    friend ostream &operator<<( ostream &out, const Row &r );
public:
    Row(): List<Cell>(), Node<Row>() {};
    ~Row(){};
};
#endif // ROW_H_INCLUDED
