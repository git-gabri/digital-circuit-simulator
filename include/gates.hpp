#ifndef GATES_HPP
#define GATES_HPP

#include <vector>
#include <iostream>
#include <string>

//----------------------------------------------------------------------------------------------------------------------
//Basic struct of a logic gate
enum class status{to_update, updated};
enum class gate_type{buffer, not_gate, and_gate, or_gate, xor_gate, nand_gate, nor_gate, nxor_gate};

struct gate{
    gate_type type;
    size_t uid_gate;
    status gate_status;

    bool input_0;
    gate* ptr_gate_in0;
    bool take_inv_output_in_in0;
    
    bool input_1;
    gate* ptr_gate_in1;
    bool take_inv_output_in_in1;

    bool output;
    bool inv_output;

    gate(const gate_type& t = gate_type::buffer, const size_t& uid = 0) : 
        type(t),
        uid_gate(uid),
        gate_status(status::to_update),
        input_0(false),
        ptr_gate_in0(nullptr),
        take_inv_output_in_in0(false),
        input_1(false),
        ptr_gate_in1(nullptr),
        take_inv_output_in_in1(false),
        output(false),
        inv_output(true)
    {} 

    int calc_output(){
        if(gate_status == status::updated)
            return 0;

        if(type == gate_type::buffer || type == gate_type::not_gate){
            if(ptr_gate_in0 == nullptr && ptr_gate_in1 == nullptr)
                return 1;   
        }
        else if(ptr_gate_in0 == nullptr || ptr_gate_in1 == nullptr){
            return 1;
        }

        if(ptr_gate_in0 != nullptr)
            input_0 = (take_inv_output_in_in0 ? ptr_gate_in0->inv_output : ptr_gate_in0->output);
        if(ptr_gate_in1 != nullptr)
            input_1 = (take_inv_output_in_in1 ? ptr_gate_in1->inv_output : ptr_gate_in1->output);

        switch(type){
            case gate_type::buffer:
                output = (ptr_gate_in0 != nullptr && input_0) || (ptr_gate_in1 != nullptr && input_1);
                break;
            case gate_type::not_gate:
                output = !((ptr_gate_in0 != nullptr && input_0) || (ptr_gate_in1 != nullptr && input_1));
                break;
            case gate_type::and_gate:
                output = input_0 && input_1;
                break;
            case gate_type::or_gate:
                output = input_0 || input_1;
                break;
            case gate_type::xor_gate:
                output = input_0 ^ input_1;
                break;
             case gate_type::nand_gate:
                output = !(input_0 && input_1);
                break;
            case gate_type::nor_gate:
                output = !(input_0 || input_1);
                break;
            case gate_type::nxor_gate:
                output = !(input_0 ^ input_1);
                break;
        }
        
        inv_output = !output;
        gate_status = status::updated;

        return 0;
    }
};

#endif