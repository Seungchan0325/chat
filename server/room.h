#pragma once

#include "client.h"

#include <memory>
#include <set>
#include <string>

class Room {
public:
    std::string room_name;
    std::shared_ptr<Client> host;
    ClientSet client_set;
};

using RoomSet = std::set<std::shared_ptr<Room>>;