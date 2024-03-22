#include "Buttons.h"


using namespace std;


button::button(Image photo, float number, float numbers): image(photo), size_width(number), size_height(numbers)
{
    // rescaling image and loading it as Texture2D type in texture variable
    ImageResize(&image, (int) ((float) image.width * size_width), (int)((float) image.height * size_height));
    texture = LoadTextureFromImage(image);

    // texture size
    width = (float) texture.width;
    height = (float) texture.height;

    // initialise action as false (button not clicked yet)
    action = false;

    // loading sound
    click_button = LoadSound("Sounds/click.mp3");
}


button::~button()
{
    // unload image and sound
    UnloadImage(image);
    UnloadSound(click_button);
}

void button::Draw(Vector2 x)
{
    // initialise action as false mousePoint as the position of the cursor
    action = false;
    Vector2 mousePoint = GetMousePosition();

    // Check button state
    Rectangle btnBounds = { x.x, x.y,  width, height};
    if (CheckCollisionPointRec(mousePoint, btnBounds)){
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) action = true;
    }

    // perform action
    if (action) {
        PlaySound(click_button);
        cout << "clicked\n";
    }

    // Draw button
    Rectangle sourceRec = { 0, 0, width, height };
    DrawTextureRec(texture, sourceRec, (Vector2){ x.x, x.y }, WHITE); // Draw button frame

}
