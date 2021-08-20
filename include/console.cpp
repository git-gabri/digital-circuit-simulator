#include <string>
#include <cctype>
#include <vector>
#include <iostream>
#include <algorithm>

#include "circuit.hpp"
#include "console.hpp"

#include "help.hpp"

using namespace std;

//----------------------------------------------------------------------------------------------------------------------
//Private utility methods

//Function to split a string in multiple substrings
vector<string> console::split_string_in_substrings(string input, const string& delimiters) {
    if(input.back() == '\n')
        input.pop_back();

    vector<string> substrings;

    auto it = input.begin();
    string substring;

    while(it != input.end()){
        if(delimiters.find(*it) == string::npos) {
            substring += *it;
        }
        else {
            substrings.push_back(substring);
            substring.clear();
        }

        ++it;
    }

    if(!substring.empty())
        substrings.push_back(substring);
    return substrings;
}

//Function to check if a string contains a valid uint
int console::validate_uint(const string& input_str, size_t& ouput_uint, const string& error_msg){
    int ret_val = 0;

    try{
        ouput_uint = stoull(input_str);
    }catch(...){
        m_os << error_msg << endl;
        ret_val = 1;
    }

    return ret_val;
}

//Function to check if a string contains a valid bool
int console::validate_bool(const string& input_str, bool& ouput_bool, const string& error_msg){
    int ret_val = 0;

    try{
        ouput_bool = stoi(input_str);
    }catch(...){
        m_os << error_msg << endl;
        ret_val = 1;
    }

    return ret_val;
}

//----------------------------------------------------------------------------------------------------------------------
//Private command execution methods

//Handle the printing of all the helps on the screen
void console::print_help(const vector<string>& command_and_args){
    if(command_and_args.size() == 1){
        m_os << general_help << endl;
    }
    else if(command_and_args.size() == 2){
        const string help_arg = command_and_args[1];

        if(help_arg == "nio")
            m_os << nio_help << endl;
        else if(help_arg == "al")
            m_os << al_help << endl;
        else if(help_arg == "ag")
            m_os << ag_help << endl;
        else if(help_arg == "ac")
            m_os << ac_help << endl;
        else if(help_arg == "dl")
            m_os << dl_help << endl;
        else if(help_arg == "dg")
            m_os << dg_help << endl;
        else if(help_arg == "dc")
            m_os << dc_help << endl;
        else if(help_arg == "dgci")
            m_os << dcgi_help << endl;
        else if(help_arg == "dgco")
            m_os << dcgo_help << endl;
        else if(help_arg == "dgca")
            m_os << dcga_help << endl;    
        else if(help_arg == "bal")
            m_os << bal_help << endl;
        else if(help_arg == "bag")
            m_os << bag_help << endl;
        else if(help_arg == "bac")
            m_os << bac_help << endl;
        else if(help_arg == "bdl")
            m_os << bdl_help << endl;
        else if(help_arg == "bdg")
            m_os << bdg_help << endl;
        else if(help_arg == "bdc")
            m_os << bdc_help << endl;
        else if(help_arg == "si")
            m_os << si_help << endl;
        else if(help_arg == "ro")
            m_os << ro_help << endl;
        else if(help_arg == "sc")
            m_os << sc_help << endl;
        else if(help_arg == "gtt")
            m_os << gtt_help << endl;
        else if(help_arg == "pc")
            m_os << pc_help << endl;
        else if(help_arg == "lu")
            m_os << lu_help << endl;
        else if(help_arg == "vc")
            m_os << vc_help << endl;
        else if(help_arg == "lc")
            m_os << lc_help << endl;
        else if(help_arg == "gate")
            m_os << gate_help << endl;
        else if(help_arg == "circuit")
            m_os << circuit_help << endl;
        else
            m_os << "ERR: no help on the specified argument" << endl;
    }
    else {
        m_os << "ERR: the command \"help\" has too many arguments" << endl;
    }
}

//Handle the setting of the number of inputs and outputs to the circuit
void console::set_num_io(const vector<string>& command_and_args){
    if(command_and_args.size() == 3){
        const string num_inputs_str = command_and_args[1];
        const string num_outputs_str = command_and_args[2];

        size_t num_inputs;
        if(validate_uint(num_inputs_str, num_inputs, "ERR: the specified number of inputs can't be converted to uint"))
            return;

        size_t num_outputs;
        if(validate_uint(num_outputs_str, num_outputs, "ERR: the specified number of outputs can't be converted to uint"))
            return;

        m_circuit.set_io(num_inputs, num_outputs);
        m_os << VALID_COMMAND_MSG << endl;
    }
    else {
        m_os << "ERR: the command \"nio\" requires 2 arguments" << endl;
    }
}

