#include "circuit.hpp"
#include "gates.hpp"

#include <map>
#include <utility>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>

using namespace std;

//------------------------------------------------------------------------------------------------------------------------------------
//Circuit constructor
circuit::circuit(const size_t& num_inputs, const size_t& num_outputs){
    m_inputs = vector<bool>(num_inputs, false);
    m_outputs = vector<bool>(num_outputs, false);

    m_next_gate_uid = 0;

    m_layers.emplace(make_pair(0, layer()));
    for(size_t i = 0; i < num_inputs + 2; ++i){
        m_layers[0].m_gates.emplace(make_pair(m_next_gate_uid, gate(gate_type::buffer, m_next_gate_uid)));
        m_gates_in_layers.emplace(make_pair(m_next_gate_uid, 0));
        ++m_next_gate_uid;
    }

    m_layers.emplace(make_pair(-1, layer()));
    for(size_t i = 0; i < num_outputs; ++i){
        m_layers[-1].m_gates.emplace(make_pair(m_next_gate_uid, gate(gate_type::buffer, m_next_gate_uid)));
        m_gates_in_layers.emplace(make_pair(m_next_gate_uid, -1));
        ++m_next_gate_uid;
    }
}

//------------------------------------------------------------------------------------------------------------------------------------
//Circuit destructor
circuit::~circuit() {}

//------------------------------------------------------------------------------------------------------------------------------------
//Set inputs and outputs of the circuit
void circuit::set_io(const size_t& num_inputs, const size_t& num_outputs){
    *this = circuit(num_inputs, num_outputs);
}

//------------------------------------------------------------------------------------------------------------------------------------
//Private members

//Function to set the status of all the gates in the circuit to a specified status
void circuit::set_all_gates_to_status(const status& s){
    for(auto& l : m_layers){
        for(auto& g : l.second.m_gates)
            g.second.gate_status = s;
    }
}

//Function to convert the gate type to a string
string circuit::gate_type_to_str(const gate_type& g){
    string ret;

    switch(g){
        case gate_type::buffer:
            ret = "BUF";
            break;
        
        case gate_type::not_gate:
            ret = "NOT";
            break;

        case gate_type::and_gate:
            ret = "AND";
            break;

        case gate_type::or_gate:
            ret = " OR";
            break;

        case gate_type::xor_gate:
            ret = "XOR";
            break;

        case gate_type::nand_gate:
            ret = "NND";
            break;

        case gate_type::nor_gate:
            ret = "NOR";
            break;

        case gate_type::nxor_gate:
            ret = "NXR";
            break;
    }

    return ret;
}

//Function to add a gate specifying also its uid (use with caution)
int circuit::add_gate_with_uid(const size_t& uid, const gate& g, const size_t& num_layer){
    if(num_layer != 0 && num_layer != static_cast<size_t>(-1) && m_layers.contains(num_layer)){
        if(m_gates_in_layers.contains(uid))
            return 2;

        m_layers[num_layer].m_gates[uid] = gate(g.type, uid);
        m_gates_in_layers[uid] = num_layer;
        return 0;
    } else
        return 1;
}

