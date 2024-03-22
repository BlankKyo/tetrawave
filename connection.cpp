#include "connection.h"
#include <bits/stdc++.h>
#include <thread>
#include <chrono>
#include <SFML/Network.hpp>
using namespace std;

vector<sf::TcpListener> listener(4);
vector<sf::TcpSocket> client(4);
vector<sf::TcpSocket> socket0(4), socket1(4), socket2(4);
vector<string> ip(4);
vector<bool> connected(4, false);
char buffer0[1024], buffer1[1024], buffer2[1024];

void disconnect_all(){
    connected = vector<bool>(4, false);
    client = vector<sf::TcpSocket>(4);
    socket0 = vector<sf::TcpSocket>(4);
    socket1 = vector<sf::TcpSocket>(4);
    socket2 = vector<sf::TcpSocket>(4);
    listener = vector<sf::TcpListener>(4);
    cout << "disconnect done\n";
}
bool is_connected(int id){
    return connected[id];
}
void server_connect(bool& connection_established, bool& flag){
    for (int i = 1; i < 4; i++) {
        if(listener[i].listen(1000 + i) != sf::Socket::Done) {
            cout << "Error listening to port as server" << 1000 + i << endl;
            continue;
        }
        // Set the listener to non-blocking mode
        listener[i].setBlocking(false);

        // Define the timeout duration (in milliseconds)
        int timeoutMs = 10000; // 10 seconds

        // Get the current time
        auto startTime = std::chrono::steady_clock::now();
        bool accepted = false;
        if (!flag) break;
        while(true) {
            if (!flag) break;
            // Check if the timeout has elapsed
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
            if (elapsedTime >= timeoutMs) {
                std::cout << "Timeout reached, no connection with " << i << std::endl;
                break; // Handle timeout
            }
            if (listener[i].accept(client[i]) == sf::Socket::Done) {
                cout << "Accepted client " << i << endl;
                accepted = true;
                break;
            }
        }
        if (!flag) break;
        if (!accepted) continue;
        size_t received = 0;
        if (client[i].receive(buffer0, sizeof(buffer0), received) == sf::Socket::Done) {
            ip[i] = (string)buffer0;
        }
        if (!flag) break;
        if(socket0[i].connect(ip[i], 1000 + i) != sf::Socket::Done){
            cout << "Didn't find the ip address of " << i << '\n';
        } else connected[i] = true, cout << "connected with " << i << " on " << 1000 + i << endl;
        if (!flag) break;
        if(socket1[i].connect(ip[i], 1004 + i) != sf::Socket::Done){
            cout << "Didn't find the ip address of " << i << '\n';
        } else connected[i] = true, cout << "connected with " << i << " on " << 1004 + i << endl;
        if (!flag) break;
        if(socket2[i].connect(ip[i], 1008 + i) != sf::Socket::Done){
            cout << "Didn't find the ip address of " << i << '\n';
        } else connected[i] = true, cout << "connected with " << i << " on " << 1008 + i << endl;
        if (!flag) break;
    }
    connection_established = true;
}
void client_connect(const string& server_ip, int id, vector<sf::TcpListener>& listener_client, sf::TcpSocket& server, vector<sf::TcpSocket>& server_as_client){
    //sf::TcpListener listener_client;
    if(listener_client[0].listen(1000 + id) != sf::Socket::Done) { // declare these variables in main
        cout << "Error listening to port as client";
    }
    if(listener_client[1].listen(1004 + id) != sf::Socket::Done) { // declare these variables in main
        cout << "Error listening to port as client";
    }
    if(listener_client[2].listen(1008 + id) != sf::Socket::Done) { // declare these variables in main
        cout << "Error listening to port as client";
    }
    //sf::TcpSocket server; // send to server
    if(server.connect(server_ip, 1000 + id) != sf::Socket::Done){
        cout << "Didn't find the server's ip address\n";
    }
    sf::IpAddress ipAddress = sf::IpAddress::getLocalAddress();
    string ip_id = ipAddress.toString();
    server.send(ip_id.c_str(), ip_id.size() + 1);
    //sf::TcpSocket server_as_client; // recieve from server_as_client
    if(listener_client[0].accept(server_as_client[0]) != sf::Socket::Done) {
        cout << "Error accepting server as client on " << 1000 + id << endl;
    }
    if(listener_client[1].accept(server_as_client[1]) != sf::Socket::Done) {
        cout << "Error accepting server as client on " << 1004 + id << endl;
    }
    if(listener_client[2].accept(server_as_client[2]) != sf::Socket::Done) {
        cout << "Error accepting server as client on " << 1008 + id << endl;
    }
}
void server_send(string& mat, vector<queue<string>>& q, bool& running){ // send to all connected sockets, mat: server's matrix
    vector<string> old_state(4, mat);
    while(running){
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        old_state[0] = mat;
        vector<string> state = old_state;
        for (int i = 1; i < 4; i++){
            if (q[i].empty()) continue; // if no recent change
            state[i] = q[i].front();
            old_state[i] = q[i].front();
            q[i].pop();
        }
        if (connected[1]) {
            socket0[1].send(state[0].c_str(), state[0].size());
            socket1[1].send(state[2].c_str(), state[2].size());
            socket2[1].send(state[3].c_str(), state[3].size());
        }
        if (connected[2]) {
            socket0[2].send(state[0].c_str(), state[0].size());
            socket1[2].send(state[1].c_str(), state[1].size());
            socket2[2].send(state[3].c_str(), state[3].size());
        }
        if (connected[3]) {
            socket0[3].send(state[0].c_str(), state[0].size());
            socket1[3].send(state[1].c_str(), state[1].size());
            socket2[3].send(state[2].c_str(), state[2].size());
        }
    }
}
void server_receive(int id, vector<queue<string>>& q, bool& running){ // each id will have his own thread
    while(running){
        size_t received = 0;
        if (client[id].receive(buffer0, sizeof(buffer0), received) == sf::Socket::Done) {
            string msg;
            for (int j = 0; j < 260; j++) msg += buffer0[j];
            q[id].push(msg);
            cout << "received" << ' ' << id <<'\n';
            cout << msg << '\n';
        }
    }
}
void client_send(string& mat, sf::TcpSocket& server, bool& running) {
    while(running) {
        std::this_thread::sleep_for(chrono::milliseconds(400));
        server.send(mat.c_str(), mat.size());
    }
}
void client_receive(int id, std::reference_wrapper<vector<sf::TcpSocket>> server_as_client, string& startmsg, bool& started, vector<queue<string>>& q, bool& running){
    cout << "getting string of pieces" << endl;
    while(running) {
        size_t received = 0;
        if (server_as_client.get()[0].receive(buffer0, sizeof(buffer0), received) == sf::Socket::Done) {
            startmsg = buffer0;
            cout << "string of pieces acquired" << endl;
            break;
        }
    }
    cout << "waiting for game to start" << endl;
    while(running) {
        size_t received = 0;
        if (server_as_client.get()[0].receive(buffer0, sizeof(buffer0), received) == sf::Socket::Done) {
            string msg = buffer0;
            if (msg[0] == 'y') {
                started = true;
                cout << "game started" << endl;
                break;
            }
        }
    }
    while(running){
        size_t received = 0;
        if (server_as_client.get()[0].receive(buffer0, sizeof(buffer0), received) == sf::Socket::Done){
            string msg;
            for (int j = 0; j < 260; j++) msg += buffer0[j];
            q[0].push(msg);
            cout << "received" << ' ' << 0 <<'\n';
            cout << msg << '\n';
        }
        if (server_as_client.get()[1].receive(buffer0, sizeof(buffer0), received) == sf::Socket::Done){
            string msg;
            for (int j = 0; j < 260; j++) msg += buffer0[j];
            if (id == 1) {
                q[2].push(msg);
                cout << "received " << 2 <<'\n';
                cout << msg << '\n';
            }
            if (id == 2 || id == 3) {
                q[1].push(msg);
                cout << "received " << 1 <<'\n';
                cout << msg << '\n';
            }
        }
        if (server_as_client.get()[2].receive(buffer0, sizeof(buffer0), received) == sf::Socket::Done){
            string msg;
            for (int j = 0; j < 260; j++) msg += buffer0[j];
            if (id == 1 || id == 2) {
                q[3].push(msg);
                cout << "received " << 3 <<'\n';
                cout << msg << '\n';
            }
            if (id == 3) {
                q[2].push(msg);
                cout << "received " << 2 <<'\n';
                cout << msg << '\n';
            }
        }
    }
}
void server_generate(string pieces){
    for (int i = 1; i < 4; i++){
        if (!connected[i]) continue;
        socket0[i].send(pieces.c_str(), pieces.size());
    }
}
void server_start(bool& started){
     // put it in a thread
     for (int i = 1; i < 4; i++) {
         if (!connected[i]) continue;
         if (!started) socket0[i].send("n", 1);
         else socket0[i].send("y", 1);
     }
}