//Handle the addition of a layer
void console::add_layer(const vector<string>& command_and_args){
    if(command_and_args.size() == 2){
        const string num_layer_str = command_and_args[1];

        size_t num_layer;
        if(validate_uint(num_layer_str, num_layer, "ERR: the specified layer number can't be converted to uint"))
            return;

        if(m_circuit.add_layer(num_layer) == 0)
            m_os << VALID_COMMAND_MSG << endl;
        else
            m_os << "ERR: the specified layer already exists" << endl;
    }
    else{
        m_os << "ERR: the command \"al\" requires 1 argument" << endl;
    }
}

void console::add_gate(const vector<string>& command_and_args){
    if(command_and_args.size() == 3){
        //Convert the first argument to a gate_type
        string gate_type_str = command_and_args[1];
        transform(gate_type_str.begin(), gate_type_str.end(), gate_type_str.begin(), [](const unsigned char c){return tolower(c);});

        gate_type gt;
        if(gate_type_str == "buf")
            gt = gate_type::buffer;
        else if(gate_type_str == "not")
            gt = gate_type::not_gate;
        else if(gate_type_str == "and")
            gt = gate_type::and_gate;
        else if(gate_type_str == "or")
            gt = gate_type::or_gate;
        else if(gate_type_str == "xor" || gate_type_str == "exor")
            gt = gate_type::xor_gate;
        else if(gate_type_str == "nand")
            gt = gate_type::nand_gate;
        else if(gate_type_str == "nor")
            gt = gate_type::nor_gate;
        else if(gate_type_str == "nxor" || gate_type_str == "nexor")
            gt = gate_type::nxor_gate;
        else{
            m_os << "ERR: unrecognised gate type" << endl;
            return;
        }

        //Convert the second argument to the layer number
        const string num_layer_str = command_and_args[2];
        
        size_t num_layer;
        if(validate_uint(num_layer_str, num_layer, "ERR: the specified layer number can't be converted to uint"))
            return;

        //Effectively add the gate to the circuit
        if(m_circuit.add_gate(gt, num_layer) == 0)
            m_os << VALID_COMMAND_MSG << endl;
        else
            m_os << "ERR: can't add the gate to the specified layer" << endl;
    }
    else{
        m_os << "ERR: the command \"ag\" requires 2 arguments" << endl;
    }
}

