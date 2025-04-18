#ifndef GL_EXCEPTION_HPP
#define GL_EXCEPTION_HPP

#include <string>

class GLException {
    private:
        std::string reason;

    public:
        explicit GLException(const std::string& message);
        const std::string& what() const;
};

std::ostream& operator<<(std::ostream& os, const GLException& ex);

#endif // !GL_EXCEPTION_HPP
