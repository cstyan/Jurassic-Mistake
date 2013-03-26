/*------------------------------------------------------------------------------
-- FILE:        electro_tower.cpp
--
-- DATE:        2013/03/11
--
-- MAINTAINERS: Nick Raposo
--
-- FUNCTIONS:   Attack
--              Update          
--
-- DESCRIPTION: Base function for electric attacks to target multiple units.
------------------------------------------------------------------------------*/
#include "electro_tower.h"
#include "unit.h"
#include <string>
#include "../team.h"

using namespace std;

ElectroTower::ElectroTower(int uid, Point pos, int hp, int atkdmg, int atkrng,
            int atkspd, int percep, int atkcnt, int wall):
            Tower(uid, pos, hp, atkdmg, atkrng, atkspd, percep, atkcnt, wall)
{
    //validation
}

void ElectroTower::Attack(Team* team)
{
   if( attackCount++ < attackSpeed )
        return;

    Unit* temp[4];
    Unit * tempTarget;

    attackCount = 0;
    
    pTarget->health -= attackDamage;
    temp[0] = pTarget;
    tempTarget = pTarget;

    for(int j = 1; j < 4; j++)
    {
        for (size_t i = 0; i < team->creeps.size(); ++i)
        {

            if( inRange( tempTarget->getPos(), team->creeps[i].getPos(), attackRange) && team->creeps[i].health > 0) 
            {
                if(tempTarget->id == temp[0]->id || tempTarget->id == temp[1]->id || tempTarget->id == temp[2]->id || tempTarget->id == temp[3]->id)
                {
                    continue;
                }
                else
                {
                    tempTarget = (Unit*)&team->creeps[i];
                    temp[j] = tempTarget;
                    tempTarget->health -= attackDamage;
                    break;
                }       
            }
        }
    }
}

void ElectroTower::Update(Team& team) {

    /* If we have a Target, check their status. */
    if( pTarget != NULL )
        CheckTarget();

    /* Search for Target. */
    if( pTarget == NULL )
        FindTarget(&team);

    /* If we found a new Target, and they are in range.. */
    if( pTarget != NULL ) {
        Attack(&team);
        Rotate( pTarget->getPos() );
    }
}
