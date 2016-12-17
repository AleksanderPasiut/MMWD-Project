#pragma once
#include <exception>
#include <new>

template<typename T>
class CircularList
{
private:
	struct node
	{
		T data;
		node* next;
		node* prev;
	};
	node* head;
	size_t el_amount;

public:
	CircularList();
	CircularList(T new_data);
	CircularList(CircularList<T>&);
	CircularList& operator= (CircularList<T>&);
	~CircularList();

	class iterator
	{
	private:
		node* ptr;
	public:
		friend class CircularList<T>;

		iterator() : ptr(NULL) {}
		iterator(CircularList<T>& L) : ptr(L.head) {}
		iterator& operator++();
		iterator& operator--();
		T& operator*();
		node* operator->();
		iterator& operator+(size_t n);
		iterator& operator-(size_t n);
		iterator& operator=(const iterator& Q);
		bool operator==(const iterator& Q);
		bool operator!=(const iterator& Q);
	};

	void clear();
	size_t size() const;

	iterator insert(iterator position, const T& insert_data);	//insert after pointer; returns iterator on inserted data node
	iterator erase(iterator position);							//returns iterator on node before deleted one

	iterator ret_head(); // returns iterator on "head" node
	iterator set_head(const iterator&);
	iterator inc_head(); // increments "head" and returns iterator										
	iterator dec_head(); // decrements "head" and returns iterator
};

//class CircularList methods

template<typename T>
CircularList<T>::CircularList() : head(NULL), el_amount(0)
{

}

template<typename T>
CircularList<T>::CircularList(T new_data)
{
	node* head = new node;
	head->next = head;
	head->prev = head;
	head->data = new_data;
	el_amount = 1;
}

template<typename T>
CircularList<T>::CircularList(CircularList<T>& arg)
{
	el_amount = arg.el_amount;
	
	if (el_amount)
	{
		auto limit = arg.head;
		iterator it;
		do
		{
			it = insert(it, (arg.head)->data);
			arg.inc_head();
		}
		while (arg.head != limit);
	}
}

template<typename T>
CircularList<T>& CircularList<T>::operator= (CircularList<T>& arg)
{
	clear();

	el_amount = arg.el_amount;
	
	if (el_amount)
	{
		auto limit = arg.head;
		iterator it;
		do
		{
			it = insert(it, arg.head->data);
			arg.inc_head();
		}
		while (arg.head != limit);
	}
}

template<typename T>
CircularList<T>::~CircularList()
{
	clear();
}

template<typename T>
void CircularList<T>::clear()
{
	if (!el_amount)
		return;

	head->prev->next = 0;
	do
	{
		node* tmp = head;
		head = head->next;
		delete tmp;
	}
	while(head);

	el_amount = 0;
}

template<typename T>
size_t CircularList<T>::size() const
{
	return el_amount;
}

template<typename T>
typename CircularList<T>::iterator CircularList<T>::insert(iterator position, const T& insert_data)
{
	node* temp = new node;
	temp->data = insert_data;
	if (position.ptr == NULL)
	{
		if (head == NULL)
		{
			temp->next = temp;
			temp->prev = temp;
			head = temp;
			position.ptr = head;
			el_amount++;
			return position;
		}
		else throw std::exception("iterator invalid");
	}

	node* pos = position.ptr;

	temp->prev = pos;
	temp->next = pos->next;

	pos->next->prev = temp;
	pos->next = temp;

	el_amount++;
	position.ptr = temp;
	return position;
}

template<typename T>
typename CircularList<T>::iterator CircularList<T>::erase(iterator position)
{
	node* temp;
	temp = position.ptr;
	if (temp == head)
	{
		if (head->next == head)
		{
			delete temp;
			head = NULL;
			position.ptr = head;
			return position;
		}
		else
			head = head->next;
	}
	(temp->prev)->next = temp->next;
	(temp->next)->prev = temp->prev;
	--position;
	delete temp;
	el_amount--;
	return position;
}

template<typename T>
typename CircularList<T>::iterator CircularList<T>::ret_head()
{
	return CircularList<T>::iterator(*this);
}

template<typename T>
typename CircularList<T>::iterator CircularList<T>::set_head(const iterator& arg)
{
	head = arg.ptr;
	return arg;
}

template<typename T>
typename CircularList<T>::iterator CircularList<T>::inc_head()
{
	if (head)
		head = head->next;
	return CircularList<T>::iterator(*this);
}

template<typename T>
typename CircularList<T>::iterator CircularList<T>::dec_head()
{
	if (head)
		head = head->prev;
	return CircularList<T>::iterator(*this);
}

//class iterator methods

template<typename T>
typename CircularList<T>::iterator& CircularList<T>::iterator::operator++()
{
	if (!ptr)
		throw std::exception("list is empty");
	else ptr = ptr->next;
	return *this;
}

template<typename T>
typename CircularList<T>::iterator& CircularList<T>::iterator::operator--()
{
	if (!ptr)
		throw std::exception("list is empty");
	else ptr = ptr->prev;
	return (*this);
}

template<typename T>
typename T& CircularList<T>::iterator::operator*()
{
	return ptr->data;
}

template<typename T>
typename CircularList<T>::node* CircularList<T>::iterator::operator->()
{
	return ptr;
}

template<typename T>
typename CircularList<T>::iterator& CircularList<T>::iterator::operator+(size_t n)
{
	if (!ptr)
		throw std::exception("list is empty");
	else
	{
		node* temp = ptr;
		for (size_t i = 0; i < n; i++)
			temp = temp->next;

		ptr = temp;
	}
	return *this;
}

template<typename T>
typename CircularList<T>::iterator& CircularList<T>::iterator::operator-(size_t n)
{
	if (ptr == NULL)
		throw std::exception("list is empty");
	else
	{
		node* temp = ptr;
		for (size_t i = 0; i < n; i++)
			temp = temp->prev;

		ptr = temp;
	}
	return *this;
}

template<typename T>
typename CircularList<T>::iterator& CircularList<T>::iterator::operator=(const iterator& Q)
{
	ptr = Q.ptr;
	return *this;
}

template<typename T>
bool CircularList<T>::iterator::operator==(const iterator& Q)
{
	return ptr == Q.ptr;
}

template<typename T>
bool CircularList<T>::iterator::operator!=(const iterator& Q)
{
	return ptr != Q.ptr;
}

