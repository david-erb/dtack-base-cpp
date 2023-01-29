namespace dtack 
{ 
    namespace base
    {
        namespace bytes_reader
        {

            class base_c: public interface_i, public dtack::base::base_c
            {
                public:
                    base_c(dtack::base::environment_c *environment);
                    ~base_c();
                    virtual void configure(dtack::base::Limn *limn);
                    virtual void open();
                    virtual void read(char *data, int size);
                    virtual void close();

                protected:
                    dtack::base::Limn *limn;
            }; // end class

        } // end namespace

    } // end namespace

} // end namespace