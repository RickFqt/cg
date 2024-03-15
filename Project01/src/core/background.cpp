#include "background.h"

namespace rt3 {



/*!
 * Samples a color base on spherical interpolation of an image ou colored
 * background.
 *
 * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
 * \return The interpolated color.
 */
Color24 Background::sampleXYZ(const Point2f& pixel_ndc) const {
  return Color24{ 0, 0, 0 };
}

/**
 * Linear interpolation between two Colors
 * 
 * @param A The first color
 * @param B The second color
 * @param t The float value that determines the influence of each color.
 * @return The linearly interpolated color in [A;B]. If t = 0, returns A. If t = 1, returns B.
 */
Color24 BackgroundColor::lerp_colors( const Color24 &A, const Color24 &B, float t ) const{

  uint8_t r = (uint8_t)Lerp(t, A[0], B[0]);
  uint8_t g = (uint8_t)Lerp(t, A[1], B[1]);
  uint8_t b = (uint8_t)Lerp(t, A[2], B[2]);
  return Color24{r, g, b};
}

/*!
 * Samples a color base on spherical interpolation of an image ou colored
 * background.
 *
 * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
 * \return The interpolated color.
 */
Color24 BackgroundColor::sampleXYZ(const Point2f& pixel_ndc) const {
  Color24 color_top = lerp_colors(corners[tl], corners[tr], pixel_ndc[0]);
  Color24 color_bottom = lerp_colors(corners[bl], corners[br], pixel_ndc[0]);
  return lerp_colors(color_top, color_bottom, pixel_ndc[1]);
}

BackgroundColor* create_color_background(const ParamSet& ps) {
  Color24 color = retrieve(ps, "color", Color24{255, 255, 255});
  std::vector<Color24> bkg_corners;

  // If there exists a "color" keyword, we use its value
  if(ps.count("color") >= 1){
    bkg_corners = {color, color, color, color};
    return new BackgroundColor(bkg_corners);
  }
  else{
    // Find the 4 corners, and save them in the background
    Color24 color_bl = retrieve(ps, "bl", Color24{0, 0, 0});
    Color24 color_tl = retrieve(ps, "tl", Color24{0, 0, 0});
    Color24 color_tr = retrieve(ps, "tr", Color24{0, 0, 0});
    Color24 color_br = retrieve(ps, "br", Color24{0, 0, 0});
    bkg_corners = {color_bl, color_tl, color_tr, color_br};
    return new BackgroundColor(bkg_corners);
  }
}
}  // namespace rt3