//Handle addition of a connection
void console::add_connection(const vector<string>& command_and_args){
    string num_layer_out_str;
    string gate_out_uid_str;
    string take_inv_output_str;
    string num_layer_in_str;
    string gate_in_uid_str;
    string num_input_str;
    int ret_val_from_fn = -10;

    switch(command_and_args.size()){
        //3 arguments: gate_out_uid, gate_in_uid, num_input
        case 4:
            {
                gate_out_uid_str = command_and_args[1];
                gate_in_uid_str = command_and_args[2];
                num_input_str = command_and_args[3];
                
                //Convert arguments to proper data
                size_t gate_out_uid;
                if(validate_uint(gate_out_uid_str, gate_out_uid, "ERR: the specified gate_out_uid can't be converted to uint"))
                    return;

                size_t gate_in_uid;
                if(validate_uint(gate_in_uid_str, gate_in_uid, "ERR: the specified gate_in_uid can't be converted to uint"))
                    return;

                bool num_input;
                if(validate_bool(num_input_str, num_input, "ERR: the specified num_input can't be converted to int and then to bool"))
                    return;

                ret_val_from_fn = m_circuit.add_connection(gate_out_uid, gate_in_uid, num_input);
            }
            break;

        //4 arguments: gate_out_uid, take_inv_output, gate_in_uid, num_input
        case 5:
            {
                gate_out_uid_str = command_and_args[1];
                take_inv_output_str = command_and_args[2];
                gate_in_uid_str = command_and_args[3];
                num_input_str = command_and_args[4];
                
                //Convert arguments to proper data
                size_t gate_out_uid;
                if(validate_uint(gate_out_uid_str, gate_out_uid, "ERR: the specified gate_out_uid can't be converted to uint"))
                    return;

                bool take_inv_output;
                if(validate_bool(take_inv_output_str, take_inv_output, "ERR: the specified take_inv_output can't be converted to int and then to bool"))
                    return;

                size_t gate_in_uid;
                if(validate_uint(gate_in_uid_str, gate_in_uid, "ERR: the specified gate_in_uid can't be converted to uint"))
                    return;

                bool num_input;
                if(validate_bool(num_input_str, num_input, "ERR: the specified num_input can't be converted to int and then to bool"))
                    return;

                ret_val_from_fn = m_circuit.add_connection(gate_out_uid, take_inv_output, gate_in_uid, num_input);
            }
            break;

        //5 arguments: num_layer_out, gate_out_uid, num_layer_in, gate_in_uid, num_input
        case 6:
            {
                num_layer_out_str = command_and_args[1];
                gate_out_uid_str = command_and_args[2];
                num_layer_in_str = command_and_args[3];
                gate_in_uid_str = command_and_args[4];
                num_input_str = command_and_args[5];
                
                //Convert arguments to proper data
                size_t num_layer_out;
                if(validate_uint(num_layer_out_str, num_layer_out, "ERR: the specified num_layer_out can't be converted to uint"))
                    return;

                size_t gate_out_uid;
                if(validate_uint(gate_out_uid_str, gate_out_uid, "ERR: the specified gate_out_uid can't be converted to uint"))
                    return;

                size_t num_layer_in;
                if(validate_uint(num_layer_in_str, num_layer_in, "ERR: the specified num_layer_in can't be converted to uint"))
                    return;

                size_t gate_in_uid;
                if(validate_uint(gate_in_uid_str, gate_in_uid, "ERR: the specified gate_in_uid can't be converted to uint"))
                    return;

                bool num_input;
                if(validate_bool(num_input_str, num_input, "ERR: the specified num_input can't be converted to int and then to bool"))
                    return;

                ret_val_from_fn = m_circuit.add_connection(num_layer_out, gate_out_uid, num_layer_in, gate_in_uid, num_input);
            }
            break;

        //6 arguments: num_layer_out, gate_out_uid, take_inv_output, num_layer_in, gate_in_uid, num_input
        case 7:
            {
                num_layer_out_str = command_and_args[1];
                gate_out_uid_str = command_and_args[2];
                take_inv_output_str = command_and_args[3];
                num_layer_in_str = command_and_args[4];
                gate_in_uid_str = command_and_args[5];
                num_input_str = command_and_args[6];
                
                //Convert arguments to proper data
                size_t num_layer_out;
                if(validate_uint(num_layer_out_str, num_layer_out, "ERR: the specified num_layer_out can't be converted to uint"))
                    return;

                size_t gate_out_uid;
                if(validate_uint(gate_out_uid_str, gate_out_uid, "ERR: the specified gate_out_uid can't be converted to uint"))
                    return;

                bool take_inv_output;
                if(validate_bool(take_inv_output_str, take_inv_output, "ERR: the specified take_inv_output can't be converted to int and then to bool"))
                    return;

                size_t num_layer_in;
                if(validate_uint(num_layer_in_str, num_layer_in, "ERR: the specified num_layer_in can't be converted to uint"))
                    return;

                size_t gate_in_uid;
                if(validate_uint(gate_in_uid_str, gate_in_uid, "ERR: the specified gate_in_uid can't be converted to uint"))
                    return;

                bool num_input;
                if(validate_bool(num_input_str, num_input, "ERR: the specified num_input can't be converted to int and then to bool"))
                    return;

                ret_val_from_fn = m_circuit.add_connection(num_layer_out, gate_out_uid, num_layer_in, gate_in_uid, num_input);
            }
            break;

        default:
            m_os << "ERR: the command \"ac\" requires 3, 4, 5 or 6 arguments" << endl;
            return;
            break;
    }

    //Call function to add connection in circuit and display message based on the return value
    switch(ret_val_from_fn){
        case 0:
            m_os << VALID_COMMAND_MSG << endl;
            break;

        case -1:
            m_os << "ERR: one or both of the specified gates aren't present in the circuit" << endl;
            break;

        case 1:
            m_os << "ERR: one or both of the specified layers aren't present in the circuit" << endl;
            break;

        case 2:
            m_os << "ERR: one or both of the specified gates aren't in their respective specified layers" << endl;
            break;

        case 3:
            m_os << "ERR: only forward connections are allowed" << endl;
            break;

        default:
            m_os << GENERIC_INVALID_COMMAND_MSG << endl;
            break;
    }
}

//Handle deletion of a layer
void console::delete_layer(const vector<string>& command_and_args){
    if(command_and_args.size() == 2){
        const string num_layer_str = command_and_args[1];

        size_t num_layer;
        if(validate_uint(num_layer_str, num_layer, "ERR: the specified layer number can't be converted to uint"))
            return;

        switch(m_circuit.delete_layer(num_layer)){
            case 0:
                m_os << VALID_COMMAND_MSG << endl;
                break;

            case 1:
                m_os << "ERR: the specified layer isn't contained in the circuit" << endl;
                break;

            case 2:
                m_os << "ERR: input and output layer can't be deleted" << endl;
                break;

            case 3:
                m_os << "ERR: couldn't empty the specified layer from all the gates it contained" << endl;
                break;

            default:
                m_os << GENERIC_INVALID_COMMAND_MSG << endl;
                break;
        }
    }
    else{
        m_os << "ERR: the command \"dl\" requires 1 argument" << endl;
    }
}

