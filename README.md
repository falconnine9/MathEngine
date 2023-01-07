# C++ Math Engine

This is a project that I originally intended to just be for my computer science class at school. However I decided that it's probably a good idea to keep it on github for safe-keeping.

### The library

The library comes with 3 functions for all the stuff you might need. They are all stages of the engine working but you dont need to use all of them (like if you have the correct input for the final evaluation function you can just go from there).

However, if all you have is a string of something like `3 + 7 * sin(30)` you will have to use all 3 functions to get your end result as a the correct number.

Guides for all the functions and other side stuff are in `lib-guide.md`

### The interface

I made a simple console interface that just lets the user give an equation and it'll return a result. This isn't necessary to the engine as a whole but it gives an idea of it's uses.

For example, if you wanted to make your own interface using a GUI with buttons to create the equation, you could just skip the lexer stage and give the parser a list of tokens created from the user pressing these buttons.

Or if for some reason you ever wanted to force the user to give an equation in the format of RPN you could skip both the lexer and the parser and just give the evaluator that RPN input directly.