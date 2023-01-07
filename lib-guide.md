# C++ Math Engine library guide

Everything is encased in the `mathengine` namespace

### Core methods

 - `tokenize_infix`: Breaks a raw infix string into it's respective tokens (lexer)
 - `create_rpn`: Rearranges and adds/removes tokens to build an RPN array from an infix token array (parser). **This method may throw an undefined error if a syntax error occurs**
 - `evaluate_rpn`: Returns a resultant number from an RPN token array (evaluator). **This method may throw a math-based error if there was an illegal operation. It's contents can be found in the .what()**

### Utility and settings methods
 
 - `set_trigmode`: Sets the trigonometry functions in the engine to degree or radian mode
 - `create_externfunc`: Adds a function to the external function table with the identifier provided. The given function must take 1 `long double` type and return 1 `long double` type

### Token class

The token class is the overall container for each bit of data used by the engine.
 - type: The type of token (operator, function call, number value)
 - value: Either the number value of the token or the operator/function call made
