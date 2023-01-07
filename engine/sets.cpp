/* sets.cpp
 *
 * Provides methods that allow the way the engine works to be modified. These
 * modifications currently include changing the trigonometry mode (either to
 * degree or radian mode) and submitting external functions into the function
 * index.
 */

#include <engine/engine.h>

int mathengine::_trigmode;
std::vector<std::string> mathengine::_efindex;
std::vector<std::function<long double(long double)>> mathengine::_efvalues;

void mathengine::set_trigmode(int value) {
    if (value >= 0 && value <= 1)
        mathengine::_trigmode = value;
}

void mathengine::create_externfunc(std::string name, std::function<long double(long double)> f) {
    mathengine::_efindex.push_back(name);
    mathengine::_efvalues.push_back(f);
}