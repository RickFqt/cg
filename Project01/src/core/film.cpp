#include "film.h"
#include <cmath>

#include "api.h"
#include "image_io.h"
#include "paramset.h"

namespace rt3 {

//=== Film Method Definitions
Film::Film(const Point2i& resolution, const std::string& filename, image_type_e imgt)
    : m_full_resolution{ resolution }, m_filename{ filename }, m_image_type{ imgt } {
    
  // Update the size of the matrix, filling it with {0, 0, 0} color
  m_color_matrix = std::vector<std::vector<Color24>>(resolution[0], std::vector<Color24>(resolution[1],{0, 0, 0}));

}

Film::~Film() = default;

/// Add the color to image.
void Film::add_sample(const Point2f& pixel_coord, const Color24& pixel_color) {
  m_color_matrix[pixel_coord[0]][pixel_coord[1]] = pixel_color;
}

/// Convert image to RGB, compute final pixel values, write image.
void Film::write_image() const {
  

  // Vector that will store the full image. Each pixel is defined by its RGB, and an Alpha parameter in hte case of PNG
  unsigned char* byte_vector = new unsigned char[m_full_resolution[0] * m_full_resolution[1] * 4];
  int pixel = 0;
  for(int i{0}; i < m_full_resolution[0]; ++i){
    for(int j{0}; j < m_full_resolution[1]; ++j){

      byte_vector[pixel++] = m_color_matrix[i][j][0]; // R
      byte_vector[pixel++] = m_color_matrix[i][j][1]; // G
      byte_vector[pixel++] = m_color_matrix[i][j][2]; // B

      if(m_image_type == Film::image_type_e::PNG){
        byte_vector[pixel++] = 255;                     // Alpha
      }
    }
  }

  if(m_image_type == Film::image_type_e::PPM3){
    save_ppm3(byte_vector, m_full_resolution[0], m_full_resolution[1], 3, m_filename);
  }
  else if(m_image_type == Film::image_type_e::PPM6){
    save_ppm6(byte_vector, m_full_resolution[0], m_full_resolution[1], 3, m_filename);
  }
  else{
    save_png(byte_vector, m_full_resolution[0], m_full_resolution[1], 4, m_filename);
  }
}

// Factory function pattern.
// This is the function that retrieves from the ParamSet object
// all the information we need to create a Film object.
Film* create_film(const ParamSet& ps) {
  std::cout << ">>> Inside create_film()\n";
  std::string filename;
  // Let us check whether user has provided an output file name via
  // command line arguments in main().
  if (not API::curr_run_opt.outfile.empty()) {
    // Retrieve filename defined via CLI.
    filename = API::curr_run_opt.outfile;
    // Retrieve another filename, now from the ParamSet object.
    std::string filename_from_file = retrieve(ps, "filename", std::string{ "" });
    if (not filename_from_file.empty()) {  // We also get a filename from scene file...
      RT3_WARNING(string{ "Output filename supplied on command line, \"" }
                  + API::curr_run_opt.outfile
                  + string{ "\" is overriding filename provided in scene "
                            "description file, \"" }
                  + filename_from_file);
    }
  } else {
    // Try yo retrieve filename from scene file.
    filename = retrieve(ps, "filename", std::string{ "image.png" });
  }

  // Read resolution.
  // Aux function that retrieves info from the ParamSet.
  int xres = retrieve(ps, "x_res", int(1280));
  // Aux function that retrieves info from the ParamSet.
  int yres = retrieve(ps, "y_res", int(720));
  // Quick render?
  if (API::curr_run_opt.quick_render) {
    // decrease resolution.
    xres = std::max(1, xres / 4);
    yres = std::max(1, yres / 4);
  }

  // TODO
  // Read crop window information.
  std::vector<real_type> cw = retrieve(ps, "crop_window", std::vector<real_type>{ 0, 1, 0, 1 });
  std::cout << "Crop window ";
  for (const auto& e : cw) {
    std::cout << e << " ";
  }
  std::cout << '\n';

  std::string img_type = retrieve(ps, "img_type", std::string{ "" });
  if(img_type.empty()){
    img_type = "ppm3";
  }

  // Gets the image format. the default being PNG
  Film::image_type_e img_type_e;
  if(img_type == "ppm3"){
    img_type_e = Film::image_type_e::PPM3;
  }
  else if(img_type == "ppm6"){
    img_type_e = Film::image_type_e::PPM6;
  }
  else{
    img_type_e = Film::image_type_e::PNG;
  }
  // Note that the image type is fixed here. Must be read from ParamSet, though.
  return new Film(Point2i{ xres, yres }, filename, img_type_e);
}
}  // namespace rt3
