#include "myWrappers/GLException.hpp"

#include <ostream>

GLException::GLException(const std::string& message) : reason(message) {

}

const std::string& GLException::what() const {
    return reason;
}

std::ostream& operator<<(std::ostream& os, const GLException& ex) {
    return os << "GLException: " << ex.what();
}


