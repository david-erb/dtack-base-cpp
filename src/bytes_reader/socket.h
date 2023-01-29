namespace dtack 
{ 
    namespace base
    {
        namespace bytes_reader
        {
            class socket_c: public base_c
            {
                public:
                    socket_c(dtack::base::environment_c *environment);
                    ~socket_c();
                    virtual void open();
                    virtual void read(char *data, size_t size);
                    virtual void close();

            }; // end class

        } // end namespace

    } // end namespace

} // end namespace
