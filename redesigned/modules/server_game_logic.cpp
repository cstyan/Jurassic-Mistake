#include "server_game_logic.h"
#include "../units/castle.h"
#include "../units/AiController.h"

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include <errno.h>

#define NOT_FOUND 2

ServerGameLogic * gSGL;

/* Constructor
 *
 * PRE:    
 * POST:    
 * RETURNS:
 * NOTES:   Creates a thread and starts running the module */
  ServerGameLogic::ServerGameLogic()
: gameState_(LOBBY), next_unit_id_(1)
{
  PATH p;
  Point a;
  a.x = 0;
  a.y = 0;
  p.push_back(a);
  a.x = 210;
  a.y = 210;
  p.push_back(a);
  a.x = 100;
  a.y = 200;
  p.push_back(a);
  a.x = 200;
  a.y = 100;
  p.push_back(a);
  teams[0].paths.push_back(p);
  teams[1].paths.push_back(p);

#ifndef TESTCLASS
  Creep c;
  c.pPath = &teams[1].paths[0][0];
  c.attackRange = 100;
  c.attackDamage = 10;
  c.health = 100;
  c.position.x = 200;
  c.position.y = 200;
  c.moveSpeed = 1;
  teams[0].creeps.push_back(c);


  c.position.x = 250;
  c.position.y = 200;
  c.moveSpeed = 1;
  teams[1].creeps.push_back(c);
  initializeCastles();
  //initializeTeams();
#endif
}


ServerGameLogic::~ServerGameLogic()
{
}

void ServerGameLogic::initializeCastles() 
{

  int uid = next_unit_id_++;
  Point pos;
  pos.x = 0;
  pos.y = 0;

  // Team 0
  Castle castle1 = Castle(uid, pos, INIT_CASTLE_HP, INIT_CASTLE_ATKDMG, INIT_CASTLE_ATKRNG, INIT_CASTLE_ATKSPD,
      INIT_CASTLE_PERCEP, INIT_CASTLE_ATKCNT, INIT_CASTLE_WALL, 0);
  teams[0].towers.push_back(castle1);

  // Team 1
  uid = next_unit_id_++;
  pos.x = MAX_X; // TODO: MAX_X and MAX_Y will  be replaced later when we get map reading functionality working
  pos.y = MAX_Y; // TODO:
  Castle castle2 = Castle(uid, pos, INIT_CASTLE_HP, INIT_CASTLE_ATKDMG, INIT_CASTLE_ATKRNG, INIT_CASTLE_ATKSPD,
      INIT_CASTLE_PERCEP, INIT_CASTLE_ATKCNT, INIT_CASTLE_WALL, 1);
  teams[1].towers.push_back(castle2);

#ifdef TESTCLASS

  printf("Castle 2 position..x: %d y: %d\n", castle2.getPos().x, castle2.getPos().y);

  mapTeams_[0].build(teams[0]);
  mapTeams_[1].build(teams[1]);

  printf("Team 0\n");
  mapTeams_[0].printGrid();
  printf("Team 1\n");
  mapTeams_[1].printGrid();

  exit(0);
#endif
}

void ServerGameLogic::initializeCreeps()
{

  for (int team_i=0; team_i<2; team_i++)
    for (int j=0; j<INIT_NUM_CREEPS; j++) {

      int uid = next_unit_id_++;
      Point pos = Point();
      
      if (team_i == 0) {
        pos.x = 5 + j;
        pos.y = 5 + j;
      }
      else {
        pos.x = MAX_X -5 - j;
        pos.y = MAX_Y -5 - j;
      }

      int hp = INIT_CREEP_HP;
      int atkdmg = INIT_CREEP_ATKDMG;
      int atkrng = INIT_CREEP_ATKRNG;
      int atkspd = INIT_CREEP_ATKSPD;
      int percep = INIT_CREEP_PERCEP;
      int atkcnt = INIT_CREEP_ATKCNT;
      int spd = INIT_CREEP_SPD;
      Direction direct = Direction();
      Point *path= &teams[1].paths[0][0];
      int movespeed = INIT_CREEP_MOVESPEED;

      Creep creep = Creep(uid, pos, hp, atkdmg, atkrng, atkspd, percep, atkcnt, spd, direct, path, movespeed);
      teams[team_i].creeps.push_back(creep);
    }
}

