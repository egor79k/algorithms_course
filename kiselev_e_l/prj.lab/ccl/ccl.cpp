#include <iostream>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <m3i/m3i.h>


class CCLSolver {
public:
    void readFromStr(std::istream& istr) {
        istr >> space;
    }


    void writeToStr(std::ostream& ostr) {
        ostr << space;
    }


    bool readFromImg(const int imgs_num, char* imgs[]) {
        int first_width = 0;
        int first_height = 0;

        for (int img_id = 0; img_id < imgs_num; ++img_id) {
            int width = 0;
            int height = 0;
            int channels = 0;

            unsigned char *img = stbi_load(imgs[img_id], &width, &height, &channels, 0);

            if (nullptr == img) {
               printf("Error in loading the image '%s'\n", imgs[img_id]);
               return false;
            }

            if (first_width == 0) {
                first_width = width;
                first_height = height;
                space.Resize(imgs_num, height, width);
            }

            if (height != first_height || width != first_width) {
                printf("Wrong image size '%s'\n", imgs[img_id]);
                return false;
            }

            for (int h = 0; h < height; ++h) {
                for (int w = 0; w < width; ++w) {
                    space.At(img_id, h, w) = img[(h * width + w) * channels] / 255;
                }
            }
        }

        return true;
    }


    bool writeToImg(const int imgs_num, char* imgs[]) {
        if (imgs_num != space.Size(0)) {
            printf("Wrong output images count. Given: %d, must be: %d\n", imgs_num, space.Size(0));
            return false;
        }
        
        int height = space.Size(1);
        int width = space.Size(2);
        int channels = 3;
        unsigned char *img = new unsigned char[height * width * channels];

        for (int img_id = 0; img_id < imgs_num; ++img_id) {
            // memset(img, 0, height * width * channels);

            for (int h = 0; h < height; ++h) {
                for (int w = 0; w < width; ++w) {
                    int label = space.At(img_id, h, w);
                    
                    if (0 == label) {
                        img[h * width * channels + w * channels + 0] = 0;
                        img[h * width * channels + w * channels + 1] = 0;
                        img[h * width * channels + w * channels + 2] = 0;
                        continue;
                    }

                    if (1 == label) {
                        img[h * width * channels + w * channels + 0] = 255;
                        img[h * width * channels + w * channels + 1] = 255;
                        img[h * width * channels + w * channels + 2] = 255;
                        continue;
                    }

                    unsigned char chan = 255 * static_cast<float>(label) / curr_label;

                    img[h * width * channels + w * channels + 0] = 255;
                    img[h * width * channels + w * channels + 1] = chan;
                    img[h * width * channels + w * channels + 2] = 255 - chan;
                }
            }

            stbi_write_png(imgs[img_id], width, height, channels, img, width * channels);
        }

        delete[] img;
        return true;
    }


    void solve() {
        for (int x = 0; x < space.Size(0); ++x) {
            for (int y = 0; y < space.Size(1); ++y) {
                for (int z = 0; z < space.Size(2); ++z) {
                    if (space.At(x, y, z) == 1) {
                        labelComponent(x, y, z);
                        ++curr_label;
                    }
                }
            }
        }
    }


private:
    void labelComponent(const int x, const int y, const int z) {
        space.At(x, y, z) = curr_label;

        if (x + 1 < space.Size(0)) {
            if (space.At(x + 1, y, z) == 1) {
                labelComponent(x + 1, y, z);
            }
        }

        if (x > 0) {
            if (space.At(x - 1, y, z) == 1) {
                labelComponent(x - 1, y, z);
            }
        }

        if (y + 1 < space.Size(1)) {
            if (space.At(x, y + 1, z) == 1) {
                labelComponent(x, y + 1, z);
            }
        }

        if (y > 0) {
            if (space.At(x, y - 1, z) == 1) {
                labelComponent(x, y - 1, z);
            }
        }

        if (z + 1 < space.Size(2)) {
            if (space.At(x, y, z + 1) == 1) {
                labelComponent(x, y, z + 1);
            }
        }

        if (z > 0) {
            if (space.At(x, y, z - 1) == 1) {
                labelComponent(x, y, z - 1);
            }
        }
    }


    M3i space;
    int curr_label = 2;
};


int main(int argc, char* argv[]) {
    CCLSolver solver;

    if (argc > 1) {
        solver.readFromImg(argc - 1, &(argv[1]));
        solver.solve();

        for (int i = 1; i < argc; ++i) {
            argv[i][4] = 'o';
        }

        solver.writeToImg(argc - 1, &(argv[1]));
        solver.writeToStr(std::cout);
    }
    else {
        solver.readFromStr(std::cin);
        solver.solve();
        solver.writeToStr(std::cout);
    }
    
    return 0;
}