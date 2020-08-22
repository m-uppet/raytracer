#ifndef TEXTURE_H
#define TEXTURE_H

class texture {
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_texture : public texture {
public:
    solid_texture() {}
    solid_texture(color c) : color_value(c) {}

    solid_texture(double red, double green, double blue)
        : solid_texture(color(red, green, blue)) {}
    
    virtual color value(double u, double v, const point3& p) const override {
        return color_value;
    }

private:
    color color_value;
};


#endif