#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void draw() const = 0;
};

#endif // !DRAWABLE_HPP
