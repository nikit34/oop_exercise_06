#pragma once
#include <exception>
#include <iostream>
#include "stack.h"


template<class T, size_t ALLOC_SIZE>
struct allocator {
   using value_type = T;
   using size_type = std::size_t;
   using difference_type = std::ptrdiff_t;
   using is_always_equal = std::false_type;

   template<class U>
   struct rebind {
       using other = allocator<U, ALLOC_SIZE>;
   };

   allocator(): ptr_begin(new char[ALLOC_SIZE]), ptr_end(ptr_begin + ALLOC_SIZE), ptr_tail(ptr_begin) {}

   allocator(const allocator&) = delete;
   allocator(allocator&&) = delete;

   ~allocator() {
       delete[] this->ptr_begin;
   }

   T* allocate(std::size_t n);
   void deallocate(T* ptr, std::size_t n);

private:
   char* ptr_begin;
   char* ptr_end;
   char* ptr_tail;
   Stack<char*> free_blocks;
};

template<class T, size_t ALLOC_SIZE>
T* allocator<T, ALLOC_SIZE>::allocate(std::size_t n){
    if(n != 1)
        throw std::logic_error("Can not allocate arrays");
    if(size_t(this->ptr_end - this->ptr_tail) < sizeof(T)){
        if(this->free_blocks.size == 0)
            throw std::bad_alloc();
        auto it = this->free_blocks.begin();
        char* ptr = *it;
        this->free_blocks.pop();
        return reinterpret_cast<T*>(ptr);
   }
   T* result = reinterpret_cast<T*>(this->ptr_tail);
   this->ptr_tail += sizeof(T);
   return result;
}

template<class T, size_t ALLOC_SIZE>
void allocator<T, ALLOC_SIZE>::deallocate(T* ptr, std::size_t n) {
   if(n != 1)
       throw std::logic_error("Can not allocate arrays");
   if(ptr == nullptr)
       return;
   this->free_blocks.push(reinterpret_cast<char*>(ptr));
}
