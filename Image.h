#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <cstring>

using namespace std;



#ifndef IMAGEPROJECT_IMAGE_H
#define IMAGEPROJECT_IMAGE_H
struct RGB {
    unsigned char r, g, b;
    RGB(unsigned char r, unsigned char g, unsigned char b):r(r), g(g), b(b){}
    RGB():r(0),g(0),b(0){}
};

class MyImage : public sf::Drawable
{
    vector<RGB> pixels;
    sf::Vector2f size;
    sf::Vector2f targetSize;;
    const int PORTRAIT = 1;
    const int LANDSCAPE = 2;
    sf::Vector2f position;
    string fileName ;

    void load(string filename);
public:
    MyImage(string filename);
    void draw(sf::RenderTarget &target, sf::RenderStates states)const;
    void  setTargetSize(sf::Vector2f target);
    void setPosition(sf::Vector2f pos){this->position = pos;}
    sf::Vector2f getSize();
    void save();
    friend ostream& operator<<(ostream &os, MyImage &img) {
        return os << img.fileName << " " << img.size.x <<" " << img.size.y<<endl;;

    }
    // functions to implement
    void flipHorizontal();
    void flipVertical();
    void advancedFeature1();

    /**
     * @brief Uses mouse input for creating a rectangle to crop a part of the image.
     * @param window Pass reference of sf::RenderWindow
     */
    void advancedFeature2(sf::RenderWindow& window);

    /**
     * @brief Increases or decreases brightness.
     * @param value int value for adjusting brightness. Use Positive for adding. Negative for subtracting.
     * Default is set to 10.
     */
    void advancedFeature3(int value = 15);
    void reload();
    void filterRed();
    void filterGreen();
    void filterBlue();
    void greyScale();
};


#endif //IMAGEPROJECT_IMAGE_H