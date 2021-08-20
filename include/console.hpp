#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#define VALID_COMMAND_MSG "OK"
#define GENERIC_INVALID_COMMAND_MSG "ERROR"

#include <string>
#include <vector>
#include <iostream>

#include "circuit.hpp"

class console{
    private:
        circuit& m_circuit;
        std::ostream& m_os;

        std::vector<std::string> split_string_in_substrings(std::string input, const std::string& delimiters);
        int validate_uint(const std::string& input_str, size_t& ouput_uint, const std::string& error_msg);
        int validate_bool(const std::string& input_str, bool& ouput_bool, const std::string& error_msg);

        void print_help(const std::vector<std::string>& command_and_args);
        void set_num_io(const std::vector<std::string>& command_and_args);
        void add_layer(const std::vector<std::string>& command_and_args);
        void add_gate(const std::vector<std::string>& command_and_args);
        void add_connection(const std::vector<std::string>& command_and_args);
        void delete_layer(const std::vector<std::string>& command_and_args);
        void delete_gate(const std::vector<std::string>& command_and_args);
        void delete_connection(const std::vector<std::string>& command_and_args);
        void delete_connections_to_gate_inputs(const std::vector<std::string>& command_and_args);
        void delete_connections_from_gate_outputs(const std::vector<std::string>& command_and_args);
        void delete_all_connections_of_gate(const std::vector<std::string>& command_and_args);
        void bulk_add_layer(const std::vector<std::string>& command_and_args);
        void bulk_add_gate(const std::vector<std::string>& command_and_args);
        void bulk_add_connection(const std::vector<std::string>& command_and_args);
        void bulk_delete_layer(const std::vector<std::string>& command_and_args);
        void bulk_delete_gate(const std::vector<std::string>& command_and_args);
        void bulk_delete_connection(const std::vector<std::string>& command_and_args);
        void set_inputs(const std::vector<std::string>& command_and_args);
        void read_outputs(const std::vector<std::string>& command_and_args);
        void simulate_circuit(const std::vector<std::string>& command_and_args);
        void gen_truth_table(const std::vector<std::string>& command_and_args);
        void print_circuit(const std::vector<std::string>& command_and_args);
        void list_unconnected(const std::vector<std::string>& command_and_args);
        void save_circuit(const std::vector<std::string>& command_and_args);
        void load_circuit(const std::vector<std::string>& command_and_args);

    public:
        console(circuit& c, std::ostream& os) : m_circuit(c), m_os(os) {};
        ~console() {};

        void execute_command(const std::string& user_input);
};

#endif