//Handle gate deletion
void console::delete_gate(const vector<string>& command_and_args){
    string gate_uid_str;
    string num_layer_str;
    int ret_val_from_fn = -10;

    switch(command_and_args.size()){
        case 2:
            {
                gate_uid_str = command_and_args[1];
                
                size_t gate_uid;
                if(validate_uint(gate_uid_str, gate_uid, "ERR: the specified gate uid can't be converted to uint"))
                    return;

                ret_val_from_fn = m_circuit.delete_gate(gate_uid);
            }   
            break;

        case 3:
            {
                gate_uid_str = command_and_args[1];
                num_layer_str = command_and_args[2];

                size_t gate_uid;
                if(validate_uint(gate_uid_str, gate_uid, "ERR: the specified gate uid can't be converted to uint"))
                    return;

                size_t num_layer;
                if(validate_uint(num_layer_str, num_layer, "ERR: the specified layer number can't be converted to uint"))
                    return;

                ret_val_from_fn = m_circuit.delete_gate(gate_uid, num_layer);
            }   
            break;

         default:
            m_os << "ERR: the command \"dg\" requires 1 or 2 arguments" << endl;
            return;
            break;
    }

    switch(ret_val_from_fn){
        case 0:
            m_os << VALID_COMMAND_MSG << endl;
            break;

        case 1:
            m_os << "ERR: the gate with the specified uid is not in the circuit" << endl;
            break;

        case 2:
            m_os << "ERR: the specified gate isn't in the specified layer" << endl;
            break;

        case 3:
            m_os << "ERR: can't delete gates from the input or the output layers" << endl;
            break;

        default:
            m_os << GENERIC_INVALID_COMMAND_MSG << endl;
            break;
    }
 
}

//Handle connection deletion
void console::delete_connection(const vector<string>& command_and_args){
    string num_layer_out_str;
    string gate_out_uid_str;
    string take_inv_output_str;
    string num_layer_in_str;
    string gate_in_uid_str;
    string num_input_str;
    int ret_val_from_fn = -10;
 
    switch(command_and_args.size()){
        //2 arguments: <gate_in_uid> <num_input 0/1>
        case 3:
            {
                gate_in_uid_str = command_and_args[1];
                num_input_str = command_and_args[2];
                
                size_t gate_in_uid;
                if(validate_uint(gate_in_uid_str, gate_in_uid, "ERR: the specified gate_in_uid can't be converted to uint"))
                    return;

                bool num_input;
                if(validate_bool(num_input_str, num_input, "ERR: the specified num_input can't be converted to int and then to bool"))
                    return;

                ret_val_from_fn = m_circuit.delete_connection(gate_in_uid, num_input);
            }
            break;

        //3 arguments: <gate_out_uid> <gate_in_uid> <num_input 0/1>
        case 4:
            {
                gate_out_uid_str = command_and_args[1];
                gate_in_uid_str = command_and_args[2];
                num_input_str = command_and_args[3];
                
                size_t gate_out_uid;
                if(validate_uint(gate_out_uid_str, gate_out_uid, "ERR: the specified gate_out_uid can't be converted to uint"))
                    return;

                size_t gate_in_uid;
                if(validate_uint(gate_in_uid_str, gate_in_uid, "ERR: the specified gate_in_uid can't be converted to uint"))
                    return;

                bool num_input;
                if(validate_bool(num_input_str, num_input, "ERR: the specified num_input can't be converted to int and then to bool"))
                    return;

                ret_val_from_fn = m_circuit.delete_connection(gate_out_uid, gate_in_uid, num_input);
            }
            break;

        //4 arguments: <gate_out_uid> <take_inv_output 0/1> <gate_in_uid> <num_input 0/1>
        case 5:
            {
                gate_out_uid_str = command_and_args[1];
                take_inv_output_str = command_and_args[2];
                gate_in_uid_str = command_and_args[3];
                num_input_str = command_and_args[4];
                
                size_t gate_out_uid;
                if(validate_uint(gate_out_uid_str, gate_out_uid, "ERR: the specified gate_out_uid can't be converted to uint"))
                    return;

                bool take_inv_output;
                if(validate_bool(take_inv_output_str, take_inv_output, "ERR: the specified take_inv_output can't be converted to int and then to bool"))
                    return;

                size_t gate_in_uid;
                if(validate_uint(gate_in_uid_str, gate_in_uid, "ERR: the specified gate_in_uid can't be converted to uint"))
                    return;

                bool num_input;
                if(validate_bool(num_input_str, num_input, "ERR: the specified num_input can't be converted to int and then to bool"))
                    return;

                ret_val_from_fn = m_circuit.delete_connection(gate_out_uid, take_inv_output, gate_in_uid, num_input);
            }
            break;

        //6 arguments: num_layer_out, gate_out_uid, take_inv_output, num_layer_in, gate_in_uid, num_input
        case 7:
            {
                num_layer_out_str = command_and_args[1];
                gate_out_uid_str = command_and_args[2];
                take_inv_output_str = command_and_args[3];
                num_layer_in_str = command_and_args[4];
                gate_in_uid_str = command_and_args[5];
                num_input_str = command_and_args[6];
                
                size_t num_layer_out;
                if(validate_uint(num_layer_out_str, num_layer_out, "ERR: the specified num_layer_out can't be converted to uint"))
                    return;

                size_t gate_out_uid;
                if(validate_uint(gate_out_uid_str, gate_out_uid, "ERR: the specified gate_out_uid can't be converted to uint"))
                    return;

                bool take_inv_output;
                if(validate_bool(take_inv_output_str, take_inv_output, "ERR: the specified take_inv_output can't be converted to int and then to bool"))
                    return;

                size_t num_layer_in;
                if(validate_uint(num_layer_in_str, num_layer_in, "ERR: the specified num_layer_in can't be converted to uint"))
                    return;

                size_t gate_in_uid;
                if(validate_uint(gate_in_uid_str, gate_in_uid, "ERR: the specified gate_in_uid can't be converted to uint"))
                    return;

                bool num_input;
                if(validate_bool(num_input_str, num_input, "ERR: the specified num_input can't be converted to int and then to bool"))
                    return;

                ret_val_from_fn = m_circuit.delete_connection(num_layer_out, gate_out_uid, take_inv_output, num_layer_in, gate_in_uid, num_input);
            }
            break;

        default:
            m_os << "ERR: the command \"dc\" requires 2, 3, 4 or 6 arguments" << endl;
            return;
            break;
    }

    switch(ret_val_from_fn){
        case 0:
            m_os << VALID_COMMAND_MSG << endl;
            break;

        case 1:
            m_os << "ERR: one or both of the specified layers aren't present in the circuit" << endl;
            break;

        case 2:
            m_os << "ERR: one or both of the specified gates aren't in their respective specified layers" << endl;
            break;

        default:
            m_os << GENERIC_INVALID_COMMAND_MSG << endl;
            break;
    }
}

