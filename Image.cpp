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

/**
 * Mirror half of the image.
 */
void MyImage::advancedFeature1() {
    cout << "Advanced Feature 1 - Mirror Image" << endl;
    int w = (int)this->size.x;
    int h = (int)this->size.y;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w / 2; ++x) {
            int top    = y * w + x;
            int right = y * w + (w - 1 - x);

            RGB temp = pixels[top];
            pixels[right] = temp;
        }
    }
}
void MyImage::advancedFeature2(sf::RenderWindow &window) {
    std::cout << "Advanced Feature 2 - Cropping Image" << std::endl;

    // These flags control the cropping interaction logic
    bool dragging = false;
    bool wasDown = false;
    bool readyToCrop = false;
    bool done = false;

    // Stores the position where dragging starts and the current drag position
    sf::Vector2f dragStart;
    sf::Vector2f dragNow;

    // Rectangle used to visually show the selected crop region
    sf::RectangleShape rect;
    rect.setFillColor(sf::Color(80, 160, 255, 60));
    rect.setOutlineColor(sf::Color(80, 160, 255, 220));
    rect.setOutlineThickness(2.f);

    while (!done) {

        // https://www.sfml-dev.org/documentation/3.0.2/namespacesf_1_1Mouse.html
        bool leftDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

        // Get mouse position relative to the window
        sf::Vector2i mp = sf::Mouse::getPosition(window);

        // Convert mouse position to floating point vector
        sf::Vector2f mousePos((float)mp.x, (float)mp.y);

        // Prevent cropping from starting if the mouse button was already held
        // when this function started
        if (!readyToCrop) {
            if (!leftDown) readyToCrop = true;

            // Store mouse state for next frame
            wasDown = leftDown;

            // Draw the image normally while waiting
            window.clear();
            window.draw(*this);
            window.display();
            continue;
        }

        // Detect a fresh mouse press (button down now but not in previous frame)
        if (leftDown && !wasDown) {
            dragging = true;

            // Record the start position of the drag
            dragStart = mousePos;

            // Initialize current drag position
            dragNow = mousePos;
        }

        // While dragging and mouse still held down
        if (leftDown && dragging) {
            // Update current drag position
            dragNow = mousePos;

            // https://en.cppreference.com/w/cpp/algorithm/min.html
            // Determine the top-left corner of the rectangle
            // using min so dragging works in any direction
            sf::Vector2f topLeft(
                std::min(dragStart.x, dragNow.x),
                std::min(dragStart.y, dragNow.y)
            );

            // https://en.cppreference.com/w/cpp/numeric/math/abs
            // Calculate width and height of the selection rectangle
            sf::Vector2f size(
                std::abs(dragNow.x - dragStart.x),
                std::abs(dragNow.y - dragStart.y)
            );

            // Update rectangle position and size
            rect.setPosition(topLeft);
            rect.setSize(size);
        }

        // Detect mouse release (button up now but was down last frame)
        if (!leftDown && wasDown && dragging) {
            dragging = false;

            // Get crop rectangle boundaries in screen coordinates
            float screenX1 = rect.getPosition().x;
            float screenY1 = rect.getPosition().y;
            float screenX2 = screenX1 + rect.getSize().x;
            float screenY2 = screenY1 + rect.getSize().y;

            // Get the image drawing position and size on screen
            float drawLeft   = this->position.x;
            float drawTop    = this->position.y;
            float drawWidth  = this->targetSize.x;
            float drawHeight = this->targetSize.y;

            // Convert screen coordinates into normalized ratios
            // relative to where the image is drawn
            float rx1 = (screenX1 - drawLeft) / drawWidth;
            float ry1 = (screenY1 - drawTop)  / drawHeight;
            float rx2 = (screenX2 - drawLeft) / drawWidth;
            float ry2 = (screenY2 - drawTop)  / drawHeight;

            // Convert normalized ratios into actual image pixel coordinates
            int imgX1 = (int)(rx1 * this->size.x);
            int imgY1 = (int)(ry1 * this->size.y);
            int imgX2 = (int)(rx2 * this->size.x);
            int imgY2 = (int)(ry2 * this->size.y);

            // Clamp coordinates so they stay inside image boundaries
            imgX1 = std::max(0, std::min((int)this->size.x, imgX1));
            imgY1 = std::max(0, std::min((int)this->size.y, imgY1));
            imgX2 = std::max(0, std::min((int)this->size.x, imgX2));
            imgY2 = std::max(0, std::min((int)this->size.y, imgY2));

            // Ensure coordinates are ordered correctly
            if (imgX2 < imgX1) std::swap(imgX1, imgX2);
            if (imgY2 < imgY1) std::swap(imgY1, imgY2);

            // Calculate new cropped image dimensions
            int newW = imgX2 - imgX1;
            int newH = imgY2 - imgY1;

            // If crop area is invalid, stop cropping
            if (newW <= 0 || newH <= 0) {
                std::cout << "Invalid crop area.\n";
                done = true;
                continue;
            }

            // Create a new pixel vector for the cropped image
            std::vector<RGB> newPixels;
            newPixels.reserve(newW * newH);

            // Store the original image width
            int oldW = (int)this->size.x;

            // Copy pixels from the selected region
            for (int y = 0; y < newH; ++y) {
                for (int x = 0; x < newW; ++x) {

                    // Convert 2D coordinate into 1D index
                    int oldIndex = (imgY1 + y) * oldW + (imgX1 + x);

                    // Add the pixel to the new image
                    newPixels.push_back(this->pixels[oldIndex]);
                }
            }

            // Replace old pixel data with the cropped pixel data
            this->pixels = newPixels;

            // Update image dimensions
            this->size.x = (float)newW;
            this->size.y = (float)newH;

            std::cout << "Cropped image to: " << newW << " x " << newH << std::endl;
            std::cout << "Cropping done." << std::endl;

            // End cropping loop
            done = true;
        }

        // Update previous mouse state
        wasDown = leftDown;

        // Render the image
        window.clear();
        window.draw(*this);

        // Draw the crop selection rectangle while dragging
        if (dragging)
            window.draw(rect);

        // Display the frame
        window.display();
    }
}
void MyImage::advancedFeature3(int value) {
    cout << "Advanced Feature 3 - Brightness Change" << endl;

    int percent = (value * 100) / 255; // integer percent
    cout << "Brightness change: " << percent << "% (change " << value << ")\n";

    for (RGB& p : pixels) {
        int r = p.r + value;
        int g = p.g + value;
        int b = p.b + value;

        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;

        if (r < 0) r = 0;
        if (g < 0) g = 0;
        if (b < 0) b = 0;

        p.r = r;
        p.g = g;
        p.b = b;
    }
}

