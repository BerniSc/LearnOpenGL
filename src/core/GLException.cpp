#include "core/GLException.hpp"

GLException::GLException(const std::string& msg) : reason(msg)  {

}
 
const std::string& GLException::what() const {
    return reason;
}

std::ostream& operator<<(std::ostream& os, const GLException& ex) {
    return os << "GLException: " << ex.what();
}
