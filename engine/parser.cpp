/* parser.cpp
 *
 * Running correct BEDMAS rules with infix input is a very hard task. Some
 * operators with higher precedence than others may be detected after the other
 * has already run (breaking BEDMAS rules). This part of the engine uses the
 * shunting-yard algorithm to convert infix notation to RPN (reverse polish
 * notation), which is designed in a more computer comprehensible manner.
 *
 * This step should be run with the output from the lexer. However, direct input
 * from whatever program is running this library is also allowed, assuming it's
 * in correct infix notation just like what the lexer would output.
 */

#include <vector>
#include <stack>
#include <string>
#include <engine/engine.h>

#include <iostream>

// Simplified operator confirmation
static bool _op_of(mathengine::token tok, int type) {
    return tok.type == mathengine::token::OP_TOKEN && tok.value == type;
}

// Operator and function precedence when there's a right bracket active
static bool _rb_preced(std::stack<mathengine::token>* op, mathengine::token tok) {
    if (op->empty())
        return false;
    mathengine::token opt = op->top();
    return !_op_of(opt, mathengine::token::OP_LB);
}

// Operator and function precedence during normal execution
static bool _op_preced(std::stack<mathengine::token> *op, mathengine::token tok) {
    if (op->empty())
        return false;
    mathengine::token opt = op->top();
    return !_op_of(opt, mathengine::token::OP_LB) &&
        (opt.value > tok.value || opt.type == mathengine::token::FUNC_TOKEN);
}

// Converts the infix token array to an RPN based token array
std::vector<mathengine::token> mathengine::create_rpn(std::vector<mathengine::token> tokens) {
    std::vector<mathengine::token> rpn;
    std::stack<mathengine::token> operators;

    for (int i = 0; i < tokens.size(); i++) {
        mathengine::token tok = tokens.at(i);

        switch (tok.type) {
            case mathengine::token::VALUE_TOKEN:
                rpn.push_back(tok);
                break;
            
            case mathengine::token::FUNC_TOKEN:
                while (!operators.empty() && operators.top().type == mathengine::token::FUNC_TOKEN) {
                    rpn.push_back(operators.top());
                    operators.pop();
                }
                operators.push(tok);
                break;
            
            case mathengine::token::OP_TOKEN:
                if (tok.value == mathengine::token::OP_LB)
                    operators.push(tok);
                else if (tok.value == mathengine::token::OP_RB) {
                    while (_rb_preced(&operators, tok)) {
                        rpn.push_back(operators.top());
                        operators.pop();
                    }
                    operators.pop();
                }
                else {
                    while (_op_preced(&operators, tok)) {
                        rpn.push_back(operators.top());
                        operators.pop();
                    }
                    operators.push(tok);
                }
        }
    }

    while (!operators.empty()) {
        rpn.push_back(operators.top());
        operators.pop();
    }

    return rpn;
}