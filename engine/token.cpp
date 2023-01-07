#include <engine/engine.h>

mathengine::token::token(int type, long double value) {
    this->type = type;
    this->value = value;
}