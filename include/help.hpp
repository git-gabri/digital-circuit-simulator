#ifndef HELP_HPP
#define HELP_HPP

#include <string>

const std::string general_help =
R"foobar(Combinational logic circuit simulator by git-gabri

Type "help" or "h" for this help.
Type "help/h <command>" to get help on a specific command.
Type "help/h <argument>" to get help on a specific argument.

The commands recognized by this simulator are the following:
- help/h    -> print this help
- nio   -> set the number of inputs and outputs of the circuit
           (Note: this completely resets the contents of the circuit)
- al    -> add layer to the circuit
- ag    -> add gate to the circuit
- ac    -> add connection to the circuit
- dl    -> delete layer
- dg    -> delete gate
- dc    -> delete connection
- dcgi  -> delete all connections to a gate's inputs
- dcgo  -> delete all connections from a gate's outputs
- dcga  -> delete all connections to and from a gate
- bal   -> bulk add layers, add layers in bulk
- bag   -> bulk add gates, add gates in bulk
- bac   -> bulk add connections, add connections in bulk
- bdl   -> bulk delete layers, delete layers in bulk
- bdg   -> bulk delete gates, delete gates in bulk
- bdc   -> bulk delete connections
- si    -> set circuit inputs
- ro    -> read circuit outputs
- sc    -> simulate circuit
- gtt   -> generate the truth table
- pc    -> print circuit
- lu    -> list unconnected gates
- vc    -> saves the circuit to file
- lc    -> load the circuit from file

The arguments onto which some help is written are the following:
- gate  -> description on how gates are costructed internally
- circuit -> description on how the circuit is structured in memory

If this is the first time using this program, "help gate" and "help circuit"
are highly recommended reads.)foobar";

const std::string nio_help =
R"foobar("nio" command.
This command sets the number of inputs and outputs of the circuit.

Syntax: "nio <uint> <uint>"
The first unsigned integer is the number of inputs.
The second unsigned integer is the number of outputs.

NOTE: doing this causes all the contents of the circuit (gates, layers)
to be deleted!
Usage of this command is suggested at the start of the project.)foobar";

const std::string al_help =
R"foobar("al" command.
This command adds a layer of gates to the circuit.

Syntax: "al <uint>"
The unsigned integer is the number of the layer.

NOTE: the numbers 0 and -1 are reserved for the input and output layers
respectively.)foobar";

const std::string ag_help =
R"foobar("ag" command.
This command adds a gate to a layer in the circuit.

Syntax: "ag <gate_type> <uint>"
The recognised gate types (case insensitive) are:
"buf", "not", "and", "or", "xor/exor", "nand", "nor", "nxor/nexor".
The unsigned integer is the number of the layer into which the gate will
be added.

NOTE: gates cannot be added to the input or the output layers.)foobar";

const std::string ac_help =
R"foobar("ac" command.
This command adds a connection from a gate's output to a gate's input.

Syntaxes:
1) "ac <gate_out_uid> <gate_in_uid> <num_input 0/1>"
2) "ac <gate_out_uid> <take_inv_output 0/1> <gate_in_uid> <num_input 0/1>"
3) "ac <num_layer_out> <gate_out_uid> <num_layer_in> <gate_in_uid> <num_input 0/1>"
4) "ac <num_layer_out> <gate_out_uid> <take_inv_output 0/1> <num_layer_in> <gate_in_uid> <num_input 0/1>"

The gate uids are unsigned integers that can be seen by printing the circuit to the screen.
The layer numbers are where the gates with the specified uids should be.
"take_inv_output" is a boolean that specifies to which output of the gate (normal or inverted) the
connection should start.
"num_input" is a boolean that specifies to which input of the gate the connection should end.

Syntax 1 is the most common, with syntax 2 the user already has full control on the connection.
Syntaxes 3 and 4 have just some extra checks.

NOTE: a connection must go forward in the number of layers, which means that the gate from which the
signal is taken must be in a layer whose number is less than the one of the layer containing the gate
at which the connection ends.
Example: layer 1 contains gate 5, layer 2 contains gate 4. A connection from 5 to 4 is allowed but
not viceversa.)foobar";

const std::string dl_help =
R"foobar("dl" command.
This command deletes a layer of gates from the circuit.
First the connections to the gates contained in the layer are deleted, then the gates are deleted,
then the layer is deleted.

