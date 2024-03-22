#include <bits/stdc++.h>
#include <raylib.h>
#include "Menu/Background.h"
#include "game.h"
#include "colors.h"
#include "Button/Buttons.h"
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "connection.h"
#include <thread>
#include "robot.hpp"

using namespace std;

#define  ll  long long
#define  pb   push_back
#define  all(v) v.begin(),v.end()
#define mk make_pair
#define ls first
#define rs second
#define FAST ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);

bool connection_established = false;
bool run = true;
bool paused = false;
bool running = false;
int nbrows = 26;
int nbcols = 10;
int cell_size = 30;
int screen_width = 1200;
int screen_height = 1020;
int fps = 60;
int nbplayer = 0;
float timer = 0;
float timeOver = 0;
string state = "Menu";
string test;
vector<queue<string>> q(4);
vector<string> states(4);

double lastUpdateTime = 0;
double interval = 0.4;

void update_grid(const std::string& mat, Grid &matrix) {
    int n = (int) matrix.grid.size(), m = (int)matrix.grid[0].size(), i = 0, j = 0;
    vector<vector<int>> res(n, (vector<int>(m, 0)));
    for (auto c: mat){
        res[i][j] = c - '0';
        j++;
        if (j == m) i++, j = 0;
    }
    matrix.grid = res;
}

