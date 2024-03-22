#include "Background.h"

#include <utility>

using namespace std;

background::background(Texture2D Texture, float rotations, int vitesse, float number, float numbers, float px, float py, string mesg): texture(Texture), rotation(rotations), speed(vitesse), size_width(number), size_height(numbers), posx(px), posy(py), msg(std::move(mesg))
{initialize();}

background::background(Texture2D Texture, float rotations, int vitesse, float number, float numbers, float px, float py): texture(Texture), rotation(rotations), speed(vitesse), size_width(number), size_height(numbers), posx(px), posy(py)
{
    initialize();
}
void background::initialize()
{
    // texture size
    width = texture.width;
    height = texture.height;

    // variables responsible for animating the image
    source1 = (Rectangle) {0,0, (float) width, (float) height };
    source2 = (Rectangle) {0,0, 0,(float) height};
    dest1 = (Rectangle) {posx ,posy , source1.width * size_width , source1.height * size_height };
    dest2 = (Rectangle) {posx ,posy , source2.width * size_width , source2.height * size_height };

    // loading music if msg not empty
    if (!msg.empty()) {
        music = LoadMusicStream(msg.c_str());
        PlayMusicStream(music);
    }
}
background::~background()
{
    // unload music
    UnloadMusicStream(music);
}


void background::Draw()
{
    // Drawing block
    if (speed == 0){
        // Drawing a fixed background
        DrawTexturePro(texture, source1, dest1, Vector2{dest1.width/2,  dest1.height/2}, rotation, RAYWHITE);
    }
    else{
        // Drawing a moving background
        DrawTexturePro(texture, source2, dest2, Vector2{dest2.x, dest2.y}, rotation, RAYWHITE);
        DrawTexturePro(texture, source1, dest1, Vector2{dest1.x, dest1.y}, rotation, RAYWHITE);
    }

    // block responsible for animating the background
    if (speed) {
        // each time we crop the image into two parts source1 and source2
        // while dest1 and dest2 are the destination were we are going to
        // draw source1 and source2
        source2.width += (float) speed;
        dest2.width = source2.width * size_width;
        source1.x += (float) speed;
        dest1.x = source1.x * size_height;
        if (source2.width >= (float) width) {
            source2.width = 0;
            dest2.width = 0;
            source1.x = 0;
            dest1.x = 0;
        }
    }
}