Syntax: "dl <uint>"
The unsigned integer is the number of the layer.

NOTE: the input and the output layers cannot be deleted.)foobar";

const std::string dg_help =
R"foobar("dg" command.
This command deletes a gate from the circuit.
First the connections to the gate are deleted, then the gate itself is deleted

Syntaxes:
1) "dg <gate_uid>"
2) "dg <gate_uid> <num_layer>"
All the parameters are uint.
In syntax 2, the num_layer parameter is the number of the layer in which the specified gate
should be located.

NOTE: gates cannot be deleted from the input or the output layers.)foobar";

const std::string dc_help =
R"foobar("dc" command.
This command deletes a connection from a gate's output to a gate's input.

Syntaxes:
1) "dc <gate_in_uid> <num_input 0/1>"
2) "dc <gate_out_uid> <gate_in_uid> <num_input 0/1>"
3) "dc <gate_out_uid> <take_inv_output 0/1> <gate_in_uid> <num_input 0/1>"
4) "dc <num_layer_out> <gate_out_uid> <take_inv_output 0/1> <num_layer_in> <gate_in_uid> <num_input 0/1>"

The gate uids are unsigned integers that can be seen by printing the circuit to the screen.
The layer numbers are where the gates with the specified uids should be.
"take_inv_output" is a boolean that specifies to which output of the gate (normal or inverted) the
connection should start.
"num_input" is a boolean that specifies to which input of the gate the connection should end.

Syntaxes 1 and 2 are the most common, with syntax 1 the user already has full control on the connection.
Syntaxes 3 and 4 have just some extra checks.)foobar";

const std::string dcgi_help =
R"foobar("dcgi" command.
This command deletes all the connections to a gate's inputs. 

Syntaxes:
1) "dcgi <gate_uid>"
2) "dcgi <gate_uid> <num_layer>"

The gate uid and the layer number are uints.)foobar";

const std::string dcgo_help =
R"foobar("dcgo" command.
This command deletes all the connections from a gate's outputs.

Syntaxes:
1) "dcgo <gate_uid>"
2) "dcgo <gate_uid> <num_layer>"

The gate uid and the layer number are uints.)foobar";

const std::string dcga_help =
R"foobar("dcga" command.
This command deletes all the connections to and from a gate.

Syntaxes:
1) "dcga <gate_uid>"
2) "dcga <gate_uid> <num_layer>"

The gate uid and the layer number are uints.

NOTE: this command internally runs \"dcgi\" and \"dcgo\" one after the other.)foobar";

const std::string bal_help =
R"foobar("bal" command.
This command adds multiple layers at once.

Syntax: "bal <uint> ..."
The unsigned integers are the number of the layers.

NOTE: the three dots indicate that the arguments specified before them can be repeated
multiple times.)foobar";

const std::string bag_help =
R"foobar("bag" command.
This command adds multiple gates at once.

Syntax: "bag <gate_type> <uint> ..."
The recognised gate types (case insensitive) are:
"buf", "not", "and", "or", "xor/exor", "nand", "nor", "nxor/nexor".
The unsigned integers are the number of the layer into which the respective gate will
be added.

NOTE: the three dots indicate that the arguments specified before them can be repeated
multiple times.)foobar";

const std::string bac_help =
R"foobar("bac" command.
This command adds multiple connections at once.

Syntax: "bac <gate_out_uid> <take_inv_output 0/1> <gate_in_uid> <num_input 0/1> ..."

The gate uids are unsigned integers that can be seen by printing the circuit to the screen.
The layer numbers are where the gates with the specified uids should be.
"take_inv_output" is a boolean that specifies to which output of the gate (normal or inverted) the
connection should start.
"num_input" is a boolean that specifies to which input of the gate the connection should end.

NOTE: the three dots indicate that the arguments specified before them can be repeated
multiple times.)foobar";

const std::string bdl_help =
R"foobar("bdl" command.
This command deletes multiple layers at once.

Syntax: "bdl <uint> ..."
The unsigned integer is the number of the layer.

NOTE: the three dots indicate that the arguments specified before them can be repeated
multiple times.)foobar";

const std::string bdg_help =
R"foobar("bdg" command.
This command deletes multiple gates at once.

Syntax : "bdg <gate_uid> ..."
The gate uid is an unsigned integer.

NOTE: the three dots indicate that the arguments specified before them can be repeated
multiple times.)foobar";

