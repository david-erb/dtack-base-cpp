#pragma once

namespace dtack
{
    namespace base
    {
        namespace bytes_reader
        {
            // ----------------------------------------------------------------
            class interface_i: public dtack::base::diwec_log_producer_c
            {
                public:
                    virtual ~interface_i() = 0;
                    virtual void configure(dtack::base::Limn *limn) = 0;
                    virtual void open() = 0;
                    virtual void read(char *data, size_t size) = 0;
                    virtual void close() = 0;

                public:
                
            }; // end class

            // ----------------------------------------------------------------
            class factory_c: public dtack::base::base_c
            {
                public: 
                    factory_c(dtack::base::environment_c *environment);
                    interface_i *manufacture(std::string class_name);
                    interface_i *manufacture(const char *class_name);

            }; // end class

            // ----------------------------------------------------------------
            class eof_exception_c: std::exception
            {
                public: 
                    eof_exception_c() {}
            };
        } // end namespace

    } // end namespace

} // end namespace
