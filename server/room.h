#pragma once

#include "client.h"

#include <memory>
#include <set>
#include <string>
#include <vector>

class Room {
public:
    Room(std::string _room_name, std::shared_ptr<Client> _host) : room_name(_room_name), host(_host) {}

    std::string room_name;
    std::shared_ptr<Client> host;
    ClientSet client_set;
};

using RoomSet = std::set<std::shared_ptr<Room>>;