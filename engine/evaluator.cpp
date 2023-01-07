/* evaluator.cpp
 *
 * Evaluates a RPN formatted token array into a single resultant number.
 * 
 * Normally, you'd want input for the evaluation method to be from the parser,
 * however - just like the parser - it can also be directly given input from
 * whatever is running this library (assuming it is in correct RPN format).
 * 
 * The evaluation method will normally return a 128 bit decimal value, however
 * there are also cases when it will throw an error (division by 0, non-integral,
 * non-real, etc).
 */

#include <math.h>
#include <vector>
#include <engine/engine.h>

// Factorial function for C
static long double _factorial(long double a) {
    long double fact = 1.0;
    for (int n = 1; n <= a; fact *= n++);
    return fact;
}

// Handles operator commands
static long double _operator(long double a, long double b, int op) {
    switch (op) {
        case mathengine::token::OP_ADD:
            return a + b;
        
        case mathengine::token::OP_SUB:
            return a - b;
        
        case mathengine::token::OP_MUL:
            return a * b;
        
        case mathengine::token::OP_DIV:
            if (b == 0)
                throw mathengine::exc_divbyzero();
            return a / b;
        
        case mathengine::token::OP_MOD:
            if ((int)a != a || (int)b != b)
                throw mathengine::exc_nonintegral();
            return (int)a % (int)b;
        
        case mathengine::token::OP_POW:
            return pow(a, b);
    }
}

// Handles function commands
static long double _function(long double a, int func) {
    if (func >= 0x40)
        return mathengine::_efvalues.at(func - 0x40)(a);
    
    if (func >= mathengine::token::FUNC_SIN && func <= mathengine::token::FUNC_ATAN)
        a = mathengine::_trigmode == mathengine::TRIGMODE_DEGREES ? a * (3.14159265359 / 180) : a;

    switch (func) {
        case mathengine::token::FUNC_SQRT:
            if (a < 0)
                throw mathengine::exc_nonreal();
            return sqrt(a);
        
        case mathengine::token::FUNC_SIN:
            return sin(a);
        
        case mathengine::token::FUNC_ASIN:
            return asin(a);
        
        case mathengine::token::FUNC_COS:
            return cos(a);
        
        case mathengine::token::FUNC_ACOS:
            return acos(a);
        
        case mathengine::token::FUNC_TAN:
            return tan(a);
        
        case mathengine::token::FUNC_ATAN:
            return atan(a);
        
        case mathengine::token::FUNC_LOG:
            if (a < 0)
                throw mathengine::exc_nonreal();
            return log(a);
        
        case mathengine::token::FUNC_FACT:
            if (a < 0)
                throw mathengine::exc_nonreal();
            return _factorial(a);
        
        case mathengine::token::FUNC_ABS:
            return abs(a);
        
        case mathengine::token::FUNC_ROUND:
            return round(a);
        
        case mathengine::token::FUNC_CEIL:
            return ceil(a);
        
        case mathengine::token::FUNC_FLOOR:
            return floor(a);
        
        case mathengine::token::FUNC_GAMMA:
            return tgamma(a);
    }
}

// Evaluates the RPN formatted token array to a single number result
long double mathengine::evaluate_rpn(std::vector<mathengine::token> rpn) {
    mathengine::token first(mathengine::token::VALUE_TOKEN, 0);
    mathengine::token second(mathengine::token::VALUE_TOKEN, 0);

    for (int idx = 0; idx < rpn.size() && rpn.size() > 1;) {
        mathengine::token idx_token = rpn.at(idx);

        switch (idx_token.type) {
            case mathengine::token::VALUE_TOKEN: {
                second = first;
                first = idx_token;
                idx++;
                break;
            }
            
            case mathengine::token::OP_TOKEN: {
                long double op_result = _operator(second.value, first.value, idx_token.value);
                rpn.erase(rpn.begin() + idx - 2, rpn.begin() + idx + 1);
                rpn.insert(rpn.begin() + idx - 2, mathengine::token(mathengine::token::VALUE_TOKEN, op_result));
                idx = 0;
                break;
            }
            
            case mathengine::token::FUNC_TOKEN: {
                long double func_result = _function(first.value, idx_token.value);
                rpn.erase(rpn.begin() + idx - 1, rpn.begin() + idx + 1);
                rpn.insert(rpn.begin() + idx - 1, mathengine::token(mathengine::token::VALUE_TOKEN, func_result));
                idx = 0;
                break;
            }
        }
    }

    return rpn.at(0).value;
}