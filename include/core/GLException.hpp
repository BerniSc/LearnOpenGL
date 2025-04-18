#ifndef GLEXCEPTION_HPP
#define GLEXCEPTION_HPP

#include <string>
#include <ostream>

class GLException {
    private:
        std::string reason;
    public:
        explicit GLException(const std::string& msg);
        const std::string& what() const;
};

std::ostream& operator<<(std::ostream& os, const GLException& ex);

#endif // !GLEXCEPTION_HPP
