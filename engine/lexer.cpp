/* lexer.cpp
 *
 * A lexer (otherwise known as a tokenizer) is an important step in using formatted
 * user input to run operations. It will go through each character in the string of
 * input and determine if that character is part of a larger token (ie. a digit in a
 * number) or rather a token by itself (ie. add/subtract).
 * 
 * The lexer will accept any infix formatted string.
 */

#include <map>
#include <string>
#include <vector>
#include <engine/engine.h>

static std::map<char, int> _operators {
    {'+', mathengine::token::OP_ADD},
    {'-', mathengine::token::OP_SUB},
    {'*', mathengine::token::OP_MUL},
    {'/', mathengine::token::OP_DIV},
    {'%', mathengine::token::OP_MOD},
    {'^', mathengine::token::OP_POW},
    {'(', mathengine::token::OP_LB},
    {')', mathengine::token::OP_RB}
};

static std::map<std::string, int> _functions {
    {"sqrt", mathengine::token::FUNC_SQRT},
    {"sin", mathengine::token::FUNC_SIN},
    {"asin", mathengine::token::FUNC_ASIN},
    {"cos", mathengine::token::FUNC_COS},
    {"acos", mathengine::token::FUNC_ACOS},
    {"tan", mathengine::token::FUNC_TAN},
    {"atan", mathengine::token::FUNC_ATAN},
    {"log", mathengine::token::FUNC_LOG},
    {"fact", mathengine::token::FUNC_FACT},
    {"abs", mathengine::token::FUNC_ABS},
    {"round", mathengine::token::FUNC_ROUND},
    {"ceil", mathengine::token::FUNC_CEIL},
    {"floor", mathengine::token::FUNC_FLOOR},
    {"gamma", mathengine::token::FUNC_GAMMA}
};

static std::map<std::string, long double> _constants {
    {"E", 1.6066951524},
    {"e", 2.7182818284},
    {"pi", 3.14159265359},
    {"\u03C0", 3.14159265359},
    {"gr", 1.6180339887},
    {"\u03C6", 1.6180339887},
    {"sgr", 1.4655712318},
    {"\u03C8", 1.4655712318},
    {"ec", 0.5772156649},
    {"\u03B3", 0.5772156649},
    {"om", 0.5671432904},
    {"\u03A9", 0.5671432904},
};

// Simplified operator confirmation
static bool _op_of(mathengine::token tok, int type) {
    return tok.type == mathengine::token::OP_TOKEN && tok.value == type;
}

// Simplified finding of an operator in the list
static bool _is_operator(char c) {
    std::map<char, int>::iterator it;
    return (it = _operators.find(c)) != _operators.end();
    
}

// Whether a character falls between the digit values in ASCII
static bool _is_digit(char c) {
    return (c >= '0' && c <= '9') || c == '.';
}

// Finds if a given function is a constant or an actual function
static mathengine::token _get_function(std::string s) {
    if (_constants.find(s) != _constants.end())
        return mathengine::token(mathengine::token::VALUE_TOKEN, _constants[s]);
    
    if (_functions.find(s) != _functions.end())
        return mathengine::token(mathengine::token::FUNC_TOKEN, _functions[s]);
    
    for (int i = 0; i < mathengine::_efindex.size(); i++) {
        if (mathengine::_efindex.at(i) == s)
            return mathengine::token(mathengine::token::FUNC_TOKEN, i + 0x40);
    }
    
    return mathengine::token(0, 0);
}

// Pushes whatever is in the current buffer with the correct token type
static void _push_buffer(std::vector<mathengine::token>* tokens, std::string* buffer, int type) {
    if (buffer->size() == 0)
        return;

    size_t idx;
    switch (type) {
        case mathengine::token::OP_TOKEN:
            tokens->push_back(mathengine::token(type, _operators[buffer->at(0)]));
            break;
        
        case mathengine::token::VALUE_TOKEN:
            tokens->push_back(mathengine::token(type, stod(*buffer, &idx)));
            break;
        
        case mathengine::token::FUNC_TOKEN:
            tokens->push_back(_get_function(*buffer));
            break;
    }

    *buffer = "";
}

// The lexer will originally find all subtract operators as a subtract command, this
// will determine if they are supposed to be a negative operator or subtraction
static void _find_negatives(std::vector<mathengine::token>* tokens) {
    for (int i = 0; i < tokens->size(); i++) {
        mathengine::token tok = tokens->at(i);
        if (tok.type != mathengine::token::OP_TOKEN  || tok.value != mathengine::token::OP_SUB) {
            continue;
        }
        
        if (i == 0 || (tokens->at(i - 1).type == mathengine::token::OP_TOKEN)) {
            tokens->erase(tokens->begin() + i);
            tokens->at(i).value *= -1;
            i = -1;
        }
    }
}

static void _insert_mul(std::vector<mathengine::token>* tokens) {
    auto _needs_mul = [](mathengine::token tok, mathengine::token ntok)->bool {
        if (_op_of(tok, mathengine::token::OP_RB) || tok.type == mathengine::token::VALUE_TOKEN) {
            return _op_of(ntok, mathengine::token::OP_LB) ||
                ntok.type == mathengine::token::FUNC_TOKEN ||
                ntok.type == mathengine::token::VALUE_TOKEN;
        }
        else
            return false;
    };

    for (int i = 0; i < tokens->size(); i++) {
        if (i == tokens->size() - 1)
            break;

        mathengine::token tok = tokens->at(i);
        mathengine::token ntok = tokens->at(i + 1);

        if (_needs_mul(tok, ntok)) {
            tokens->insert(tokens->begin() + i + 1, mathengine::token(
                mathengine::token::OP_TOKEN,
                mathengine::token::OP_MUL
            ));
            i = -1;
        }
    }
}

// Turns raw string infix into a infix formatted token array
std::vector<mathengine::token> mathengine::tokenize_infix(std::string infix) {
    std::vector<mathengine::token> tokens;
    std::string buffer;
    bool number;

    for (int i = 0; i < infix.size(); i++) {
        char c = infix.at(i);

        if (c == ' ')
            continue;
        else if (_is_operator(c)) {
            _push_buffer(&tokens, &buffer,
                number
                ? mathengine::token::VALUE_TOKEN
                : mathengine::token::FUNC_TOKEN);
            buffer += c;
            _push_buffer(&tokens, &buffer, mathengine::token::OP_TOKEN);
        }
        else if (_is_digit(c)) {
            if (!number) {
                _push_buffer(&tokens, &buffer, mathengine::token::FUNC_TOKEN);
                number = true;
            }
            buffer += c;
        }
        else {
            if (number) {
                _push_buffer(&tokens, &buffer, mathengine::token::VALUE_TOKEN);
                number = false;
            }
            buffer += c;
        }
    }
    _push_buffer(&tokens, &buffer,
        number
        ? mathengine::token::VALUE_TOKEN
        : mathengine::token::FUNC_TOKEN);
    
    _find_negatives(&tokens);
    _insert_mul(&tokens);
    return tokens;
}