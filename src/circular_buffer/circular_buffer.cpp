#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <unordered_map>

#include "dtack.base.circular_buffer.h"

namespace dtack
{
    namespace base
    {
        extern void *allocate_from_heap(const char *caller, const char *purpose, int count, int size);

        template<typename T> CircularBuffer::CircularBuffer(uint64_t size):
            size(size),
            put_at(0),
            buffer(NULL)
        {
            buffer = (T *)allocate_from_heap("CircularBuffer", "buffer", size, sizeof(T));
            // const char *F = "CircularBuffer"; 
            // printf(".......... %s: constructing with %zd items of size %zd at0x%08x\n", F, size, sizeof(T), buffer);
        }

        CircularBuffer::~CircularBuffer()
        {
            wipe();

            // const char *F = "~CircularBuffer"; 
            // printf(".......... %s: destructing buffer at 0x%08x\n", F, buffer);
            if (buffer != NULL)
                free(buffer);
        }

        bool CircularBuffer::read(int64_t at, T &value)
        {
            std::lock_guard<std::mutex> lock(sync);
            size_t get_at = put_at - size + at;
            if (get_at < 0)
                get_at = 0;
            
printf(".......... %s@%d: trying to read at %zd which is get_at %zd\n", __FILE__, __LINE__, at, get_at);
            if (get_at >= put_at)
            {
printf(".......... %s@%d: cannot read at %zd because put_at is %zd\n", __FILE__, __LINE__, get_at, put_at);
                return false;
            }
printf(".......... %s@%d: reading at %zd\n", __FILE__, __LINE__, at);
            value = buffer[get_at % size];
            return true;
        }

        void CircularBuffer::push(T value)
        {
            std::lock_guard<std::mutex> lock(sync);
printf(".......... %s@%d: pushing at %zd\n", __FILE__, __LINE__, put_at);
            buffer[put_at % size] = value;
            put_at++;
        }
        void CircularBuffer::wipe()
        {
            std::lock_guard<std::mutex> lock(sync);
            size_t get_at = put_at - size;
            if (get_at < 0)
                get_at = 0;
            for (int64_t at=get_at; at<put_at; at++)
                buffer[at % size].reset();

            put_at = 0;
        }

    }
}

