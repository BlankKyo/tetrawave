#include <bits/stdc++.h>
#include <raylib.h>

///////  Button Class  ///////


class button
{
private:
// button parameters

    // image scaling parameters
    float size_width;
    float size_height;

    // image size after scaling
    float width;
    float height;

    // button image
    Image image;
    Texture2D texture;

    // button sound effect
    Sound click_button;

public:
    // button action
    bool action;

public:
    // constructor
    button(Image photo, float number, float numbers);

    // destructor
    ~button();

    // Draw function with parameter x which indicates where to draw the button in the window
    void Draw(Vector2 x);

};