//Handle more connection deletions
void console::delete_connections_to_gate_inputs(const vector<string>& command_and_args){
    string gate_uid_str;
    string num_layer_str;
    int ret_val_from_fn = -10;
 
    switch(command_and_args.size()){
        //1 argument: <gate_uid>
        case 2:
            {
                gate_uid_str = command_and_args[1];

                size_t gate_uid;
                if(validate_uint(gate_uid_str, gate_uid, "ERR: the specified gate uid can't be converted to uint"))
                    return;

                ret_val_from_fn = m_circuit.delete_connections_to_gate_inputs(gate_uid);
            }
            break;

        //2 arguments: <gate_uid> <num_layer>
        case 3:
            {
                gate_uid_str = command_and_args[1];
                num_layer_str = command_and_args[2];

                size_t gate_uid;
                if(validate_uint(gate_uid_str, gate_uid, "ERR: the specified gate uid can't be converted to uint"))
                    return;

                size_t num_layer;
                if(validate_uint(num_layer_str, num_layer, "ERR: the specified layer number can't be converted to uint"))
                    return;

                ret_val_from_fn = m_circuit.delete_connections_to_gate_inputs(gate_uid, num_layer);
            }
            break;

        default:
            m_os << "ERR: the command \"dcgi\" requires 1 or 2 arguments" << endl;
            return;
            break;
    } 

    switch(ret_val_from_fn){
        case 0:
            m_os << VALID_COMMAND_MSG << endl;
            break;

        case 1:
            m_os << "ERR: the specified gate is not in the circuit" << endl;
            break;

        case 2:
            m_os << "ERR: the specified gate is not in the specified layer" << endl;
            break;

        default:
            m_os << GENERIC_INVALID_COMMAND_MSG << endl;
            break;
    }
}

