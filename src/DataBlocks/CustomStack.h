#ifndef CUSTOMSTACK_H_INCLUDED
#define CUSTOMSTACK_H_INCLUDED

template <typename E_TYPE, int MAXSIZE>
class CustomStack{

CustomStack(CustomStack const &) {};
CustomStack& operator=(CustomStack const &) {};

E_TYPE c_stack[MAXSIZE];
int c_count;

public:
    CustomStack();
    ~CustomStack();
    bool Push(E_TYPE value);
	bool Pop(E_TYPE& value);
	E_TYPE Peek();
	size_t GetSize();
	bool IsEmpty();
};

/**
 *
 *	\brief Конструктор
 *
 */
template <class E_TYPE, int MAXSIZE>
CustomStack<E_TYPE, MAXSIZE>::CustomStack()
{
	c_count = 0;
}


/**
 *
 *	\brief Деструктор
 *
 */
template <class E_TYPE, int MAXSIZE>
CustomStack<E_TYPE, MAXSIZE>::~CustomStack()
{
}

/**
 *
 * \brief Добавя нов елемент в стека
 *
 *	\return
 *	Ако функцията успее да добави елемента, връща true.
 *	Ако в стека няма свободно място и тя не успее да го добави, връща false.
 *
 */
template <class E_TYPE, int MAXSIZE>
bool CustomStack<E_TYPE, MAXSIZE>::Push(E_TYPE Value)
{
	if(c_count < MAXSIZE)
	{
		c_stack[c_count++] = Value;

		return true;
	}
	else
	{
		return false;
	}
}


/**
 *
 *	\brief Изважда и връща елемента, който се намира най-отгоре в стека
 *
 *	\param [out] Value
 *		Псевдоним на променлива, в която ще се запише резулатата.
 *
 *	Ако в стека няма елементи, функцията не прави нищо и Value не се променя
 *
 *	\return
 *		Функцията връща true, ако успее и във Value успешно е записан
 *		най-горният елемент. 	В противен случай функцията връща false.
 */
template <class E_TYPE, int MAXSIZE>
bool CustomStack<E_TYPE, MAXSIZE>::Pop(E_TYPE& Value)
{
	if( ! this->IsEmpty() )
	{
		Value = c_stack[c_count - 1];
		c_count--;

		return true;
	}
	else
	{
		return false;
	}
}


/**
 *
 *	\brief Връща елемента, който се намира най-отгоре в стека.
 *
 *	За да работи, функцията изисква в стека да има поне един елемент;
 *
 */
template <class E_TYPE, int MAXSIZE>
E_TYPE CustomStack<E_TYPE, MAXSIZE>::Peek()
{

	return c_stack[c_count - 1];
}


/**
 *
 *	\brief Връща броя на елементите в стека
 *
 */
template <class E_TYPE, int MAXSIZE>
size_t CustomStack<E_TYPE, MAXSIZE>::GetSize()
{
	return c_count;
}


/**
 *
 *	\brief Проверява дали стекът е празен
 *
 */
template <class E_TYPE, int MAXSIZE>
bool CustomStack<E_TYPE, MAXSIZE>::IsEmpty()
{
	return c_count == 0;
}


#endif // CUSTOMSTACK_H_INCLUDED