//Function to add a phantom connection, i.e. a connection that doesn't affect the gates (use with caution)
int circuit::add_phantom_connection(const size_t& gate_out_uid, const bool& take_inv_output, const size_t& gate_in_uid, const bool& num_input){
    //Check input validity
    if(!m_gates_in_layers.contains(gate_out_uid) || !m_gates_in_layers.contains(gate_in_uid))
        return -1;

    if(!(m_gates_in_layers[gate_out_uid] < m_gates_in_layers[gate_in_uid]))
        return 3;

    //Update the vector containing info on the connections in the circuit
    auto it_phantom_connections = find_if(m_phantom_connections.begin(), m_phantom_connections.end(),
                                          [&](const connection& c) -> bool{
                                          return c.m_uid_input == gate_in_uid && c.m_num_input == num_input;
                                          });

    if(it_phantom_connections != m_phantom_connections.end())
        m_phantom_connections.erase(it_phantom_connections);
    m_phantom_connections.emplace_back(gate_out_uid, take_inv_output, gate_in_uid, num_input);

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------------
//Methods to add elements to the circuit

//Add layer of gates to the circuit
int circuit::add_layer(const size_t& num_layer){
    if(m_layers.contains(num_layer))
        return 1;
    else {
        m_layers.emplace(make_pair(num_layer, layer()));
        return 0;
    }
}

//Add gate in an existing layer of the circuit. It must not be the input nor the output layer
int circuit::add_gate(const gate& g, const size_t& num_layer){
    if(num_layer != 0 && num_layer != static_cast<size_t>(-1) && m_layers.contains(num_layer)){
        m_layers[num_layer].m_gates.emplace(make_pair(m_next_gate_uid, gate(g.type, m_next_gate_uid)));
        m_gates_in_layers.emplace(make_pair(m_next_gate_uid, num_layer));
        ++m_next_gate_uid;
        return 0;
    } else
        return 1;
}

//Add a connection to two gates by just specifying the uids of the two gates and the input number (0 or 1) of the receiving gate
int circuit::add_connection(const size_t& gate_out_uid, const size_t& gate_in_uid, const bool& num_input){
    return add_connection(gate_out_uid, false, gate_in_uid, num_input);
}

//Add a connection to two gates by just specifying the uids of the two gates, whether to take the inverted input of the giving gate, and the input number (0 or 1) of the receiving gate
int circuit::add_connection(const size_t& gate_out_uid, const bool& take_inv_output, const size_t& gate_in_uid, const bool& num_input){
    if(!m_gates_in_layers.contains(gate_out_uid) || !m_gates_in_layers.contains(gate_in_uid))
        return -1;
    else
        return add_connection(m_gates_in_layers[gate_out_uid], gate_out_uid, take_inv_output, m_gates_in_layers[gate_in_uid], gate_in_uid, num_input);
}

//Add a connection to two gates by specifying the uids of the gates, that layer in which they're in, and the input number (0 or 1) of the receiving gate
int circuit::add_connection(const size_t& num_layer_output, const size_t& gate_out_uid, const size_t& num_layer_input, const size_t& gate_in_uid, const bool& num_input){
    return add_connection(num_layer_output, gate_out_uid, false, num_layer_input, gate_in_uid, num_input);
}

//Add a connection to two gates by specifying the uids of the gates, the layer in which they're in, whether to take the inverted input of the giving gate, and the input number (0 or 1) of the receiving gate
int circuit::add_connection(const size_t& num_layer_output, const size_t& gate_out_uid, const bool& take_inv_output, const size_t& num_layer_input, const size_t& gate_in_uid, const bool& num_input){
    //Check input validity
    if(!m_gates_in_layers.contains(gate_out_uid) || !m_gates_in_layers.contains(gate_in_uid))
        return -1;

    if(!m_layers.contains(num_layer_output) || !m_layers.contains(num_layer_input))
        return 1;

    if(!m_layers[num_layer_output].m_gates.contains(gate_out_uid) || !m_layers[num_layer_input].m_gates.contains(gate_in_uid))
        return 2;

    if(!(num_layer_output < num_layer_input))
        return 3;

    //Connect the gates to one another
    gate* ptr_gate = &(m_layers[num_layer_output].m_gates[gate_out_uid]);
    if(num_input == 0){
        m_layers[num_layer_input].m_gates[gate_in_uid].ptr_gate_in0 = ptr_gate;
        m_layers[num_layer_input].m_gates[gate_in_uid].take_inv_output_in_in0 = take_inv_output;
    } else {
        m_layers[num_layer_input].m_gates[gate_in_uid].ptr_gate_in1 = ptr_gate;
        m_layers[num_layer_input].m_gates[gate_in_uid].take_inv_output_in_in1 = take_inv_output;
    }

    //Update the vector containing info on the connections in the circuit
    auto it_connections = find_if(m_connections.begin(), m_connections.end(),
                                  [&](const connection& c) -> bool{
                                      return c.m_uid_input == gate_in_uid && c.m_num_input == num_input;
                                  });

    if(it_connections != m_connections.end())
        m_connections.erase(it_connections);
    m_connections.emplace_back(gate_out_uid, take_inv_output, gate_in_uid, num_input);

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------------
//Methods to delete elements from the circuit

//Delete an entire layer and all of its gates
int circuit::delete_layer(const size_t& num_layer){
    if(!m_layers.contains(num_layer))
        return 1;

    if(num_layer == 0 || num_layer == static_cast<size_t>(-1))
        return 2;

    vector<size_t> uids_gate_to_delete;
    for(const auto& g : m_gates_in_layers){
        if(g.second == num_layer)
            uids_gate_to_delete.push_back(g.first);
    }

    for(const auto& uid : uids_gate_to_delete)
        delete_gate(uid);

    if(!m_layers[num_layer].m_gates.empty())
        return 3;

    m_layers.extract(num_layer);
    return 0;
}

//Delete a gate by just specifying its uid
int circuit::delete_gate(const size_t& uid){
    if(!m_gates_in_layers.contains(uid))
        return 1;

    return delete_gate(uid, m_gates_in_layers[uid]);
}

//Delete a gate by specifying its uid and the layer it's in
int circuit::delete_gate(const size_t& uid, const size_t& num_layer){
    if(!m_gates_in_layers.contains(uid))
        return 1;
    
    if(m_gates_in_layers[uid] != num_layer)
        return 2;

    if(num_layer == 0 || num_layer == static_cast<size_t>(-1))
        return 3;
    
    //Delete all the connections first
    for(auto it_conn = m_connections.begin(); it_conn < m_connections.end(); ++it_conn){
        //If the connection specifies that the gate's input was connected somewhere, it's not a big deal
        if(it_conn->m_uid_input == uid){
            it_conn = m_connections.erase(it_conn);
        }

        //If the connection specifies that the gate's outpus was connected somewhere, then we have to set
        //the other gates' pointer to nullptr
        if(it_conn->m_uid_output == uid){
            if(it_conn->m_num_input == 0)
                m_layers[m_gates_in_layers[it_conn->m_uid_input]].m_gates[it_conn->m_uid_input].ptr_gate_in0 = nullptr;
            if(it_conn->m_num_input == 1)
                m_layers[m_gates_in_layers[it_conn->m_uid_input]].m_gates[it_conn->m_uid_input].ptr_gate_in1 = nullptr;

            it_conn = m_connections.erase(it_conn);
        }
    }

    //Delete the gate from the circuit
    m_layers[num_layer].m_gates.extract(uid);

    //Delete the information of the gate from "m_gates_in_layers"
    m_gates_in_layers.extract(uid);

    return 0;
}

//Delete all the connections to a gate inputs by just specifying its uid
int circuit::delete_connections_to_gate_inputs(const size_t& uid){
    if(!m_gates_in_layers.contains(uid))
        return 1;

    return delete_connections_to_gate_inputs(uid, m_gates_in_layers[uid]);
}

//Delete all the connections to a gate inputs by specifying its uid and the layer it's in
int circuit::delete_connections_to_gate_inputs(const size_t& uid, const size_t& num_layer){
    if(!m_gates_in_layers.contains(uid))
        return 1;
    
    if(m_gates_in_layers[uid] != num_layer)
        return 2;

    delete_connection(uid, 0);
    delete_connection(uid, 1);

    return 0;
}

//Delete all the connections from a gate's outputs by just specifying its uid
int circuit::delete_connections_from_gate_outputs(const size_t& uid){
    if(!m_gates_in_layers.contains(uid))
        return 1;

    return delete_connections_from_gate_outputs(uid, m_gates_in_layers[uid]);
}

//Delete all the connections from a gate's outputs by specifying its uid and the layer it's in
int circuit::delete_connections_from_gate_outputs(const size_t& uid, const size_t& num_layer){
    if(!m_gates_in_layers.contains(uid))
        return 1;
    
    if(m_gates_in_layers[uid] != num_layer)
        return 2;

    for(auto it_conn = m_connections.begin(); it_conn < m_connections.end(); ++it_conn){
        if(it_conn->m_uid_output == uid){
            if(it_conn->m_num_input == 0)
                m_layers[m_gates_in_layers[it_conn->m_uid_input]].m_gates[it_conn->m_uid_input].ptr_gate_in0 = nullptr;
            if(it_conn->m_num_input == 1)
                m_layers[m_gates_in_layers[it_conn->m_uid_input]].m_gates[it_conn->m_uid_input].ptr_gate_in1 = nullptr;

            it_conn = m_connections.erase(it_conn);
        }
    }
    return 0;
}

//Delete all the connections to a gate's inputs and outputs by just specifying its uid
int circuit::delete_connections_to_and_from_gate(const size_t& uid){
    return delete_connections_to_gate_inputs(uid) + 10 * delete_connections_from_gate_outputs(uid);
}

//Delete all the connections to a gate's inputs and outputs by specifying its uid and the layer it's in
int circuit::delete_connections_to_and_from_gate(const size_t& uid, const size_t& num_layer){
    return delete_connections_to_gate_inputs(uid, num_layer) + 10 * delete_connections_from_gate_outputs(uid, num_layer);
}

//Delete a connection to an input of a gate by just specifying the gate's uid and its input number
int circuit::delete_connection(const size_t& gate_in_uid, const bool& num_input){
    if(!m_gates_in_layers.contains(gate_in_uid))
        return 1;
    
    for(auto it_conn = m_connections.begin(); it_conn < m_connections.end(); ++it_conn){
        if(it_conn->m_uid_input == gate_in_uid && it_conn->m_num_input == num_input){
            if(num_input == 0)
                m_layers[m_gates_in_layers[gate_in_uid]].m_gates[gate_in_uid].ptr_gate_in0 = nullptr;
            if(num_input == 1)
                m_layers[m_gates_in_layers[gate_in_uid]].m_gates[gate_in_uid].ptr_gate_in1 = nullptr;

            it_conn = m_connections.erase(it_conn);
        }
    }

    return 0;
}

int circuit::delete_connection(const size_t& gate_out_uid, const size_t& gate_in_uid, const bool& num_input){
    return delete_connection(gate_out_uid, false, gate_in_uid, num_input);
}

int circuit::delete_connection(const size_t& gate_out_uid, const bool& take_inv_output, const size_t& gate_in_uid, const bool& num_input){
    if(!m_gates_in_layers.contains(gate_in_uid) || !m_gates_in_layers.contains(gate_out_uid))
        return 1;

    return delete_connection(m_gates_in_layers[gate_out_uid], gate_out_uid, take_inv_output, m_gates_in_layers[gate_in_uid], gate_in_uid, num_input);
}

int circuit::delete_connection(const size_t& num_layer_output, const size_t& gate_out_uid, const bool& take_inv_output, const size_t& num_layer_input, const size_t& gate_in_uid, const bool& num_input){
    if(!m_gates_in_layers.contains(gate_in_uid) || !m_gates_in_layers.contains(gate_out_uid))
        return 1;

    if(m_gates_in_layers[gate_out_uid] != num_layer_output || m_gates_in_layers[gate_in_uid] != num_layer_input)
        return 2;

    for(auto it_conn = m_connections.begin(); it_conn < m_connections.end(); ++it_conn){
        if(it_conn->m_uid_output == gate_out_uid && it_conn->m_inv_output == take_inv_output && it_conn->m_uid_input == gate_in_uid && it_conn->m_num_input == num_input){
            if(num_input == 0)
                m_layers[m_gates_in_layers[gate_in_uid]].m_gates[gate_in_uid].ptr_gate_in0 = nullptr;
            if(num_input == 1)
                m_layers[m_gates_in_layers[gate_in_uid]].m_gates[gate_in_uid].ptr_gate_in1 = nullptr;

            it_conn = m_connections.erase(it_conn);
        }
    }

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------------
//Methods to interact with the inputs and outputs of the circuit

//Set inputs to specified values
int circuit::set_inputs(const vector<bool>& inputs){
    if(inputs.size() != m_inputs.size())
        return 1;

    m_inputs = inputs;

    set_all_gates_to_status(status::to_update);

    //Set the first two gates to be constants false and true
    m_layers[0].m_gates[0].output = false;
    m_layers[0].m_gates[0].inv_output = true;
    m_layers[0].m_gates[0].gate_status = status::updated;
    m_layers[0].m_gates[1].output = true;
    m_layers[0].m_gates[1].inv_output = false;
    m_layers[0].m_gates[1].gate_status = status::updated;

    //Update all the other input gates
    for(size_t i = 0; i < inputs.size(); ++i){
        m_layers[0].m_gates[i + 2].output = m_inputs[i];
        m_layers[0].m_gates[i + 2].inv_output = !m_inputs[i];
        m_layers[0].m_gates[i + 2].gate_status = status::updated;
    }

    return 0;
}

//Read outputs and return them in a vector of bools
vector<bool> circuit::read_outputs(){
    size_t output_index = 0;

    for(auto it_last_layer = m_layers[-1].m_gates.begin(); it_last_layer != m_layers[-1].m_gates.end(); ++it_last_layer){
        m_outputs[output_index] = it_last_layer->second.output;
        ++output_index;
    }

    return m_outputs;
}

//------------------------------------------------------------------------------------------------------------------------------------
//Methods to simulate the circuit

//Function to simulate the circuit while specifying some inputs
int circuit::simulate_circuit(const vector<bool>& inputs){
    if(set_inputs(inputs))
        return 1;

    return simulate_circuit();
}

//Function to simulate the circuit, layer by layer
int circuit::simulate_circuit(){
    for(auto& l : m_layers){
        for(auto& g : l.second.m_gates)
            if(g.second.calc_output()){
                return 1;
            }
    }

    return 0;    
}

//Function to repeatedly simulate the circuit with every possible input, generating the truth table,
//and "printing" the specified results on the specified ostream
int circuit::gen_truth_table(ostream& os){
    auto inputs_to_restore = m_inputs;
    auto outputs_to_restore = m_outputs;

    vector<bool> current_input = vector<bool>(m_inputs.size(), 0);

    do{
        set_inputs(current_input);
        
        if(simulate_circuit())
            return 1;

        for(const auto& b : current_input)
            os << b;
        os << " | ";

        for(const auto& b : read_outputs())
            os << b;
        os << endl;

        bool increment_next_bit = true;
        for(size_t i = 0; increment_next_bit && i < current_input.size(); ++i){
            if(current_input[i] == 0){
                current_input[i] = 1;
                increment_next_bit = false;
            }
            else{
                current_input[i] = 0;
                increment_next_bit = 1;
            }
        }
    }while(current_input != vector<bool>(m_inputs.size(), 0));

    set_inputs(inputs_to_restore);
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------------
//Methods to write text data which represents the circuit

//Function to "print" the circuit in the specified ostream
//This lists all the gates and all their types, but not the connections
void circuit::print_circuit(const bool& print_gates, const bool& print_connections, std::ostream& os){
    if(print_gates){
        os << "Inputs  : " << m_inputs.size() << " (+2)" << endl;
        os << "Outputs : " << m_outputs.size() << endl;
    }
    if(!print_gates && print_connections)
        os << "Connections" << endl;

    for(const auto& l : m_layers){
        if(l.first == 0)
            os << "Input layer:" << endl;
        else if(l.first == static_cast<size_t>(-1))
            os << "Output layer:" << endl;
        else
            os << "Layer " << l.first << ":" << endl;
        
        for(const auto& p : l.second.m_gates){
            os << "    " << gate_type_to_str(p.second.type) << " (" << p.second.uid_gate << ")" << (l.first != 0 && print_connections ? ":" : "") << endl;

            if(l.first != 0 && print_connections){
                if(p.second.type == gate_type::buffer || p.second.type == gate_type::not_gate){
                    const int unconnected_inputs = (p.second.ptr_gate_in0 == nullptr) + (p.second.ptr_gate_in1 == nullptr);
                    if(unconnected_inputs == 2){
                        os << "        " << "in_0: nc" << endl;
                        os << "        " << "in_1: nc" << endl;
                    }
                    else {
                        if(p.second.ptr_gate_in0 != nullptr)
                            os << "        " << "in_0: " << (p.second.take_inv_output_in_in0 ? "!" : "") + to_string(p.second.ptr_gate_in0->uid_gate) << endl;
                        if(p.second.ptr_gate_in1 != nullptr)
                            os << "        " << "in_1: " << (p.second.take_inv_output_in_in1 ? "!" : "") + to_string(p.second.ptr_gate_in1->uid_gate) << endl;
                    }
                }
                else {
                    os << "        " << "in_0: " << (p.second.ptr_gate_in0 == nullptr ? "nc" : (p.second.take_inv_output_in_in0 ? "!" : "") + to_string(p.second.ptr_gate_in0->uid_gate)) << endl;
                    os << "        " << "in_1: " << (p.second.ptr_gate_in1 == nullptr ? "nc" : (p.second.take_inv_output_in_in1 ? "!" : "") + to_string(p.second.ptr_gate_in1->uid_gate)) << endl;
                }
            }
        }
    }
}

//Function to "print" all the unconnected gates in the specified ostream
void circuit::list_unconnected(ostream& os){
    os << "Unconnected gates" << endl;

    for(const auto& l : m_layers){
        if(l.first == 0)
            continue;

        if(l.first == static_cast<size_t>(-1))
            os << "Output layer:" << endl;
        else
            os << "Layer " << l.first << ":" << endl;

        bool unconnected_gates = false;

        for(const auto& p : l.second.m_gates){
            if(p.second.type == gate_type::buffer || p.second.type == gate_type::not_gate){
                if(p.second.ptr_gate_in0 == nullptr && p.second.ptr_gate_in1 == nullptr){
                    os << "    " << gate_type_to_str(p.second.type) << " (" << p.second.uid_gate << ")" << endl;
                    unconnected_gates = true;
                }
            }
            else {
                if(p.second.ptr_gate_in0 == nullptr){
                    os << "    " << gate_type_to_str(p.second.type) << " (" << p.second.uid_gate << ") - input 0" << endl;
                    unconnected_gates = true;
                }
                if(p.second.ptr_gate_in1 == nullptr){
                    os << "    " << gate_type_to_str(p.second.type) << " (" << p.second.uid_gate << ") - input 1" << endl;
                    unconnected_gates = true;
                }
            }
        }

        if(!unconnected_gates)
            os << "    OK" << endl; 
    }
}

//------------------------------------------------------------------------------------------------------------------------------------
//Methods to save and load a circuit

int circuit::save_circuit_to_file(const std::string& filename){
    ofstream out_file(filename);

    if(!out_file.is_open())
        return 1;

    //Write number of inputs and outputs to file
    out_file << "I " << m_inputs.size() << "\n";
    out_file << "O " << m_outputs.size() << "\n";

    //Write layers to file
    for(const auto& l : m_layers){
        if(l.first != 0 && l.first != static_cast<size_t>(-1))
            out_file << "L " << l.first << "\n";
    }

    //Write gates in layers to file
    for(const auto& l : m_layers){
        if(l.first != 0 && l.first != static_cast<size_t>(-1)){
            for(const auto& g : l.second.m_gates){
                out_file << "G " << g.second.uid_gate << " " << gate_type_to_str(g.second.type) << " " << l.first << "\n";
            }
        }
    }

    //Write connections between gates to file
    for(const auto& l : m_layers){
        if(l.first != 0){
            for(const auto& g : l.second.m_gates){
                if(g.second.ptr_gate_in0 != nullptr)
                    out_file << "C " << g.second.ptr_gate_in0->uid_gate << " " << g.second.take_inv_output_in_in0 << " " << g.second.uid_gate << " 0\n";
                if(g.second.ptr_gate_in1 != nullptr)
                    out_file << "C " << g.second.ptr_gate_in1->uid_gate << " " << g.second.take_inv_output_in_in1 << " " << g.second.uid_gate << " 1\n";
            }
        }
    }

    out_file.close();

    return 0;
}

int circuit::load_circuit_from_file(const std::string& filename){
    ifstream in_file(filename);

    if(!in_file.is_open())
        return 1;

    string line;
    circuit loaded_circuit(4, 4);
    const regex num_inputs_re(R"foo(^I \d+$)foo");
    const regex num_outputs_re(R"foo(^O \d+$)foo");
    const regex layer_re(R"foo(^L \d+$)foo");
    const regex gate_re(R"foo(^G \d+ (?:BUF|NOT|AND| OR|XOR|NND|NOR|NXR) \d+$)foo");
    const regex connection_re(R"foo(^C \d+ \d+ \d+ \d+$)foo");

    //Read number of inputs and outputs from file
    if(!getline(in_file, line))
        return 2;
    if(!regex_match(line, num_inputs_re))
        return 20;
    const size_t num_inputs_tmp = stoull(line.substr(2));
    if(!getline(in_file, line))
        return 3;
    if(!regex_match(line, num_outputs_re))
        return 30;
    const size_t num_outputs_tmp = stoull(line.substr(2));
    loaded_circuit.set_io(num_inputs_tmp, num_outputs_tmp);

    //Read rest of the data from the file
    bool reading_layers = true;
    bool reading_gates = false;
    bool reading_connections = false;
    size_t highest_gate_uid = num_inputs_tmp + 2 + num_outputs_tmp - 1;
    while(getline(in_file, line)){
        const bool matches_layer = regex_match(line, layer_re);
        const bool matches_gate = regex_match(line, gate_re);
        const bool matches_connection = regex_match(line, connection_re);
 
        //If the line doesn't match any valid syntax, return with error code
        if(!matches_layer && !matches_gate && !matches_connection)
            return 4;
        
        //Return value from the function called based on the line type
        int ret_val_from_fn;
        //Code to handle layer addition
        if(matches_layer){
            //If we're not reading layers anymore, return with error code
            if(!reading_layers)
                return 40;

            ret_val_from_fn = loaded_circuit.add_layer(stoull(line.substr(2)));
        }
        //Code to handle gate addition
        else if(matches_gate){
            //If we were reading layers, now update the booleans to signal that the layers are finished and we're reading gates
            if(reading_layers){
                reading_layers = false;
                reading_gates = true;
                reading_connections = false;
            }

            //If we're not reading gates anymore, return with error code
            if(!reading_gates)
                return 40;

            stringstream ss_line(line);
            char dummy_char;
            ss_line >> dummy_char;

            size_t gate_uid_tmp;
            string gate_type_str_tmp;
            size_t num_layer_tmp;
            ss_line >> gate_uid_tmp >> gate_type_str_tmp >> num_layer_tmp;

            if(gate_uid_tmp > highest_gate_uid)
                highest_gate_uid = gate_uid_tmp;

            gate_type gt = gate_type::buffer;
            if(gate_type_str_tmp == "BUF")
                gt = gate_type::buffer;
            else if(gate_type_str_tmp == "NOT")
                gt = gate_type::not_gate;
            else if(gate_type_str_tmp == "AND")
                gt = gate_type::and_gate;
            else if(gate_type_str_tmp == "OR")
                gt = gate_type::or_gate;
            else if(gate_type_str_tmp == "XOR")
                gt = gate_type::xor_gate;
            else if(gate_type_str_tmp == "NND")
                gt = gate_type::nand_gate;
            else if(gate_type_str_tmp == "NOR")
                gt = gate_type::nor_gate;
            else if(gate_type_str_tmp == "NXR")
                gt = gate_type::nxor_gate;

            ret_val_from_fn = loaded_circuit.add_gate_with_uid(gate_uid_tmp, gate(gt), num_layer_tmp);
        }
        //Code to handle connection addition
        else if(matches_connection){
            //If we were reading layers or gates, update the booleans to signal that the layers and/or gates are finished and we're reading connections
            if(reading_layers || reading_gates){
                reading_layers = false;
                reading_gates = false;
                reading_connections = true;
            }

            //If we're not reading connections anymore, return with error code
            //This will probably never get executed, but I'm not sure
            if(!reading_connections)
                return 40;

            stringstream ss_line(line);
            char dummy_char;
            ss_line >> dummy_char;

            size_t gate_out_uid_tmp;
            bool take_inv_output_tmp;
            size_t gate_in_uid_tmp;
            bool num_input_tmp;
            ss_line >> gate_out_uid_tmp >> take_inv_output_tmp >> gate_in_uid_tmp >> num_input_tmp;

            ret_val_from_fn = loaded_circuit.add_phantom_connection(gate_out_uid_tmp, take_inv_output_tmp, gate_in_uid_tmp, num_input_tmp);
        }

        //If the called function based on the line didn't return a 0 (success), return with error code
        if(ret_val_from_fn != 0)
            return 5;
    }

    in_file.close();
    loaded_circuit.m_next_gate_uid = highest_gate_uid + 1;

    *this = loaded_circuit;

    //Iterate over the phantom connection in the loaded circuit and based on that data, add connection to this circuit.
    //This is because connections also update pointers inside gates, and if we were just to copy one object into another, then problems
    //will arise when simulating the circuit.
    for(const auto& c : loaded_circuit.m_phantom_connections){
        if(add_connection(c.m_uid_output, c.m_inv_output, c.m_uid_input, c.m_num_input)){
            set_io(num_inputs(), num_outputs());
            return 6;
        }
    }

    set_inputs(vector<bool>(num_inputs(), false));
    m_outputs = vector<bool>(num_outputs(), false);

    return 0;
}