bool wait(double duration){
    double currentTime = GetTime();
    if (currentTime - timeOver >= duration){
        timeOver = currentTime;
        return true;
    }
    return false;
}
bool EventTriggered(double intervals){
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= intervals){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}
void type(string &ans){
    int key = GetCharPressed();
    if (key > 0) test += (char)key;
    // Check for backspace
    if (IsKeyPressed(KEY_BACKSPACE)) {
        if (!test.empty()) {
            reverse(all(test));
            test.erase(test.begin());
            reverse(all(test));
        }
    }
    else if (IsKeyPressed(KEY_ENTER)){
        ans = test;
        test = "";
    }
}
void draw_text(const string &ans, int posx, int posy, const string &msg, int size) {
    char input[256], statement[256];
    int i = 0;
    for(auto it:ans){
        input[i] = it;
        i++;
    }
    input[i] = '\0';
    i = 0;
    for(auto it:msg){
        statement[i] = it;
        i++;
    }
    statement[i] = '\0';
    DrawText(statement, posx, posy, size, LIGHTGRAY);
    DrawText(input, posx + 50, posy + 50, size, BLACK);
    DrawText("", 40 + MeasureText( input, 20), posy, size, BLACK); // Draw a cursor simulation

}
int main()
{
    bool started = false, once = false, started_comms_client = false, started_comms_server= false, once1 = false, once2 = false, once3 = false;
    bool flag_connect = false, twice = false;
    string mat, bot1_mat, bot2_mat, bot3_mat;
    int client_id = 1;
    string ip_server;
    // initialize Window and AudioDevice
    InitWindow(screen_width, screen_height, "raylib Tetris");
    SetTargetFPS(fps);
    Font font = LoadFontEx("Font/monogram.ttf", 64, nullptr, 0);
    InitAudioDevice();

    // initialize game
    Bot bot1 = Bot(nbrows, nbcols, cell_size, font, 25,5,3,10,5,14);
    Bot bot2 = Bot(nbrows, nbcols, cell_size, font, 25,3,10,5,5,30);
    Bot bot3 = Bot(nbrows, nbcols, cell_size, font,1,1,1,1,1,4);
    Game singleplayer = Game(nbrows, nbcols, cell_size, font);
    Game multiplayer = Game(nbrows, nbcols, cell_size, font);
    bot1.seq_block = multiplayer.seq_block;
    bot2.seq_block = multiplayer.seq_block;
    bot3.seq_block = multiplayer.seq_block;
    vector<Grid> players;
    for(int i = 0; i < 4; i++) players.emplace_back(nbrows, nbcols, 20);
    for(int i = 0; i < 4; i++) players[i].Initialize(), states[i] = multiplayer.getgrid();
    // load images
    Texture2D image1 = LoadTexture("Image/Game_background.png");
    Texture2D image2 = LoadTexture("Image/Back1.png");
    Texture2D image3 = LoadTexture("Image/tetris.png");
    Texture2D image4 = LoadTexture("Image/icon1.png");
    Texture2D image5 = LoadTexture("Image/icon11.png");
    Texture2D image6 = LoadTexture("Image/icon12.png");
    Texture2D image7 = LoadTexture("Image/icon2.png");
    Texture2D image8 = LoadTexture("Image/icon21.png");
    Texture2D image9 = LoadTexture("Image/icon22.png");
    Texture2D image10 = LoadTexture("Image/icon23.png");
    Texture2D image11 = LoadTexture("Image/icon13.png");
//////////////////////////////////////////////////////////////////////////////////////////
    Image photo1 = LoadImage("Image/button1.png");
    Image photo2 = LoadImage("Image/button2.png");
    Image photo3 = LoadImage("Image/button3.png");
    Image photo4 = LoadImage("Image/button4.png");
    Image photo5 = LoadImage("Image/button5.png");
    Image photo6 = LoadImage("Image/button6.png");


    // create backgrounds
    background page(image1, 0, 1, 2, 2.5, 0, 0,"Sounds/boku no hero academia S2 OST.mp3");
    background pag(image3, 0, 0, 0.75, 0.75, 600, 250);
    background pages(image2, 0, -1, 3.6, 3.6, 0, 0,"Sounds/kei theme Kung fu panda.mp3");
    background icon1(image4, 0, 0, 1.3, 1.3, 100, 350);
    background icon2(image5, 0, 0, 1.3, 1.3, 601, 570);
    background icon3(image6, 0, 0, 1.3, 1.3, 601, 350);
    background icon4(image7, 0, 0, 1, 1, 99, 273);
    background icon5(image8, 0, 0, 1, 1, 600, 493);
    background icon6(image9, 0, 0, 1, 1, 600, 273);
    background icon7(image11, 0, 0, 1.3, 1.3, 100, 570);
    background icon8(image10, 0, 0, 1, 1, 99, 493);


    // create buttons
    button but1(photo1, 0.65, 0.65);
    button but2(photo2, 0.65, 0.65);
    button but3(photo3, 0.65, 0.65);
    button but4(photo4, 0.65, 0.65);
    button but5(photo5, 0.4, 0.4);
    button but6(photo6, 0.65, 0.65);

    cout << singleplayer.generate() << '\n';
    vector<sf::TcpListener> listener_client(3);
    sf::TcpSocket server;
    vector<sf::TcpSocket> server_as_client(3);
    // loop for running the game
    while (true) {
        if (state == "Menu"){
            for (int i = 0; i < 3; i++) listener_client[i].close(), server_as_client[i].disconnect();
            server.disconnect();
            bot1.seq_block = multiplayer.seq_block;
            bot2.seq_block = multiplayer.seq_block;
            bot3.seq_block = multiplayer.seq_block;
            // Menu page
            UpdateMusicStream(pages.music);
            BeginDrawing();

            // background
            pages.Draw();
            pag.Draw();

            // buttons
            but1.Draw({450, 420});
            DrawText(TextFormat("SinglePlayer"), 475, 440, 40, BLUE);
            but2.Draw({450, 515});
            DrawText(TextFormat("Host game"), 500, 535, 40, BLUE);
            but3.Draw({450, 610});
            DrawText(TextFormat("Join game"), 510, 630, 40, BLUE);
            but4.Draw({450, 705});
            DrawText(TextFormat("Exit"), 560, 730, 40, BLUE);

            // function of each button
            if (but1.action){
                state = "singleplayer";
                ClearBackground(WHITE);
                pages.initialize();
            }
            else if (but2.action){
                state = "host";
                client_id = 0;
                running = true;
                ClearBackground(WHITE);
            }
            else if (but3.action){
                state = "join";
                running = true;
                ClearBackground(WHITE);
            }
            if (but4.action){
                CloseWindow();
                CloseAudioDevice();
                break;
            }
            EndDrawing();
        }
        else if (state == "singleplayer"){
            BeginDrawing();
            if (singleplayer.gameOver || paused){
                if (singleplayer.gameOver) DrawText(TextFormat("Game Over!"), 300, 270, 40, RED);
                DrawRectangle(0,0,screen_width, screen_height, lightestblue);
                but1.Draw({250, 320});
                DrawText(TextFormat("Continue"), 315, 340, 40, BLUE);
                but2.Draw({250, 423});
                DrawText(TextFormat("Return"), 330, 445, 40, BLUE);
                if (but1.action){
                    if (singleplayer.gameOver){
                        interval = 0.4;
                        singleplayer.initialize();
                        page.initialize();
                        ClearBackground(WHITE);
                    }
                    else paused = false;
                }
                else if (but2.action){
                    state = "Menu";
                    interval = 0.4;
                    singleplayer.initialize();
                    page.initialize();
                    ClearBackground(WHITE);
                    paused = false;
                }
            }
            else {
                UpdateMusicStream(page.music);
                singleplayer.HandleInput(timer, interval, EventTriggered(interval));
                page.Draw();
                icon4.Draw();
                icon1.Draw();
                icon5.Draw();
                icon2.Draw();
                icon6.Draw();
                icon3.Draw();
                icon8.Draw();
                icon7.Draw();
                but5.Draw({508, 700});
                DrawText(TextFormat("Pause"), 562, 712, 28, BLUE);
                if (but5.action) paused = true;
                singleplayer.Draw(200, 170);
            }
            EndDrawing();
        }
        else if (state == "join"){
            if (!connection_established) {
                UpdateMusicStream(pages.music);
                BeginDrawing();
                pages.Draw();
                DrawRectangle(350, 330, 500, 200, lightBlue);
                DrawRectangle(450, 428, 280, 45, RAYWHITE);
                but6.Draw({450, 540});
                DrawText(TextFormat("Return"), 530, 560, 40, BLUE);
                if (!ip_server.empty()) {
                    draw_text(ip_server, 430, 380, "Type IP server :", 40);
                    // change client_id above before running the game
                    thread connect_the_client(client_connect, ip_server, client_id, std::ref(listener_client), std::ref(server), std::ref(server_as_client));
                    connect_the_client.detach();
                    connection_established = true;
                    continue;
                }
                else {
                    type(ip_server);
                    draw_text(test, 430, 380, "Type IP server :", 40);
                }
                if (but6.action) {
                    test = "";
                    ip_server = "";
                    state = "Menu";
                    running = false;
                    ClearBackground(WHITE);
                }
                EndDrawing();
            } else {
                if (!started) {
                    // draw waiting page
                    UpdateMusicStream(pages.music);
                    BeginDrawing();
                    pages.Draw();
                    DrawRectangle(360, 410, 480, 90, RAYWHITE);
                    DrawText(TextFormat("Waiting for host !!!"), 370, 430, 50, BLACK);
                    but3.Draw({450, 500});
                    DrawText(TextFormat("Return"), 530, 530, 40, BLUE);
                    if (!once) {
                        thread ClientReceive(client_receive, client_id, ref(server_as_client), ref(multiplayer.seq_block), ref(started), ref(q), ref(running));
                        ClientReceive.detach();
                        once = true;
                    }
                    if (but3.action){
                        once = false;
                        connection_established = false;
                        test = "";
                        ip_server = "";
                        running = false;
                        state = "Menu";
                    }
                    EndDrawing();
                }
                else { // draw multiplayer page
                    multiplayer.multiplayer_init();
                    mat = multiplayer.getgrid();
                    thread ClientSend(client_send, ref(mat), ref(server), ref(running));
                    ClientSend.detach();
                    state = "multiplayer";
                }
            }
        }
        else if (state == "host"){ // host
            mat = multiplayer.getgrid();
            if (!started) {
                flag_connect = true;
                UpdateMusicStream(pages.music);
                BeginDrawing();
                pages.Draw();
                but4.Draw({450, 300});
                DrawText(TextFormat("Search"), 525, 321, 40, BLUE);
                but1.Draw({450, 400});
                DrawText(TextFormat("Send pieces"), 480, 420, 40, BLUE);
                but2.Draw({450, 500});
                DrawText(TextFormat("Start"), 540, 525, 40, BLUE);
                but3.Draw({450, 600});
                DrawText(TextFormat("Return"), 530, 625, 40, BLUE);
                if (but4.action && !once1) { // attempt connection only once
                    thread ServerConnect(server_connect, ref(connection_established), ref(flag_connect));
                    ServerConnect.detach();
                    once1 = true;
                }
                if (connection_established && but1.action) { // but1: send button
                    server_generate(multiplayer.seq_block);
                    connection_established = false; // flag once3
                    once1 = false;
                    twice = true;
                }
                if (but2.action && twice) {
                    started = true;
                    server_start(started);
                    bot1.started = true;
                    bot2.started = true;
                    bot3.started = true;
                }
                if (but3.action) {
                    once1 = false;
                    flag_connect = false;
                    started = false;
                    bot1.initialize();
                    bot2.initialize();
                    bot3.initialize();
                    started_comms_server = false;
                    running = false;
                    client_id = 1;
                    disconnect_all();
                    state = "Menu";
                }
                if (once1 && !connection_established){
                    DrawRectangle(290, 200, 640, 90, RAYWHITE);
                    DrawText(TextFormat("Searching for players !!!"), 300, 220, 50, BLACK);
                }
                if (connection_established){
                    DrawRectangle(320, 200, 590, 90, RAYWHITE);
                    DrawText(TextFormat("You can send pieces !!!"), 330, 220, 50, BLACK);
                }
                if (twice){
                    DrawRectangle(390, 200, 430, 90, RAYWHITE);
                    DrawText(TextFormat("You can start !!!"), 400, 220, 50, BLACK);
                }
                EndDrawing();
            }
            if(started && wait(2)) {
                if (!started_comms_server){
                    thread ServerSend(server_send, ref(mat), ref(q), ref(running));
                    if (is_connected(1)) {
                        thread ServerReceive1(server_receive, 1, ref(q), ref(running));
                        ServerReceive1.detach();
                    }
                    if (is_connected(2)) {
                        thread ServerReceive2(server_receive, 2, ref(q), ref(running));
                        ServerReceive2.detach();
                    }
                    if (is_connected(3)) {
                        thread ServerReceive3(server_receive, 3, ref(q), ref(running));
                        ServerReceive3.detach();
                    }
                    ServerSend.detach();
                    started_comms_server = true;
                }
                state = "multiplayer";
            }
        }
        else{
            mat = multiplayer.getgrid();
            if (client_id == 0 && wait(0.4)) { // host
                bot1_mat = bot1.getgrid();
                if (!is_connected(2)) {
                    q[2].push(bot1_mat);
                    cout << 1 << ' ' << bot1.lvl << ' ' << bot1.score << '\n';
                }
                bot2_mat = bot2.getgrid();
                if (!is_connected(1)) {
                    q[1].push(bot2_mat);
                    cout << 2 << ' ' << bot2.lvl << ' ' << bot2.score << '\n';
                }
                bot3_mat = bot3.getgrid();
                if (!is_connected(3)) {
                    q[3].push(bot3_mat);
                    cout << 3 << ' ' << bot3.lvl << ' ' << bot3.score << '\n';
                }
            }
            BeginDrawing();
            UpdateMusicStream(page.music);
            multiplayer.HandleInput(timer, interval, EventTriggered(interval));
            page.Draw();
            for(int i = 0; i < 4; i++){
                if (client_id != i){
                    if (q[i].empty()) continue; // if no recent change
                    states[i] = q[i].front();
                    update_grid(states[i], players[i]);
                    if (client_id != 0) q[i].pop();
                }
            }
            for(int i = 0; i < 4; i++){
                if (i != client_id){
                    if (nbplayer == 0) players[i].Draw(730,   80);
                    if (nbplayer == 1) players[i].Draw(980,   80);
                    if (nbplayer == 2) players[i].Draw(855,   525);
                    nbplayer++;
                }
            }
            nbplayer = 0;
            icon4.Draw();
            icon1.Draw();
            icon5.Draw();
            icon2.Draw();
            icon6.Draw();
            icon3.Draw();
            icon8.Draw();
            icon7.Draw();
            multiplayer.Draw(200, 170);
            if (multiplayer.gameOver ) {
                DrawText(TextFormat("Game Over!"), 250, 780, 40, RED);
                DrawRectangle(0, 0, 700, screen_height, lightestblue);
                but2.Draw({220, 830});
                DrawText(TextFormat("Return"), 290, 850, 40, BLUE);
                if (but2.action) {
                    state = "Menu";
                    interval = 0.4;
                    once1 = false;
                    flag_connect = false;
                    started = false;
                    bot1.initialize();
                    bot2.initialize();
                    bot3.initialize();
                    started_comms_server = false;
                    connection_established = false;
                    started_comms_client = false;
                    running = false;
                    once = false;
                    once2 = false;
                    twice = false;
                    test = "";
                    ip_server = "";
                    disconnect_all();
                    multiplayer.initialize();
                    page.initialize();
                    pages.initialize();
                    client_id = 1;
                    ClearBackground(WHITE);
                }
            }
            EndDrawing();
        }
    }

}