#include <bits/stdc++.h>
#include "lodepng.h"

using byte = unsigned char;

float lerp(float v0, float v1, float t){ return (1-t) * v0 + t * v1;}

struct RGB {
    byte r = 0;
    byte g = 0;
    byte b = 0;
};

//Encode from raw pixels to disk with a single function call
//The image argument has width * height RGBA pixels or width * height * 4 bytes
void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
  //Encode the image
  unsigned error = lodepng::encode(filename, image, width, height);

  //if there's an error, display it
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}




int main(){

    std::cout << "P3\n";
    std::cout << "400 200\n";
    std::cout << "255\n";

    std::vector<byte> byte_vector(400 * 200 * 4);


    int pixel = 0;
    for(int h{0}; h < 200; ++h){
        for(int w{0}; w < 400; ++w){
            std::cout << (int)lerp(0, 255, (float)w/(float)400) << " ";
            std::cout << (int)lerp(0, 255, 1 - (float)h/(float)200) << " ";
            std::cout << (int)lerp(0, 255, 0.2) << "\n";

            byte_vector[pixel++] = lerp(0, 255, (float)w/(float)400);
            byte_vector[pixel++] = lerp(0, 255, 1 - (float)h/(float)200);
            byte_vector[pixel++] = lerp(0, 255, 0.2);
            byte_vector[pixel++] = 255;
            


        }

        encodeOneStep("out.png", byte_vector, 400, 200);
    }
    return 0;
}