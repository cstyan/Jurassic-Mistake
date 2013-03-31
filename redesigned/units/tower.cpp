#include "tower.h"
#include "unit.h"
#include <string>

using namespace std;

//old ctor - Callum
Tower::Tower(int uid, Point pos, int hp, int atkdmg, int atkrng,
            int atkspd, int percep, int atkcnt, int wall):
            Unit(uid, pos, hp, atkdmg, atkrng, atkspd, percep, atkcnt), wallArmour_(wall)
{
    //validation
}
/*------------------------------------------------------------------------------
-- FUNCTION:    Creep - constructor
--
-- DATE:        March 26, 2013
--
-- DESIGNER:    Callum Styan
-- PROGRAMMER:  Callum Styan
--
-- INTERFACE:   Creep(int uid, int side, Point pos, Point* path, int hp = 100, int atkdmg = 10, int atkrng = 10,
                      int atkspd = 10, int percep = 10, int atkcnt = 30, int movespeed = 10,
                      Direct direct = Direction())

                uid: unit id generated by the server
                pos: starting position when you call the ctor
                path: pointer to all the points for the creeps pathing
--
-- RETURNS:     a new Creep object
--
-- DESCRIPTION: Creates a new Creep object.  Default values are given.
--              
--              The unit id must be specified, it is generated server side.
--              
--              The starting position must be specified, this should be the first value
--              of the path it is in.
--              
--              The *path must be specified, this stores the points for the lane that the
--              creep will move through.
------------------------------------------------------------------------------*/

Tower::Tower(int uid, int side, Point pos, int hp, int atkdmg, int atkrng,
            int atkspd, int percep, int atkcnt, int wall):
            Unit(uid, side, pos, hp, atkdmg, atkrng, atkspd, percep, atkcnt), wallArmour_(wall)
{
    //validation
}

/*------------------------------------------------------------------------------
-- FUNCTION:    serializeCreep
--
-- DATE:        2013/03/11
--
-- DESIGNER:    Behnam Bastami, Aaron Lee
-- PROGRAMMER:  Behnam Bastami, Aaron Lee, David Czech
--
-- INTERFACE:   Unit::serializeUnit()
--
-- RETURNS:     string, the serialized Unit
--
-- DESCRIPTION: Converts the Tower classes members into one string to be sent over
-- the network between client and server.  Uses the unit serialize function.
------------------------------------------------------------------------------*/
string Tower::serializeTower(){
    string s;
    s = Unit::serializeUnit();

    tower_t t;
    t.wallArmour = wallArmour_;
    
    s += string((const char*)&t, sizeof(t));
    
    return s;
}