//Handle more connection deletions
void console::delete_connections_from_gate_outputs(const vector<string>& command_and_args){
    string gate_uid_str;
    string num_layer_str;
    int ret_val_from_fn = -10;
 
    switch(command_and_args.size()){
        //1 argument: <gate_uid>
        case 2:
            {
                gate_uid_str = command_and_args[1];

                size_t gate_uid;
                if(validate_uint(gate_uid_str, gate_uid, "ERR: the specified gate uid can't be converted to uint"))
                    return;

                ret_val_from_fn = m_circuit.delete_connections_from_gate_outputs(gate_uid);
            }
            break;

        //2 arguments: <gate_uid> <num_layer>
        case 3:
            {
                gate_uid_str = command_and_args[1];
                num_layer_str = command_and_args[2];

                size_t gate_uid;
                if(validate_uint(gate_uid_str, gate_uid, "ERR: the specified gate uid can't be converted to uint"))
                    return;

                size_t num_layer;
                if(validate_uint(num_layer_str, num_layer, "ERR: the specified layer number can't be converted to uint"))
                    return;

                ret_val_from_fn = m_circuit.delete_connections_from_gate_outputs(gate_uid, num_layer);
            }
            break;

        default:
            m_os << "ERR: the command \"dcgo\" requires 1 or 2 arguments" << endl;
            return;
            break;
    } 

    switch(ret_val_from_fn){
        case 0:
            m_os << VALID_COMMAND_MSG << endl;
            break;

        case 1:
            m_os << "ERR: the specified gate is not in the circuit" << endl;
            break;

        case 2:
            m_os << "ERR: the specified gate is not in the specified layer" << endl;
            break;

        default:
            m_os << GENERIC_INVALID_COMMAND_MSG << endl;
            break;
    }
}

//Handle more connection deletions
void console::delete_all_connections_of_gate(const vector<string>& command_and_args){
    m_os << "Inputs  : ";
    delete_connections_to_gate_inputs(command_and_args);
    m_os << "Outputs : ";
    delete_connections_from_gate_outputs(command_and_args);
}

//Handle bulk layer addition
void console::bulk_add_layer(const vector<string>& command_and_args){
    if(command_and_args.size() > 1){
        for(auto it_caa = command_and_args.begin() + 1; it_caa < command_and_args.end(); ++it_caa){
            m_os << "Adding layer " << *it_caa << " : ";
            add_layer(vector<string>{"al", *it_caa});
        }
    }
    else{
        m_os << "ERR: the command \"bal\" requires at least 1 argument" << endl;
    }
}

//Handle bulk gate addition
void console::bulk_add_gate(const vector<string>& command_and_args){
    if(command_and_args.size() >= 3){
        if((command_and_args.size() - 1) % 2 != 0){
            m_os << "ERR: the list of arguments is of the wrong length to repeatedly call \"ag\"" << endl;
            return;
        }

        size_t counter = 0;
        for(auto it_caa = command_and_args.begin() + 1; it_caa < command_and_args.end(); it_caa += 2){
            m_os << "Adding gate " << counter << " in list: ";
            add_gate(vector<string>{"ag", *it_caa, *(it_caa + 1)});

            ++counter;
        }
    }
    else{
        m_os << "ERR: the command \"bag\" requires at least 2 arguments" << endl;
    }
}

//Handle bulk connection addition
void console::bulk_add_connection(const vector<string>& command_and_args){
    if(command_and_args.size() >= 5){
        if((command_and_args.size() - 1) % 4 != 0){
            m_os << "ERR: the list of arguments is of the wrong length to repeatedly call \"ac\" with syntax 2" << endl;
            return;
        }

        size_t counter = 0;
        for(auto it_caa = command_and_args.begin() + 1; it_caa < command_and_args.end(); it_caa += 4){
            m_os << "Adding connection " << counter << " in list: ";
            add_connection(vector<string>{"ac", *it_caa, *(it_caa + 1), *(it_caa + 2), *(it_caa + 3)});

            ++counter;
        }
    }
    else{
        m_os << "ERR: the command \"bac\" requires at least 4 arguments" << endl;
    }
}

//Handle bulk layer deletion
void console::bulk_delete_layer(const vector<string>& command_and_args){
    if(command_and_args.size() > 1){
        for(auto it_caa = command_and_args.begin() + 1; it_caa < command_and_args.end(); ++it_caa){
            m_os << "Deleting layer " << *it_caa << " : ";
            delete_layer(vector<string>{"dl", *it_caa});
        }
    }
    else{
        m_os << "ERR: the command \"bdl\" requires at least 1 argument" << endl;
    }
}

//Handle bulk gate deletion
void console::bulk_delete_gate(const vector<string>& command_and_args){
    if(command_and_args.size() > 1){
        for(auto it_caa = command_and_args.begin() + 1; it_caa < command_and_args.end(); ++it_caa){
            m_os << "Deleting gate " << *it_caa << " : ";
            delete_gate(vector<string>{"dg", *it_caa});
        }
    }
    else{
        m_os << "ERR: the command \"bdg\" requires at least 1 argument" << endl;
    }
}