const std::string bdc_help =
R"foobar("bdc" command.
This command deletes multiple connections at once.

Syntax: "bdc <gate_in_uid> <num_input 0/1> ..."
The gate uids are unsigned integers that can be seen by printing the circuit to the screen.
"num_input" is a boolean that specifies to which input of the gate the connection should end.

NOTE: the three dots indicate that the arguments specified before them can be repeated
multiple times.)foobar";

const std::string si_help =
R"foobar("si" command.
This command sets the inputs of the circuit.

Syntax: "si <in0 0/1><in1 0/1><in2 0/1>..."
The string "si" is followed by a space and then a series of 1s and 0s, without spaces in between.
Example: suppose the circuit has 7 inputs -> "si 1010101".

NOTE: the number of bits specified as input must be the same as the number of inputs of the circuit,
without counting the constant-1 and constant-0 input gates.

NOTE: after setting the inputs, a "sc" command has to be given, in order to make the circuit react
to the set input.)foobar";

const std::string ro_help =
R"foobar("ro" command.
This command reads the output of the circuit.

Syntax: "ro"

NOTE: this outputs a series of 1s and 0s, where the first bit is the output of the first output buffer,
i.e. the one at the top of the layer, and the last bit is the output of the last outout buffer, i.e. the
one at the bottom of the layer.

NOTE: after setting the inputs, a "sc" command has to be given, in order to make the circuit react
to the set input.)foobar";

const std::string sc_help =
R"foobar("sc" command.
This command simulates the propagation of the signals in the circuit according to the given input.

Syntaxes:
1) "sc"
2) "sc <in0 0/1><in1 0/1><in2 0/1>..."
In syntax 2 the string "sc" is followed by a space and then a series of 1s and 0s, without spaces in between.
Example: suppose the circuit has 7 inputs -> "sc 1010101".
With this last syntax, the inputs are set and then the circuit is simulated.

NOTE: to view the output of the circuit, to see how it reacted to the inputs, use the command "ro".)foobar";

const std::string gtt_help =
R"foobar("gtt" command.
This command simulates the circuit over and over to generate a complete truth table.

Syntax: "gtt"
The truth table is printed on the screen with the following syntax:
<inputs> | <corresponding outputs>
<inputs> | <corresponding outputs>
<inputs> | <corresponding outputs>
...)foobar";

const std::string pc_help =
R"foobar("pc" command.
This command prints the circuit on the screen, in a (kind of) human readable form.

Syntaxes:
1) "pc"
2) "pc <print_gates 0/1>"
3) "pc <print_gates 0/1> <print_connections 0/1>"
In syntax 1 both parameters are assumed to be 1.
In syntax 2 "print_connections" is assumed to be 1.)foobar";

const std::string lu_help =
R"foobar("lu" command.
This command prints on the screen only the gates that have their inputs in an invalid,
state, which will cause a simulation to fail.

Syntax: "lu"
The output to the screen is similar to the one of the "pc" command.)foobar";

const std::string vc_help =
R"foobar("vc" command.
This command saves the circuit on a file.

Syntax: "vc <filename>"
The filename is a string.

Note: the file saved by the program is a text file. It can be viewed but should NOT be modified.)foobar";

const std::string lc_help =
R"foobar("lc" command.
This command loads a circuit from file.

Syntax: "lc <filename>"
The filename is a string.

Note: the file saved by the program is a text file. It can be viewed but should NOT be modified.)foobar";

const std::string gate_help =
R"foobar(This help will talk about how gates are and behave in this simulator.

First, here's a list of the supported gate types in the circuit:
buffers, NOT, AND, OR, XOR, NAND, NOR and NXOR gates.

Each of them has two inputs (input_0, in0 for short and input_1, in1 for short) and
two outputs (output, out for short and inv_output, iout for short).
Yes, buffers and NOT gates have two inputs as well, but they're a bit special,
more on this shortly.

For each gate to be in a valid state, both of its inputs have to be connected to an output
of another gate, except for the buffers and NOT gates. Outputs can be left unconnected.
Only one connection can lead to the input of a gate (adding another one to it overwrites
the old one), but multiple connections can start from the same output of a gate.
Buffers and NOT gates, to be in a valid state, have to have at least one of their inputs
connected. They can be both connected, in this case they will behave as an OR and a NOR
gate respectively.

