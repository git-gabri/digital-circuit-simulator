#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "gates.hpp"

class circuit{
    private:
        struct layer{
            std::map<size_t, gate> m_gates;
        };
        
        struct connection{           
            size_t m_uid_output;
            bool m_inv_output;
            size_t m_uid_input;
            bool m_num_input;

            connection(const size_t& uid_out, const bool& inv, const size_t& uid_in, const bool& num_in) :
                m_uid_output(uid_out),
                m_inv_output(inv),
                m_uid_input(uid_in),
                m_num_input(num_in)
            {}
        };

        std::vector<bool> m_inputs;
        std::vector<bool> m_outputs;
        std::map<size_t, layer> m_layers;
        std::map<size_t, size_t> m_gates_in_layers; 
        std::vector<connection> m_connections;
        std::vector<connection> m_phantom_connections;  //Used only when loading a circuit from file. Phantom because it doesn't affect the gates

        size_t m_next_gate_uid;

        void set_all_gates_to_status(const status& s);
        std::string gate_type_to_str(const gate_type& g);
        int add_gate_with_uid(const size_t& uid, const gate& g, const size_t& num_layer);
        int add_phantom_connection(const size_t& gate_out_uid, const bool& take_inv_output, const size_t& gate_in_uid, const bool& num_input);

    public:
        circuit(const size_t& num_inputs, const size_t& num_outputs);
        ~circuit();

        void set_io(const size_t& num_inputs, const size_t& num_outputs);

        size_t num_inputs() const {return m_inputs.size();}
        size_t num_outputs() const {return m_outputs.size();}

        int add_layer(const size_t& num_layer);
        int add_gate(const gate& g, const size_t& num_layer);
        int add_connection(const size_t& gate_out_uid, const size_t& gate_in_uid, const bool& num_input);
        int add_connection(const size_t& gate_out_uid, const bool& take_inv_output, const size_t& gate_in_uid, const bool& num_input);
        int add_connection(const size_t& num_layer_output, const size_t& gate_out_uid, const size_t& num_layer_input, const size_t& gate_in_uid, const bool& num_input);
        int add_connection(const size_t& num_layer_output, const size_t& gate_out_uid, const bool& take_inv_output, const size_t& num_layer_input, const size_t& gate_in_uid, const bool& num_input);

        int delete_layer(const size_t& num_layer);
        int delete_gate(const size_t& uid);
        int delete_gate(const size_t& uid, const size_t& num_layer);
        int delete_connections_to_gate_inputs(const size_t& uid);
        int delete_connections_to_gate_inputs(const size_t& uid, const size_t& num_layer);
        int delete_connections_from_gate_outputs(const size_t& uid);
        int delete_connections_from_gate_outputs(const size_t& uid, const size_t& num_layer);
        int delete_connections_to_and_from_gate(const size_t& uid);
        int delete_connections_to_and_from_gate(const size_t& uid, const size_t& num_layer);
        int delete_connection(const size_t& gate_in_uid, const bool& num_input);
        int delete_connection(const size_t& gate_out_uid, const size_t& gate_in_uid, const bool& num_input);
        int delete_connection(const size_t& gate_out_uid, const bool& take_inv_output, const size_t& gate_in_uid, const bool& num_input);
        int delete_connection(const size_t& num_layer_output, const size_t& gate_out_uid, const bool& take_inv_output, const size_t& num_layer_input, const size_t& gate_in_uid, const bool& num_input);

        int set_inputs(const std::vector<bool>& inputs);
        std::vector<bool> read_inputs() const {return m_inputs;}
        std::vector<bool> read_outputs();

        int simulate_circuit(const std::vector<bool>& inputs);
        int simulate_circuit();
        int gen_truth_table(std::ostream& os = std::cout);

        void print_circuit(const bool& print_gates = true, const bool& print_connections = true, std::ostream& os = std::cout);
        void list_unconnected(std::ostream& os = std::cout);

        int save_circuit_to_file(const std::string& filename);
        int load_circuit_from_file(const std::string& filename);

        int regen_connection_vector();
};

#endif