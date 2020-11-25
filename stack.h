#pragma once
#include <memory>
#include <iterator>
#include <iostream>


template<typename T, typename Allocator=std::allocator<T>>
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
        Node* ptr;
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
    using allocator_type = typename Allocator::template rebind<Node>::other;
    struct deleter {
        deleter(allocator_type* tmp_allocator): allocator(tmp_allocator) {}

        void operator() (Node *ptr){
            if (ptr != nullptr){
                std::allocator_traits<allocator_type>::destroy(*(this->allocator), ptr);
                this->allocator->deallocate(ptr, 1);
            }
        }
    private:
        allocator_type* allocator;
    };

    using unique_ptr = std::unique_ptr<Node, deleter>;
    size_t size = 0;
    unique_ptr head {nullptr, deleter{&this->allocator}};
    allocator_type allocator {};

    struct Node {
        Node() = default;
        Node(const T& value): value(value), next_node(nullptr) {};
        Node(const T& value, std::unique_ptr<Node> next): value(value), next_node(std::move(next)) {};

        forward_iterator next();

    private:
        T value;
        unique_ptr next_node {nullptr, deleter{&this->allocator}};
    };
};

template<typename T, typename Allocator>
Stack<T, Allocator>::forward_iterator Stack<T, Allocator>::Node::next() {
   return this->next_node.get();
}

template<typename T, typename Allocator>
Stack<T, Allocator>::forward_iterator Stack<T, Allocator>::begin() {
   if (this->head == nullptr) {
       return nullptr;
   }
   return this->head.get();
}

template<typename T, typename Allocator>
Stack<T, Allocator>::forward_iterator Stack<T, Allocator>::end() {
   return nullptr;
}

template<typename T, typename Allocator>
Stack<T, Allocator>::forward_iterator::forward_iterator(Node *tmp_ptr): ptr(tmp_ptr) {}

template<typename T, typename Allocator>
T& Stack<T, Allocator>::forward_iterator::operator* () {
    return this->ptr->value;
}

template<typename T, typename Allocator>
Stack<T, Allocator>::forward_iterator &Stack<T, Allocator>::forward_iterator::operator++ () {
    if(this->ptr != nullptr)
        *this = this->ptr->next();
   return *this;
}

template<typename T, typename Allocator>
Stack<T, Allocator>::forward_iterator Stack<T, Allocator>::forward_iterator::operator++ (int) {
    forward_iterator tmp = *this;
    ++(*this);
    return tmp;
}

template<typename T, typename Allocator>
bool Stack<T, Allocator>::forward_iterator::operator== (const forward_iterator &tmp) const {
   return this->ptr == tmp.ptr;
}

template<typename T, typename Allocator>
bool Stack<T, Allocator>::forward_iterator::operator!= (const forward_iterator &tmp) const {
   return this->ptr != tmp.ptr;
}

template<typename T, typename Allocator>
void Stack<T, Allocator>::insert(const forward_iterator &it, const T &value) {
    Node* new_ptr = this->allocator.allocate(1);
    std::allocator_traits<allocator_type>::construct(this->allocator, new_ptr, value, std::unique_ptr<Node, deleter>(nullptr, deleter{&this->allocator}));

    unique_ptr new_node(new_ptr, deleter{&this->allocator});

    if (it.ptr == nullptr && this->size != 0) {
        throw std::logic_error("Iter went beyonds of stack");
    } else if (it.ptr == nullptr && this->size == 0) {
        this->head = std::move(new_node);
        ++(this->size);
    } else {
        new_node->next_node = std::move(it.ptr->next_node);
        it.ptr->next_node = std::move(new_node);
        ++(this->size);
    }
}

template<typename T, typename Allocator>
void Stack<T, Allocator>::erase(const typename Stack<T, Allocator>::forward_iterator &it){
    if (it.ptr == nullptr) {
        throw std::logic_error("Erasing of iter is invalid");
    } else if (it == this->begin()) {
        this->head = std::move(it.ptr->next);
        --(this->size);
    } else {
        Stack<T, Allocator>::forward_iterator tmp_it = this->begin();
        while (tmp_it.ptr->next() != it.ptr)
            ++tmp_it;
        tmp_it.ptr->next = std::move(it.ptr->next);
    }
}

template<typename T, typename Allocator>
void Stack<T, Allocator>::push(const T &value) {
    Node* new_ptr = this->allocator.allocate(1);
    std::allocator_traits<allocator_type>::construct(this->allocator, new_ptr, value, std::unique_ptr<Node, deleter>(nullptr, deleter{&this->allocator}));

    unique_ptr new_node(new_ptr,, deleter{&this->allocator});
    new_node->next_node = std::move(this->head);
   this->head = std::move(new_node);
   ++(this->size);
}

template<typename T, typename Allocator>
T &Stack<T, Allocator>::top() {
    if (this->head.get())
        return this->head->value;
    throw std::logic_error("Stack is empty");
}

template<typename T, typename Allocator>
void Stack<T, Allocator>::pop() {
    if (this->head) {
        this->head = std::move(this->head->next_node);
        --(this->size);
   } else {
       throw std::logic_error("Stack is empty");
   }
}