//Handle bulk connection deletion
void console::bulk_delete_connection(const vector<string>& command_and_args){
    if(command_and_args.size() > 1){
        if((command_and_args.size() - 1) % 2 != 0){
            m_os << "ERR: the list of arguments is of the wrong length to repeatedly call \"dc\" with syntax 1" << endl;
            return;
        }

        size_t counter = 0;
        for(auto it_caa = command_and_args.begin() + 1; it_caa < command_and_args.end(); it_caa += 2){
            m_os << "Deleting connection " << counter << " in list: ";
            add_connection(vector<string>{"dc", *it_caa, *(it_caa + 1)});

            ++counter;
        }
    }
    else{
        m_os << "ERR: the command \"bdc\" requires at least 2 arguments" << endl;
    }
}

//Handle input setting
void console::set_inputs(const std::vector<std::string>& command_and_args){
    if(command_and_args.size() == 2){
        const string inputs_str = command_and_args[1];
        
        vector<bool> inputs;
        for(const auto& c : inputs_str){
            if(c == '0')
                inputs.push_back(0);
            else if(c == '1')
                inputs.push_back(1);
            else{
                m_os << "ERR: invalid character found in argument of command" << endl;
                return;
            }
        }

        if(m_circuit.set_inputs(inputs) == 0)
            m_os << VALID_COMMAND_MSG << endl;
        else
            m_os << "ERR: the number of specified bits as inputs isn't equal to the number of inputs of the circuit" << endl;
    }
    else{
        m_os << "ERR: the command \"si\" requires 1 argument" << endl;
    }
}

//Handle output reading
void console::read_outputs(const std::vector<std::string>& command_and_args){
    if(command_and_args.size() == 1){
        const vector<bool> circuit_outputs = m_circuit.read_outputs();

        for(auto out_it = circuit_outputs.begin(); out_it < circuit_outputs.end(); ++out_it)
            m_os << (int)(*out_it);
        m_os << endl;

        m_os << VALID_COMMAND_MSG << endl;
    }
    else{
        m_os << "ERR: the command \"ro\" requires no arguments" << endl;
    }
}

//Handle circuit simulation
void console::simulate_circuit(const std::vector<std::string>& command_and_args){
    string inputs_str;
    int ret_val_from_fn = -10;

    switch(command_and_args.size()){
        case 1:
            ret_val_from_fn = m_circuit.simulate_circuit();
            break;

        case 2:
            inputs_str = command_and_args[1];

            m_os << "Set inputs: ";
            set_inputs(vector<string>{"si", inputs_str});
            m_os << "Simulation: ";
            ret_val_from_fn = m_circuit.simulate_circuit();
            break;

        default:
            m_os << "ERR: the command \"sc\" requires 0 or 1 argument" << endl;
            break;
    }

    if(ret_val_from_fn == 0)
        m_os << VALID_COMMAND_MSG << endl;
    else
        m_os << "ERR: some gates in the circuit have their inputs not connected" << endl;
}

//Handle truth table generation
void console::gen_truth_table(const std::vector<std::string>& command_and_args){
    if(command_and_args.size() == 1){
        if(m_circuit.gen_truth_table(m_os))
            m_os << "ERR: some gates in the circuit have their inputs not connected" << endl;
        else
            m_os << VALID_COMMAND_MSG << endl;
    }
    else{
        m_os << "ERR: the command \"gtt\" requires no arguments" << endl;
    }
}

//Handle circuit printing to screen
void console::print_circuit(const std::vector<std::string>& command_and_args){
    string print_gates_str;
    string print_connections_str;

    switch(command_and_args.size()){
        case 1:
            m_circuit.print_circuit(true, true, m_os);
            break;

        case 2:
            {
                print_gates_str = command_and_args[1];

                bool print_gates;
                if(validate_bool(print_gates_str, print_gates, "ERR: the flag for printing gates can't be converted to int and then to bool"))
                    return;

                m_circuit.print_circuit(print_gates, true, m_os);
            }
            break;

        case 3:
            {
                print_gates_str = command_and_args[1];
                print_connections_str = command_and_args[2];

                bool print_gates;
                if(validate_bool(print_gates_str, print_gates, "ERR: the flag for printing gates can't be converted to int and then to bool"))
                    return;

                bool print_connections;
                if(validate_bool(print_connections_str, print_connections, "ERR: the flag for printing connections can't be converted to int and then to bool"))
                    return;

                m_circuit.print_circuit(print_gates, print_connections, m_os);
            }
            break;

        default:
            m_os << "ERR: the command \"pc\" requires 0, 1 or 2 arguments" << endl;
            return;
            break;
    }

    m_os << VALID_COMMAND_MSG << endl;
}

