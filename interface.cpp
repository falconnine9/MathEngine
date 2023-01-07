// Basic Input/Output console interface for the math engine

#include <vector>
#include <string>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include "engine/engine.h"

static char _kbuffer[512];

int main() {
    mathengine::set_trigmode(mathengine::TRIGMODE_DEGREES);

    while (true) {
        std::cout << "> ";

        fgets(_kbuffer, 512, stdin);
        std::string s(_kbuffer);
        s.erase(s.end() - 1);

        std::vector<mathengine::token> lex = mathengine::tokenize_infix(s);
        std::vector<mathengine::token> rpn = mathengine::create_rpn(lex);

        long double result;
        try {
            result = mathengine::evaluate_rpn(rpn);
            std::cout << std::to_string(result) << std::endl << std::endl;
        }
        catch (mathengine::exc_divbyzero exc) {
            std::cout << "Error: " << exc.what() << std::endl << std::endl;
        }
        catch (mathengine::exc_nonintegral exc) {
            std::cout << "Error: " << exc.what() << std::endl << std::endl;
        }
        catch (mathengine::exc_nonreal exc) {
            std::cout << "Error: " << exc.what() << std::endl << std::endl;
        }
        catch (std::out_of_range) {
            std::cout << "Error: Syntax error" << std::endl << std::endl;
        }
    }
}