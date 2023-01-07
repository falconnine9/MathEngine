#ifndef _MATHENGINE_H
#define _MATHENGINE_H 1

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace mathengine {
    class token {
    public:
        const static int VALUE_TOKEN = 0;
        const static int OP_TOKEN = 1;
        const static int FUNC_TOKEN = 2;

        const static int OP_ADD = 0;
        const static int OP_SUB = 1;
        const static int OP_MUL = 2;
        const static int OP_DIV = 3;
        const static int OP_MOD = 4;
        const static int OP_POW = 5;
        const static int OP_LB = 6;
        const static int OP_RB = 7;

        const static int FUNC_SQRT = 0;
        const static int FUNC_SIN = 1;
        const static int FUNC_ASIN = 2;
        const static int FUNC_COS = 3;
        const static int FUNC_ACOS = 4;
        const static int FUNC_TAN = 5;
        const static int FUNC_ATAN = 6;
        const static int FUNC_LOG = 7;
        const static int FUNC_FACT = 8;
        const static int FUNC_ABS = 9;
        const static int FUNC_ROUND = 10;
        const static int FUNC_CEIL = 11;
        const static int FUNC_FLOOR = 12;
        const static int FUNC_GAMMA = 13;
        
        int type;
        long double value;

        token(int type, long double value);
    };

    class exc_divbyzero {
    public:
        const char* what() {
            return "Division by zero";
        }
    };

    class exc_nonintegral {
    public:
        const char* what() {
            return "Non-integral given value";
        }
    };

    class exc_nonreal {
    public:
        const char* what() {
            return "Non-real result value";
        }
    };

    const int TRIGMODE_DEGREES = 0;
    const int TRIGMODE_RADIANS = 1;

    extern int _trigmode;
    extern std::vector<std::string> _efindex;
    extern std::vector<std::function<long double(long double)>> _efvalues;

    void set_trigmode(int value);
    void create_externfunc(std::string name, std::function<long double(long double)> f);

    std::vector<token> tokenize_infix(std::string infix);
    std::vector<token> create_rpn(std::vector<mathengine::token> tokens);
    long double evaluate_rpn(std::vector<mathengine::token> rpn);
}

#endif