void ServerGameLogic::initializeTowers()
{
  for (int team_i=0; team_i<2; team_i++)
    for (int j=0; j<INIT_NUM_TOWERS; j++) {

      int uid = next_unit_id_++;
      Point pos = Point();

      if (team_i == 0) {
        pos.x = 15 + j;
        pos.y = 15 + j;
      }
      else {
        pos.x = MAX_X -15 - j;
        pos.y = MAX_Y -15 - j;
      }

      int hp = INIT_TOWER_HP;
      int atkdmg = INIT_TOWER_ATKDMG;
      int atkrng = INIT_TOWER_ATKRNG;
      int atkspd = INIT_TOWER_ATKSPD;
      int percep = INIT_TOWER_PERCEP;
      int atkcnt = INIT_TOWER_ATKCNT;
      int wall = INIT_TOWER_WALL;

      Tower tower = Tower(uid, pos, hp, atkdmg, atkrng, atkspd, percep, atkcnt, wall);
      teams[team_i].towers.push_back(tower);
    }
}

void ServerGameLogic::initializeTeams()
{
  initializeCastles();
  initializeCreeps();
  initializeTowers();
}

/* Starts the game.
 *
 * PRE:     Game is in the lobby and players are ready.
 * POST:    Data structures and initialized and game is placed in an active state, with all appropriate modules processing.
 * RETURNS:
 * NOTES:    */
void ServerGameLogic::startGame()
{
  gSGL = this;
  setAlarm();
#ifdef TESTCLASS
  initializeTeams();
#endif
}

/* Receive and queue a create unit command from a client.
 *
 * PRE:     Game is active.
 * POST:    Command has been queued.
 * RETURNS:
 * NOTES:   No validation is performed here. */
void ServerGameLogic::receiveCreateUnitCommand(int playerId, UnitType type, Point location)
{
  CommandData newCommand;

  newCommand.cmd = Create;
  newCommand.playerID = playerId;
  newCommand.type = type;
  newCommand.location = location;

  requestedCommands.push(newCommand);
}

/* Receive and queue a move player command from a client.
 *
 * PRE:     Game is active.
 * POST:    Command has been queued.
 * RETURNS:
 * NOTES:   No validation is performed here. */
void ServerGameLogic::receiveMovePlayerCommand(int playerId, Direction direction)
{
  CommandData newCommand;

  newCommand.cmd = MovePlayer;
  newCommand.playerID = playerId;
  newCommand.direction = direction;

  requestedCommands.push(newCommand);
}

void ServerGameLogic::receiveMoveUnitCommand(int unitId, Direction direction)
{
  CommandData newCommand;

  newCommand.cmd = MoveUnit;
  newCommand.unitID = unitId;
  newCommand.direction = direction;

  requestedCommands.push(newCommand);
}

/* Receive and queue an attack command from a client.
 *
 * PRE:     Game is active.
 * POST:    Command has been queued.
 * RETURNS:
 * NOTES:   No validation is performed here. */
void ServerGameLogic::receiveAttackCommand(int playerId, Direction direction)
{
  CommandData newCommand;

  newCommand.cmd = Attack;
  newCommand.playerID = playerId;
  newCommand.direction = direction;

  requestedCommands.push(newCommand);
}


std::vector<Unit>::iterator ServerGameLogic::findUnit(std::vector<Unit>::iterator first, std::vector<Unit>::iterator end, int playerID) {

  for (std::vector<Unit>::iterator it = first; first != end; ++it)
    if (it->id == playerID)
      return it; 

  return end;
}

/*
 * PRE:  Maps are current
 * RETURNS: 
 * 0 - Team 1
 * 1 - Team 2
 * 2 - Not Found
 */
int ServerGameLogic::WhichTeam(int id) {

  if (mapTeams_[0].units_.find(id) != mapTeams_[0].units_.end())
    return 0;

  if (mapTeams_[1].units_.find(id) != mapTeams_[1].units_.end())
    return 1;

  return 2;
}