Each gate has a normal output, and an inverted output. When the gate gets updated, they both
get updated. This effectively makes NAND, NOR and NXOR gates redundant, but it has been decided
to add this feature because it was easy to implement and to save a few NOT gates here and there.

Each gate has a method, internally, called "calc_output".
In this method, depending on the gate type, the input connections are checked and if they're valid,
again, depending on the gate type, the output gets computed and stored.
If anything goes wrong during this process, the method returns 1, otherwise it returns 0.)foobar";

const std::string circuit_help =
R"foobar(This help will talk about how the circuit is structured internally and how it's simulated.

A circuit is a set of gates and connections, with a certain amout of inputs and outputs.
The number of inputs and outputs can be set with the "nio" command, but executing it will DELETE ALL of
the contents of the circuit, so it's better to be executed before adding anything else to the circuit.

Gates are arranged in layers, each of them with a unique identifying number. The reason is explained
in a later paragraph.
Try to visualize layers as slices containing one or more gates, all of them right next to each other,
and the circuit as a set of layers, going from left to right, each of them one after the other, kind of
like a feed-forward neural network.

Layer 0 is the input layer, and layer -1 (which on 64 bit machines is actually layer
18446744073709551615, because the identifiers are unsigned integers) is the output layer of the circuit.
They're both filled with buffer gates, to easily inject and read data from them.
They can't be deleted nor gates can be added or removed from them.

You might notice that if a circuit has been created to have X inputs, the input layer has actually
X+2 buffer gates in it. This is because the first two buffer gates in the circuit are special, they're
always present and their uids will always be 0 and 1.
0 is a gate whose output is constantly set to 0, 1 is a gate whose output is constantly set to 1.
These two special buffers are there because every gate has to have its inputs connected somewhere to
succesfully simulate the circuit. If a user wants to test only a part of the circuit, the part not in exam
can be connected to these constants to "disable" it, or better, to make it insensitive to the stimuli given
to the circuit by the user.
When setting the inputs of the circuit, with the "si" command, only X bits have to be specified though, and
when generating the truth table with the "gtt" command, only the combinations of X bits are examined.
The gates in layer 0 are the only ones in the circuit that don't (and can't) have their inputs connected to
anything and the simulation still runs succesfully.

The number of buffers in the output layer is the same as the specified number of output bits of the circuit.
No special constant buffers in this layer.
They are normal buffers and as such, they have to have at least one of their inputs connected somewhere.

Since the user can't add nor remove gates from the input and output layer, to do anything interesting it's
necessary to add other layer of gates to the circuit. To do this the "al" command is provided.
Once a layer has been added, to add gates to it, the "ag" command is provided and to connect them the "ac"
command is provided.

This program is a "combinational logic circuit simulator". Note the specification of "combinational".
To ensure that the circuits built here are really combinational, connections can only be done forward (or
backwards, depending on how you want to see it).
This means that the output of a gate in layer 3, for example, can only be connected to the input of a gate
in a layer whose number is greater than 3.
This prevents loopbacks, the creation of memory cells, or situations where certain signals are undetermined
(for example, a NOT gate with its output connected directly to its input).
This also adds a bit of inconvenience if the user wants to add more intermediate circuitery between two
already existing layer, as not enough in-between layers might exist to fit all the circuitery.
For this reason, it's suggested to keep the initially added layers many numbers apart (for example, instead
of adding layers 2, 3 and 4, add layers 200, 300 and 400, or 2000, 3000 and 4000).

Once the circuit has been fully built, it can be simulated.
If the circuit is not fully connected, the simulation fails. To see unconnected gates, use the command "lu".

The circuit simulation is composed of 3 steps:
- setting the circuit inputs, with the "si" commands
- running the actual simulation of the circuit, to make the gates react to the stimuli, with the "sc" command
- reading the ouputs of the circuit, with the "ro" command
The first and second steps can also be combined in one by using a special syntax of the "sc" command.
What happens is:
- in step 1 the specified inputs are stored in the member "m_inputs" of the "circuit" class, then the contents of
  m_inputs are split in the buffers of the input layer.
- in step 2 the simulation goes through each gate of each layer, and computes its output, all the way to the output
  layer.
- in step 3 the outputs of the buffers in the output layer are assembled in a vector of bool that then gets printed
  on screen.
If in any of the gates the method "calc_output" fails, the entire simulation fails and an error is printed on screen.)foobar";

#endif