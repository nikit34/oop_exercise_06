#include <iterator>
#include <memory>
#include <iostream>


template<typename T>
struct Stack {
private:
    struct Node;

public:
    Stack() = default;

    struct forward_iterator {
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        forward_iterator(Node *ptr);
        T& operator* ();

        forward_iterator& operator++ ();
        forward_iterator operator++ (int);

        bool operator== (const forward_iterator& o) const;
        bool operator!= (const forward_iterator& o) const;

    private:
        Node *ptr;
        friend struct Stack;
    };

    forward_iterator begin();
    forward_iterator end();

    T &top();
    void pop();

    void insert(const forward_iterator &it, const T& value);
    void erase(const forward_iterator &it);
    void push(const T &value);

private:
    size_t size = 0;
    std::unique_ptr<Node> head = nullptr;

    struct Node {
        Node() = default;
        Node(const T& value, std::unique_ptr<Node> next): value(value), next_node(std::move(next)) {};

        forward_iterator next();

    private:
        T value;
        std::unique_ptr<Node> next_node = nullptr;
    };
};

template<typename T>
typename Stack<T>::forward_iterator Stack<T>::Node::next() {
   return this->next_node.get();
}

template<typename T>
typename Stack<T>::forward_iterator Stack<T>::begin() {
   if (this->head == nullptr) {
       return nullptr;
   }
   return this->head.get();
}

template<typename T>
typename Stack<T>::forward_iterator Stack<T>::end() {
   return nullptr;
}

template<typename T>
Stack<T>::forward_iterator::forward_iterator(Node *tmp_ptr): ptr(tmp_ptr) {}

template<typename T>
T& Stack<T>::forward_iterator::operator* () {
    return this->ptr->value;
}

template<typename T>
typename Stack<T>::forward_iterator &Stack<T>::forward_iterator::operator++ () {
   *this = this->ptr->next();
   return *this;
}

template<typename T>
typename Stack<T>::forward_iterator Stack<T>::forward_iterator::operator++ (int) {
   forward_iterator tmp = *this;
   ++(*this);
   return tmp;
}

template<typename T>
bool Stack<T>::forward_iterator::operator== (const forward_iterator &tmp) const {
   return this->ptr == tmp.ptr;
}

template<typename T>
bool Stack<T>::forward_iterator::operator!= (const forward_iterator &tmp) const {
   return this->ptr != tmp.ptr;
}

template<typename T>
void Stack<T>::insert(const forward_iterator &it, const T &value) {
    std::unique_ptr<Node> new_node(new Node(value, nullptr));

    if (it.ptr == nullptr && this->size != 0) {
        throw std::logic_error("Iter went beyonds of stack");
    } else if (it.ptr == nullptr && this->size == 0) {
        this->head = std::unique_ptr<Node>(new Node(value, nullptr));
        ++(this->size);
    } else {
        new_node->next_node = std::move(it.ptr->next_node);
        it.ptr->next_node = std::move(new_node);
        ++(this->size);
    }
}

template<typename T>
void Stack<T>::erase(const typename Stack<T>::forward_iterator &it){
   if (it.ptr == nullptr) {
       throw std::logic_error("Erasing of iter is invalid");
   } else if (it == this->begin()) {
       this->head = std::move(it.ptr->next);
       --(this->size);
   } else {
       Stack<T>::forward_iterator tmp_it = this->begin();
       while (tmp_it.ptr->next() != it.ptr) {
           ++tmp_it;
       }
       tmp_it.ptr->next = std::move(it.ptr->next);
   }
}

template<typename T>
void Stack<T>::push(const T &value) {
   std::unique_ptr<Node> new_node(new Node(value, nullptr));
   new_node->next_node = std::move(this->head);
   this->head = std::move(new_node);
   ++(this->size);
}

template<typename T>
T &Stack<T>::top() {
   if (this->head.get()) {
       return this->head->value;
   } else {
       throw std::logic_error("Stack is empty");
   }
}

template<typename T>
void Stack<T>::pop() {
   if (this->head == nullptr) {
       throw std::logic_error("Stack is empty");
   } else {
       this->head = std::move(this->head->next_node);
       --(this->size);
   }
}
