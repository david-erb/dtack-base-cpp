namespace dtack
{
    namespace base
    {
        namespace limn
        {
            namespace packets
            {
                class Limn: public base_c, public dtack::base::Limn
                {
                    public:
                        Limn(environment_c *environment);
                        ~Limn();
                        virtual void load_from_cli(int argc, char **argv);
                        virtual void load_from_json_string(std::string json_string);
                        virtual void load_from_json_file(std::string json_filename);
                        virtual dtack::base::Limn *section(const char *name, int *was_found = NULL);
                        virtual int peek_log_level();
                        virtual std::vector<std::string> get_scalar_names();

                        virtual std::string peek(const char *name, std::string default_value);
                        virtual std::string peek(std::string, std::string default_value);
                        virtual std::string peek(const char *name, int *exists, std::string default_value);
                        virtual std::string peek(std::string, int *exists, std::string default_value);

                        virtual int peek(const char *name, int default_value);
                        virtual int peek(std::string, int default_value);
                        virtual int peek(const char *name, int *exists, int default_value);
                        virtual int peek(std::string, int *exists, int default_value);

                        virtual double peek(const char *name, double default_value);
                        virtual double peek(std::string, double default_value);
                        virtual double peek(const char *name, int *exists, double default_value);
                        virtual double peek(std::string, int *exists, double default_value);

                        virtual void poke(std::string name, dtack::base::Limn *limn);
                        virtual void poke(const char *name, dtack::base::Limn *limn);
                        virtual void poke(const char *name, std::string value);
                        virtual void poke(std::string name, std::string value);
                        virtual void poke(const char *name, int value);
                        virtual void poke(std::string name, int value);
                        virtual void poke(const char *name, double value);
                        virtual void poke(std::string name, double value);
                        virtual std::string to_json_string(bool should_indent = true);

                    protected:
                        int log_level = 0;
                        std::shared_ptr<dtack::base::packet_c> packet;
                }; // end class
            } // end namespace
        } // end namespace
    } // end namespace
} // end namespace
