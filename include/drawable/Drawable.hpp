#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <vector>
#include "core/VertexAttribute.hpp"

class Drawable {
    protected:
        bool wireframeMode = false;

        // Configure the used Vertices to allow each Drawable to pass stuff like Colour etc via the Vertex Argument
        // For Position and Color alone we would have to switch up the offset and the index, therefore pass all at once via Vector
        void inline configureVertexAttributes(GLsizei stride, const std::vector<VertexAttribute>& attributes) const {
            for(const auto& attr : attributes) {
                glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalized, stride, (void*) attr.offset);
                glEnableVertexAttribArray(attr.index);
            }
        }

    public:
        virtual ~Drawable() = default;
        virtual void draw() const = 0;
};

#endif // !DRAWABLE_HPP
