/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2021 Amber-Sophia Schroeck
*
*The Mini Thread Library is free software; you can redistribute it and/or modify  
*it under the terms of the GNU Lesser General Public License as published by  
*the Free Software Foundation, version 3, or (at your option) any later version.

*The Mini Thread Library is distributed in the hope that it will be useful, but 
*WITHOUT ANY WARRANTY; without even the implied warranty of 
*MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
*General Public License for more details.
*
*You should have received a copy of the GNU Lesser General Public
*License along with the Mini Thread  Library; if not, see
*<https://www.gnu.org/licenses/>.  
*/
#ifndef _MINLIB_ALLOCATOR_BUFFER1_H_
#define _MINLIB_ALLOCATOR_BUFFER1_H_

#include "mn_allocator_interface.hpp"

namespace mn {
    namespace memory {
        /** 
         * @brief buffer based allocator.
         * @note - never frees memory
         * @note - cannot be copied
         * 
         * @author RoseLeBlood
         * @date 2021.02.21
         * @version 1.0
         * 
         * @code cpp
         * #define ALLOC_TESTS_STRUCTS 20
         * #define ALLOC_TESTS_ALLOC 22
         * 
         * int START_VAR = 0;
         * struct alloc_test {
         *  int var;
         *  alloc_test() { var = 1; }
         * };
         * 
         * alloc_test* test;
         * 
         * int main() {
         *  char* buffer = (char*)malloc(ALLOC_TESTS_STRUCTS * sizeof(alloc_test) );
         *  assert(buffer != NULL);
         *  
         *  allocator_buffer_t
         *      alloc(buffer, ALLOC_TESTS_STRUCTS * sizeof(alloc_test));
         *  
         *  alloc.alloc(200);
         *  std::cout << "free: " << alloc.get_free() << "/" << alloc.get_max() << " (" << alloc.get_allocated() << ")" << std::endl;
         *  size_t n = alloc.calloc(ALLOC_TESTS_ALLOC, &test,200);
         *  std::cout << "AL: " << n << " from " << ALLOC_TESTS_ALLOC <<  std::endl;
         *  
         *  for(int i=0; i < n; i++)  std::cout << test->var << "::";
         *  std::cout << "free: " << alloc.get_free() << "/" << alloc.get_max() << " ("  << alloc.get_allocated() << ")" << std::endl;
         * 
         *  return 0;
         * }
         * @endcode
         */
        class basic_allocator_buffer : public allocator_interface {
        public:
            basic_allocator_buffer(char* TBUFFER, unsigned long TSIZE) 
                : allocator_interface(TSIZE), m_bufferTop(0), m_BufferSize(TSIZE), m_buffer(TBUFFER)  {  }

            void* alloc(size_t size, unsigned int xTime) {
                if(is_free(size)) { 
                    assert(m_bufferTop + size <= (m_BufferSize ) );
                    char* ret = &m_buffer[0] + m_bufferTop;
                    m_bufferTop += size;
                    add_allocatedsize(size);
                }
                return (void*)ret;
            }
            /**
             * Allocate n elements of SIZE bytes each, all initialized to 0. 
             * @return A pointer of the allocated ram
             */ 
            size_t calloc(size_t n, size_t size, void** buf, unsigned int xTime) {
                size_t index = (size*n);

                if(is_free(index)) {  
                    if( (get_max() - get_allocated() < index ) ) return 0;
                    assert(m_bufferTop + index <= (m_BufferSize ) );
                    char *_buf = (char*)&m_buffer[0] + m_bufferTop;
                    m_bufferTop += index;
                    add_allocatedsize(index);
                    *buf = _buf;

                    return size;
                }
                return 0;
            }
            void free(void* ptr) { ptr = NULL; }

            basic_allocator_buffer(const basic_allocator_buffer&) = delete;
            basic_allocator_buffer& operator=(const basic_allocator_buffer&) = delete;

            /**
             * @brief Set the limit of bytes to use
             * @param maxSize The limit of bytes to use, when 0 then use no limit 
             */
            virtual void set_limit(size_t) {  }
        private:
            char            *m_buffer;
            size_t          m_bufferTop;
            size_t          m_BufferSize;
        };
    }
}


#endif