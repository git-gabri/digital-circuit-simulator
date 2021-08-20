# Digital circuit simulator
This is a command line digital combinational circuit simulator that I wrote in C++.  
It's more of an academic exercise than anything useful, but it's fun to play with it.  

### Compiling and running
To compile, `cmake` and a C++ compiler are required.  
`cd` to the folder where the `main.cpp` is located.  
Then `mkdir build; cd build`  
Then `cmake -DCMAKE_BUILD_TYPE:STRING=Release ..`  
And in the end `cmake --build .`  
You should now have an executable file called `simulator` in the `build` folder, that you can run using `./simulator`

### Help
The help is built into the program itself. Just type `help` or `h` in the simulator console and a help menu will be printed on screen.

### `circuit.hpp`
The `circuit` class used in the simulator is thought to be independent from the rest of the code.
It can be easily extracted (along with the implementation of all the methods in the `circuit.cpp` file) and used in other projects.  
**Note:** a proper copy-constructor has to be implemented for this class.