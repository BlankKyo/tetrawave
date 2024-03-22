#include <bits/stdc++.h>
#include <raylib.h>

using namespace std;

///////  Background Class  ///////


class background
{
private:
// background parameters

    // background image
    Texture2D texture;

    // image angle
    float rotation;

    // image scaling parameters
    float size_width;
    float size_height;

    // image size
    int width{};
    int height{};


    // position of the background in the window
    float posx;
    float posy;

    // sound path
    string msg;

    // variables responsible for animating the image
    Rectangle source1{};
    Rectangle source2{};
    Rectangle dest1{};
    Rectangle dest2{};

public:
    // Background music
    Music music{};

public:
    // constructor
    background(Texture2D Texture, float rotations, int vitesse, float number, float numbers, float px, float py, string mesg);
    background(Texture2D Texture, float rotations, int vitesse, float number, float numbers, float px, float py);

    // destructor
    ~background();

    // initialise background function
    void initialize();

    // Draw function
    void Draw();

    // moving speed of the image
    int speed;
};
