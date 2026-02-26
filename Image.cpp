//
// Created by floodd on 26/01/2026.
//

#include "Image.h"

#include <iostream>

/**
 * This function creates the image object, and then loads in the image from the given filename.
 *
 * @param filename: The name of the file to load
 * /
 */
MyImage::MyImage(string filename): fileName(filename) {
    this->load(filename);
}

/**
 *  This function will reload the original image.
 */
void MyImage::reload() {
    this->load(fileName);
}

/**
 *  This function is responsible for drawing the image scaling to fit on the screen. It also draws a grey
 *  background behind the image to allow for scaling.
 */
void MyImage::draw(sf::RenderTarget &target, sf::RenderStates states)const {

    float x = 0, y = 0;
    int type = this->size.x > this->size.y? LANDSCAPE: PORTRAIT;
    float step=1;
    float startX=this->position.x, startY=this->position.y;
    float dimW = this->targetSize.x;
    float dimH = this->targetSize.y;


    if (this->size.x < this->targetSize.x && this->size.y < this->targetSize.y) {

        dimW = this->size.x;
        startY += (this->targetSize.y - this->size.y)/2.0f;
        dimH = this->size.y;
        startX += (this->targetSize.x- this->size.x)/2.0f;;
    }
    else {
        if (this->size.x / this->targetSize.x > this->size.y / this->targetSize.y) {
            step = this->size.x / this->targetSize.x;
            dimH = this->size.y/step;
            startY += (this->targetSize.y - dimH)/2;;
        }
        else {
            step = this->size.y / (float)this->targetSize.y;
            dimW = this->size.x/step;
            startX +=  (this->targetSize.x - dimW)/2;
        }
    }
    float xStep = 0, yStep=0;
    for (float i = 0; i < dimH; ++i) {
        for (float j = 0; j < dimW; ++j)
        {
            x = (int)xStep;
            y = (int)yStep;
            int pix = (y*this->size.x) + x;
            sf::VertexArray circle(sf::PrimitiveType::Points, 1);
            circle[0].color = sf::Color(this->pixels[pix].r,this->pixels[pix].g,this->pixels[pix].b);
            circle[0].position = {j+startX,i+startY};

            target.draw(circle);
            xStep+=step;
        }
        xStep = 0;
        yStep+=step;
    }
}

/**
* The draw function will scale the image to the maximum defined by this function
*
 * @param target : the target size of the image when displayed.
 */
void MyImage::setTargetSize(sf::Vector2f target) {
    this->targetSize = target;
}

/**
 *
 * @return The size of the original image defined in the PPM file.
 */
sf::Vector2f MyImage::getSize() {
    return this->size;
}

/**
 * This function opens the given ppm file and attempts to read in the data to a continuous vector of pixels defined as
 * RGB structs.
 *
 * @param filename the file to load.
 */
void MyImage::load(string filename) {
    ifstream ifs;

    string dummy;
    ifs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
    try {
        if (ifs.fail()) { throw("Can't open input file"); }
        std::string header;
        int w, h, b;
        // Reads in the first string to check it starts with "P6" defining a ppm file. If not throws an exception as file
        // can't be read.
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw("Can't read input file");
        getline(ifs, dummy);
        /* Checks to see if there is a comment after the first line */
        char c;
        c = ifs.peek();
        if (c == '#') {
            getline(ifs, dummy);
        }
        // Read in the width and height of the image.
        ifs >> w >> h >> b;
        // set the size within the image object
        this->size = {(float)w,(float)h};
        ifs.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data
        unsigned char pix[3];
        this->pixels.clear();// remove any existing pixel data
        // read each pixel one by one and convert bytes to floats
        for (int i = 0; i < w * h; ++i) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            RGB pixel;
            pixel.r = pix[0];
            pixel.g = pix[1];
            pixel.b = pix[2];
            this->pixels.push_back(pixel);
        }

        ifs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ifs.close();
    }
}
void MyImage::save()
{
    string filename = "Output/SavedImage.ppm";
    if (this->size.x == 0 || this->size.y == 0) { fprintf(stderr, "Can't save an empty image\n"); return; }
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
        if (ofs.fail()) throw("Can't open output file");
        ofs << "P6\n" << this->size.x << " " <<  this->size.y << "\n255\n";
        unsigned char r, g, b;
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i <  this->size.x *  this->size.y; ++i) {
            r = static_cast<unsigned char>(this->pixels[i].r);
            g = static_cast<unsigned char>(this->pixels[i].g);
            b = static_cast<unsigned char>(this->pixels[i].b);
            ofs << r << g << b;
        }
        ofs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }
}

/**
 * Sets all channels except red to zero.
 */
void MyImage::filterRed()
{
   cout << "Filter Red" << endl;

    for (RGB & pixel : pixels) {
        pixel.g = 0;
        pixel.b = 0;
    }

}
/**
 * Sets all channels except green to zero.
 */
void MyImage::filterGreen() {
    cout << "Filter Green" << endl;

    for (RGB & pixel: pixels) {
        pixel.r = 0;
        pixel.b = 0;
    }
}
/**
 * Sets all channels except blue to zero.
 */
void MyImage::filterBlue() {
    cout << "Filter Blue" << endl;

    for (RGB & pixel: pixels) {
        pixel.r = 0;
        pixel.g = 0;
    }
}
/**
 * Makes the image greyscale.
 */
void MyImage::greyScale() {
    std::cout << "Filter Greyscale" << std::endl;

    for (RGB& p : pixels) {
        // 0.299, 0.587, 0.114 scaled by 1000
        int y = (299 * p.r + 587 * p.g + 114 * p.b + 500) / 1000; // +500 for rounding
        unsigned char pix = static_cast<unsigned char>(y);

        p.r = pix; p.g = pix; p.b = pix;
    }
}

/**
 * Mirrors the image horizontally.
 */
void MyImage::flipHorizontal() {
    std::cout << "Flip Horizontal" << std::endl;

    int w = (int)this->size.x;
    int h = (int)this->size.y;

    // mirror left <-> right within each row
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w / 2; ++x) {
            int left  = y * w + x;
            int right = y * w + (w - 1 - x);

            RGB temp = pixels[left];
            pixels[left] = pixels[right];
            pixels[right] = temp;
        }
    }
}

/**
 * Mirrors the image vertically.
 */
void MyImage::flipVertical() {
    std::cout << "Flip Vertical" << std::endl;

    int w = (int)this->size.x;
    int h = (int)this->size.y;

    // mirror top <-> bottom by swapping rows
    for (int y = 0; y < h / 2; ++y) {
        for (int x = 0; x < w; ++x) {
            int top    = y * w + x;
            int bottom = (h - 1 - y) * w + x;

            RGB temp = pixels[top];
            pixels[top] = pixels[bottom];
            pixels[bottom] = temp;
        }
    }
}
void MyImage::advancedFeature1() {
    cout << "Advanced Feature 1" << endl;
}
void MyImage::advancedFeature2() {
    cout << "Advanced Feature 2" << endl;
}
void MyImage::advancedFeature3() {
    cout << "Advanced Feature 3" << endl;
}