void ServerGameLogic::updateCreate(CommandData& command)
{
  // Passed in command: PlayerId, type, location

  int team_no;

  int x = command.location.x;
  int y = command.location.y;


  if ( !(teams[0].isAlive() && teams[1].isAlive()) ) {
    fprintf(stderr, "Game is already over!! file: %s line %d\n", __FILE__, __LINE__);
    return;
  }

  if ( (team_no = WhichTeam(command.playerID) == NOT_FOUND) ) {
    fprintf(stderr, "playerID not found file: %s line %d\n", __FILE__, __LINE__);
    return;
  }

  if (mapTeams_[0].isValidPos(command.location)) {
    fprintf(stderr, "x: %d, y: %d out of range: %s line %d\n", x, y, __FILE__, __LINE__);
    return; 
  }

  if ( mapBoth_.grid_[x][y] != 0 )
    return; // position is already occupied 

  // Create Unit
  int id = next_unit_id_++;

  switch (command.type) {
    case CREEP:
      {
        Creep creep = Creep(id, command.location, INIT_CREEP_HP, INIT_CREEP_ATKDMG, INIT_CREEP_ATKRNG, INIT_CREEP_ATKSPD, 
            INIT_CREEP_PERCEP, INIT_CREEP_ATKCNT, INIT_CREEP_SPD, Direction(), NULL, INIT_CREEP_MOVESPEED);
        teams[team_no].creeps.push_back(creep);
        break;
      }
    case TOWER:
      {
        Tower tower = Tower(id, command.location, INIT_TOWER_HP, INIT_TOWER_ATKDMG, INIT_TOWER_ATKRNG, 
            INIT_TOWER_ATKSPD, INIT_TOWER_PERCEP, INIT_TOWER_ATKCNT, INIT_TOWER_WALL);
        teams[team_no].towers.push_back(tower);
        break;
      }
    default:
      fprintf(stderr, "Unknown type %s line:%d\n", __FILE__, __LINE__);
      return;
  }

  // Update the our map 
  Location location;
  location.pos  = command.location;
  location.type = command.type;
  mapTeams_[team_no].units_[id] = location;
  mapTeams_[team_no].grid_[x][y] = id;
}

void ServerGameLogic::updateAttack(CommandData& command)
{
  // Passed in command: playerID and direction
  
  int team_no;

  if ( !(teams[0].isAlive() && teams[1].isAlive()) ) {
    fprintf(stderr, "Game is already over!! file: %s line %d\n", __FILE__, __LINE__);
    return;
  }

  if ( (team_no = WhichTeam(command.playerID) == NOT_FOUND) ) {
    fprintf(stderr, "playerID not found file: %s line %d\n", __FILE__, __LINE__);
    return;
  }

  // Attack!!
}

void ServerGameLogic::updateMovePlayer(CommandData& command)
{
  if ( !(teams[0].isAlive() && teams[1].isAlive()) ) {
    fprintf(stderr, "Game is already over!! file: %s line %d\n", __FILE__, __LINE__);
    return;
  }
}

void ServerGameLogic::updateMoveUnit(CommandData& command)
{
  if ( !(teams[0].isAlive() && teams[1].isAlive()) ) {
    fprintf(stderr, "Game is already over!! file: %s line %d\n", __FILE__, __LINE__);
    return;
  }
}

/* Processes all waiting commands.
 *
 * PRE:    
 * POST:    Command queue is cleared.
 * RETURNS:
 * NOTES:   Perform validation here.
 *          Nice to have: send a fail message if command is invalid */
void ServerGameLogic::update()
{      

  mapTeams_[0].build(teams[0]);
  mapTeams_[1].build(teams[1]);
  mapBoth_.merge(mapTeams_[0], mapTeams_[1]);

  //mapBoth_.printGrid();

  if (requestedCommands.empty())
    return;

  // Take snap shot of queue at time0
  // Only process the number of commands that were there at time0
  int size_at_time0 = requestedCommands.size(); // <---- this is time0

  for (int i=0; i<size_at_time0; i++) {

    CommandData newCommand = requestedCommands.front();
    requestedCommands.pop();

    switch (newCommand.cmd) {
      case Create:
        updateCreate(newCommand);
        break;
      case Attack:
        //updateAttack(newCommand);
        break;
      case MovePlayer:
        updateMovePlayer(newCommand);
        break;
      case MoveUnit:
        updateMoveUnit(newCommand);
        break;
    }
  }
}

void ServerGameLogic::updateClients(int i)
{
  signal(SIGALRM, updateClients);

  //std::cout << "Update" <<std::endl;

#ifndef TESTCLASS
  AiUpdate(gSGL->teams[0], gSGL->teams[1]);
#endif

  gSGL->update();

  // Call network update function
  ServerGameLogic::setAlarm();
}

void ServerGameLogic::setAlarm()
{

  struct itimerval tout_val;
  int result;

  tout_val.it_interval.tv_sec = 0;
  tout_val.it_interval.tv_usec = 0;
  tout_val.it_value.tv_sec = 0;
  tout_val.it_value.tv_usec = INTERVAL; /* set time for interval (1/30th of a second) */
  result = setitimer(ITIMER_REAL, &tout_val,0);

  if (result != 0)
    fprintf(stderr, "Error calling setitimer error %d in %s line %d\n", errno, __FILE__, __LINE__);

  signal(SIGALRM, updateClients); /* set the Alarm signal capture */

}


// To test this class use  g++ -DTESTCLASS -g -Wall server_game_logic.cpp ../build/units/*.o
#ifdef TESTCLASS
int main() {

  ServerGameLogic game;

  game.startGame();

  while (1)
    ;

}
#endif

