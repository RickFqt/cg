#include "backgroundcolor.h"

BackgroundColor::BackgroundColor(const std::vector< RGBColor >& colors){
    corners[bl] = colors[bl];
    corners[tl] = colors[tl];
    corners[tr] = colors[tr];
    corners[br] = colors[br];
}

RGBColor BackgroundColor::lerp( const RGBColor &A, const RGBColor &B, float t ) const{
    RGBColor new_color;
    new_color.r = (1-t) * A.r + t * B.r;
    new_color.g = (1-t) * A.g + t * B.g;
    new_color.b = (1-t) * A.b + t * B.b;
    return new_color;
}

RGBColor BackgroundColor::sampleUV( float u, float v )const{
    RGBColor color_top = lerp(corners[tl], corners[tr], u);
    RGBColor color_bottom = lerp(corners[bl], corners[br], u);
    return lerp(color_top, color_bottom, v);
}