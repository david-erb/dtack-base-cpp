namespace dtack 
{ 
    namespace base
    {
        namespace bytes_reader
        {

            class binary_file_c: public base_c
            {
                public:
                    binary_file_c(dtack::base::environment_c *environment);
                    ~binary_file_c();
                    virtual void open();
                    virtual void read(char *data, size_t size);
                    virtual void close();

                private:
                    std::ifstream stream;
            }; // end class

        } // end namespace

    } // end namespace
    
} // end namespace