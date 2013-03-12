#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include <iostream>
#include "../resource.h"
#include "../types.h"


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


class ClientNetwork
{
public:
   // Constructors
   ClientNetwork();
   ~ClientNetwork();

   // Functions
   bool createUnit(int playerId, UnitType type, Point location);
   bool movePlayer(int playerId, Direction direction);
   bool attack(int playerId, Direction direction);

   int sendRequest(int msg);
   bool connectToServer(std::string hostname, int port);



private:
   int connectsock;
};

#endif
