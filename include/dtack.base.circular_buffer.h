#pragma once

namespace dtack
{
    namespace base
    {
        extern void *allocate_from_heap(const char *caller, const char *purpose, int count, int size);

		// ----------------------------------------------------------------
		template<typename T> class CircularBuffer
		{
            public:
                CircularBuffer(uint64_t size):
                    size(size),
                    put_at(0),
                    buffer(NULL)
                {
                    buffer = (T *)allocate_from_heap("CircularBuffer", "buffer", size, sizeof(T));
                    // const char *F = "CircularBuffer"; 
                    // printf(".......... %s: constructing with %zd items of size %zd at0x%08x\n", F, size, sizeof(T), buffer);
                };

                virtual ~CircularBuffer()
                {
                    wipe();

                    // const char *F = "~CircularBuffer"; 
                    // printf(".......... %s: destructing buffer at 0x%08x\n", F, buffer);
                    if (buffer != NULL)
                        free(buffer);
                }

                virtual bool read(uint64_t at, T &value)
                {
                    std::lock_guard<std::mutex> lock(read_write_mutex);            
                    size_t get_at = at;
                    if (put_at >= size)
                        get_at = put_at - size + at;
                    
//printf(".......... %s@%d: trying to read at %zd which is get_at %zd\n", __FILE__, __LINE__, at, get_at);
                    if (get_at >= put_at)
                    {
//printf(".......... %s@%d: cannot read at %zd because put_at is %zd\n", __FILE__, __LINE__, get_at, put_at);
                        return false;
                    }
//printf(".......... %s@%d: reading at %zd\n", __FILE__, __LINE__, at);
                    value = buffer[get_at % size];
// printf(".......... %s@%d: after reading at %zd, use count is %zd\n", __FILE__, __LINE__, at, value.use_count());
                    return true;
                };

                virtual void push(T value)
                {
                    std::lock_guard<std::mutex> lock(read_write_mutex);            
//printf(".......... %s@%d: pushing at %zd\n", __FILE__, __LINE__, put_at);
// printf(".......... %s@%d: before pushing at %zd, use count is %zd\n", __FILE__, __LINE__, put_at, value.use_count());
                    if (put_at >= size)
                    {
                        buffer[put_at % size].reset();
                    }
                    buffer[put_at % size] = value;
// printf(".......... %s@%d: after pushing at %zd, use count is %zd\n", __FILE__, __LINE__, put_at, value.use_count());
                    put_at++;
                };
                virtual void wipe()
                {
                    std::lock_guard<std::mutex> lock(read_write_mutex);            
                    size_t get_at = 0;
                    if (put_at > size)
                        get_at = put_at - size;
                    for (uint64_t at=get_at; at<put_at; at++)
                        buffer[at % size].reset();
                    put_at = 0;
                };

            private:
                uint64_t size;
                uint64_t put_at;
                T *buffer;
                std::mutex read_write_mutex;

		};
    }
}

