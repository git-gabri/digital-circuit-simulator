/***********************************************************
Command line combinational circuit simulator written by
git-gabri.
***********************************************************/

#include <iostream>
#include <string>
#include <algorithm>

#include "circuit.hpp"
#include "console.hpp"

#define CONSOLE_CURSOR "\n> "
#define DEFAULT_NUM_INPUTS 4
#define DEFAULT_NUM_OUTPUTS 4

using namespace std;

int main() {
    //Declare the circuit and the console which will be interpreting the commands from the user
    circuit cir(DEFAULT_NUM_INPUTS, DEFAULT_NUM_OUTPUTS);
    console con(cir, cout);
    
    //Set user input processing variables
    string user_input = "";
    const vector<string> exit_commands = {"exit", "quit", "q"};
    bool should_exit = false;

    //Print header in the console
    cout << "Combinational logic circuit simulator by git-gabri" << endl;
    cout << endl;
    cout << "Circuit initialized with " << DEFAULT_NUM_INPUTS << " (+2) inputs and " << DEFAULT_NUM_OUTPUTS << " outputs" << endl;
    cout << "Type \"help\" or \"h\" for help" << endl;
    cout << "Type ";
    for(auto it_exit_cmds = exit_commands.begin(); it_exit_cmds != exit_commands.end(); ++it_exit_cmds){
        cout << "\"" << *it_exit_cmds << "\"";

        if(it_exit_cmds == exit_commands.end() - 2)
            cout << " or ";
        else if(it_exit_cmds != exit_commands.end() - 1)
            cout << ", ";
    }
    cout << " to exit" << endl;

    //Main loop that fetches commands
    do{
        cout << CONSOLE_CURSOR;
        getline(cin, user_input);

        should_exit = (find(exit_commands.begin(), exit_commands.end(), user_input) != exit_commands.end());

        if(!should_exit){
            con.execute_command(user_input);
        }
    }while(!should_exit);

    return 0;
}

/*
This program could have been written much better. I just wanted to finish it so that I could spend time on some
other projects :P
The quality of the code in the console.cpp file is questionable at best
*/