//Handle unconnected gates listing
void console::list_unconnected(const std::vector<std::string>& command_and_args){
    if(command_and_args.size() == 1){
        m_circuit.list_unconnected(m_os);
        m_os << VALID_COMMAND_MSG << endl;
    }
    else{
        m_os << "ERR: the command \"lu\" requires no arguments" << endl;
    }
}

//Handle circuit saving to file
void console::save_circuit(const std::vector<std::string>& command_and_args){
    if(command_and_args.size() == 2){
        const string filename = command_and_args[1];

        if(m_circuit.save_circuit_to_file(filename) == 0)
            m_os << VALID_COMMAND_MSG << endl;
        else
            m_os << "ERR: output file can't be opened" << endl;
    }
    else{
        m_os << "ERR: the command \"vc\" requires 1 argument" << endl;
    }
}

//Handle circuit loading from file
void console::load_circuit(const std::vector<std::string>& command_and_args){
    if(command_and_args.size() == 2){
        const string filename = command_and_args[1];

        switch(m_circuit.load_circuit_from_file(filename)){
            case 0:
                m_os << VALID_COMMAND_MSG << endl;
                break;

            case 1:
                m_os << "ERR: input file can't be opened" << endl;
                break;

            case 2:
                m_os << "ERR: can't read number of inputs from file" << endl;
                break;

            case 20:
                m_os << "ERR: badly formatted number of inputs" << endl;
                break;

            case 3:
                m_os << "ERR: can't read number of outputs from filr" << endl;
                break;

            case 30:
                m_os << "ERR: badly formatted number of outputs" << endl;
                break;

            case 4:
                m_os << "ERR: file contains a non recognized line type" << endl;
                break;

            case 40:
                m_os << "ERR: line types are out of order, they must go Ls, Gs and then Cs" << endl;
                break;

            case 5:
                m_os << "ERR: the information specified in one of the lines of the file is invalid" << endl;
                break;

            default:
                m_os << GENERIC_INVALID_COMMAND_MSG << endl;
                break;
        }
    }
    else{
        m_os << "ERR: the command \"lc\" requires 1 argument" << endl;
    }
}

//----------------------------------------------------------------------------------------------------------------------
//Public methods

//Function to execute a command and make changes according to that command, in the circuit
void console::execute_command(const string& user_input){
    vector<string> command_and_args = split_string_in_substrings(user_input, " ");

    if(command_and_args.empty())
        command_and_args.emplace_back();

    const string command_str = command_and_args[0];
    
    //Because switch statements for strings aren't a thing :(
    if(command_str == "help" || command_str == "h")
        print_help(command_and_args);
    else if(command_str == "nio")
        set_num_io(command_and_args);
    else if(command_str == "al")
        add_layer(command_and_args);
    else if(command_str == "ag")
        add_gate(command_and_args);
    else if(command_str == "ac")
        add_connection(command_and_args);
    else if(command_str == "dl")
        delete_layer(command_and_args);
    else if(command_str == "dg")
        delete_gate(command_and_args);
    else if(command_str == "dc")
        delete_connection(command_and_args);
    else if(command_str == "dcgi")
        delete_connections_to_gate_inputs(command_and_args);
    else if(command_str == "dcgo")
        delete_connections_from_gate_outputs(command_and_args);
    else if(command_str == "dcga")
        delete_all_connections_of_gate(command_and_args);
    else if(command_str == "bal")
        bulk_add_layer(command_and_args);
    else if(command_str == "bag")
        bulk_add_gate(command_and_args);
    else if(command_str == "bac")
        bulk_add_connection(command_and_args);
    else if(command_str == "bdl")
        bulk_delete_layer(command_and_args);
    else if(command_str == "bdg")
        bulk_delete_gate(command_and_args);
    else if(command_str == "bdc")
        bulk_delete_connection(command_and_args);
    else if(command_str == "si")
        set_inputs(command_and_args);
    else if(command_str == "ro")
        read_outputs(command_and_args);
    else if(command_str == "sc")
        simulate_circuit(command_and_args);
    else if(command_str == "gtt")
        gen_truth_table(command_and_args);
    else if(command_str == "pc")
        print_circuit(command_and_args);
    else if(command_str == "lu")
        list_unconnected(command_and_args);
    else if(command_str == "vc")
        save_circuit(command_and_args);
    else if(command_str == "lc")
        load_circuit(command_and_args);
    else 
        m_os << "ERR: the command \"" << command_str << "\" has not been recognized" << endl; 

    return;
}