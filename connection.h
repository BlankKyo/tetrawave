#ifndef GAME_CONNECTION_H
#define GAME_CONNECTION_H

#include <bits/stdc++.h>
#include <thread>
#include <SFML/Network.hpp>

using namespace std;

void server_connect(bool& connections_established, bool& flag);
void client_connect(const string& server_ip, int id, vector<sf::TcpListener>& listener_client, sf::TcpSocket& server, vector<sf::TcpSocket>& server_as_client);
void server_send(string& mat, vector<queue<string>>& q, bool& running);
void server_receive(int id, vector<queue<string>>& q, bool& running);
void client_send(string& mat, sf::TcpSocket& server, bool& running);
void client_receive(int id, std::reference_wrapper<vector<sf::TcpSocket>> server_as_client, string& startmsg, bool& started, vector<queue<string>>& q, bool& running);
void server_generate(string pieces);
void server_start(bool& started);
bool is_connected(int id);
void disconnect_all();

#endif //GAME_CONNECTION_H