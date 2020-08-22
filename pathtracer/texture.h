#ifndef TEXTURE_H
#define TEXTURE_H

class texture {
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color() {}
    solid_color(color c) : color_value(c) {}

    solid_color(double red, double green, double blue)
        : solid_color(color(red, green, blue)) {}
    
    virtual color value(double u, double v, const point3& p) const override {
        return color_value;
    }

private:
    color color_value;
};

#endif