void MyImage::advancedFeature4() {
    cout << "Advanced Feature 4 - Grid" << endl;
    int w = (int)this->size.x;
    int h = (int)this->size.y;

    for (int y = 0; y < h / 2; ++y) {
        for (int x = 0; x < w / 2; ++x) {
            pixels[y * w + x].r = 0;
            pixels[y * w + x].g = 0;
        }
    }
    for (int y = 0; y < h / 2; ++y) {
        for (int x = w / 2; x < w; ++x) {
            pixels[y * w + x].b = 0;
            pixels[y * w + x].r = 0;
        }
    }
    for (int y = h / 2; y < h; ++y) {
        for (int x = 0; x < w / 2; ++x) {
            pixels[y * w + x].g = 0;
            pixels[y * w + x].b = 0;
        }
    }
    /*for (RGB & pixel : pixels) {
        unsigned char P = (pixel.r + pixel.g + pixel.b) / 3;
        pixel.r = P;
        pixel.g = P;
        pixel.b = P;
    }*/
    for (int y = h / 2; y < h; ++y) {
        for (int x = w / 2; x < w; ++x) {
            unsigned char P = (pixels[y * w + x].r + pixels[y * w + x].g + pixels[y * w + x].b) / 3;
            pixels[y * w + x].r = P;
            pixels[y * w + x].g = P;
            pixels[y * w + x].b = P;
        }
    }
}