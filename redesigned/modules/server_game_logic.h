#ifndef SERVERGAMELOGIC_H
#define SERVERGAMELOGIC_H

#define INTERVAL 33333

#define INIT_NUM_CREEPS 3
#define INIT_TOWERS_START 4
#define INIT_NUM_TOWERS 4
#define INIT_CURRENCY 100
#define INIT_HEALTH 100

#define INIT_CASTLE_HP 70
#define INIT_CASTLE_ATKDMG 5
#define INIT_CASTLE_ATKRNG 7
#define INIT_CASTLE_ATKSPD 1
#define INIT_CASTLE_PERCEP 1
#define INIT_CASTLE_ATKCNT 1
#define INIT_CASTLE_WALL 2

#define INIT_CREEP_HP 100
#define INIT_CREEP_ATKDMG 5
#define INIT_CREEP_ATKRNG 7
#define INIT_CREEP_ATKSPD 1
#define INIT_CREEP_PERCEP 1
#define INIT_CREEP_ATKCNT 1
#define INIT_CREEP_SPD 1
#define INIT_CREEP_MOVESPEED 1

#define INIT_TOWER_HP 200
#define INIT_TOWER_ATKDMG 7
#define INIT_TOWER_ATKRNG 11
#define INIT_TOWER_ATKSPD 2
#define INIT_TOWER_PERCEP 23
#define INIT_TOWER_ATKCNT 1
#define INIT_TOWER_WALL 2 

// This will come actually be read from the map
#define MAX_X 1000
#define MAX_Y 1000

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../resource.h"

#include <string>
#include <queue>
#include <map>
#include "../team.h"
#include "../units/unit.h"
//#include "ai.h"

enum Command { Create, MovePlayer, MoveUnit, Attack };

struct CommandData {
  Command cmd;
  int playerID;
  int unitID;
  UnitType type;
  Point location;
  Direction direction;
};

class ServerGameLogic
{

  public:
    // Constructors
    ServerGameLogic();

    ~ServerGameLogic();

    // Fields
    Team teams[2];

    std::queue<CommandData> requestedCommands;

    // Functions
    void startGame();
    void receiveCreateUnitCommand(int playerId, UnitType type, Point location);
    void receiveMovePlayerCommand(int playerId, Direction direction);
    void receiveMoveUnitCommand(int unitId, Direction direction);
    void receiveAttackCommand(int playerId, Direction direction);
    void playerConnect();

  private:
    // Fields
    GameState gameState_;
    int next_unit_id_;
    //Ai ai_;

    struct Location {
      Point pos;
      UnitType type;
    };

    struct GameLogicMap {

      int max_x_;
      int max_y_;

      // Create a two dimensional grid player_ids .. 0 means the a position is empty
      int **grid_; 


      // Store locatation for each player_id
      std::map<int, Location> units_;

      GameLogicMap(int max_x = MAX_X, int max_y = MAX_Y): max_x_(max_x), max_y_(max_y) {
        grid_ = (int**) Malloc(sizeof(int*) * max_x);

        for ( int x=0; x <= max_x; x++) {
          grid_[x] = (int*) Malloc(sizeof(int) * max_y);
          memset(&grid_[x], 0, sizeof(int) * max_y);
        }
      }
      ~GameLogicMap() {
        // Free memory

        for (int x=0; x<= max_x_; x++)
          free (grid_[x]);

        free(grid_);
      }

      // This builds everything based on the Team class
      void build(Team &team) {

        reset();

        for (std::vector<Creep>::iterator it = team.creeps.begin(); it != team.creeps.end(); ++it) {
          Location location;

          location.type = CREEP;
          location.pos = it->getPos();
          units_[it->id] = location;
          grid_[location.pos.x][location.pos.y] = it->id;
        }

        for (std::vector<Tower>::iterator it = team.towers.begin(); it != team.towers.end(); ++it) {
          Location location;

          location.type = TOWER;
          location.pos = it->getPos();
          units_[it->id] = location;
          grid_[location.pos.x][location.pos.y] = it->id;
        }

        for (std::vector<Player>::iterator it = team.players.begin(); it != team.players.end(); ++it) {

          Location location;

          location.type = PLAYER;
          location.pos = it->getPos();
          units_[it->id] = location;
          grid_[location.pos.x][location.pos.y] = it->id;
        }
      }

      void reset() {
        // Zero memory
        for (int x=0; x<=max_x_; x++)
          memset(&grid_[x], 0, sizeof(int) * max_y_);
      } 

      void * Malloc(size_t size) {
        void * p;

        p = malloc(size);

        if (p == NULL)
          fprintf(stderr, "Error calling malloc in %s line %d\n", __FILE__, __LINE__);

        return p;
      }


    } MapTeam0_, MapTeam1_;

    // Functions
    void update();
    void updateCreate(CommandData& command);
    void updateAttack(CommandData& command);
    void updateMovePlayer(CommandData& command);
    void updateMoveUnit(CommandData& command);   

    // Initializing the Game functions
    void initializeTeams();
    void initializeCastles();
    void initializeCreeps();
    void initializeTowers();

    static void setAlarm();
    static void updateClients(int i);

    // teams, players, creeps, etc..
    std::vector<Unit>::iterator findUnit(std::vector<Unit>::iterator first, std::vector<Unit>::iterator end, int playerID);

    int WhichTeam(int id);
};

#endif

