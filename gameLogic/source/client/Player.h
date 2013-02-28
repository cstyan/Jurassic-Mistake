/*--------------------------------------------------------------------------------------
--	SOURCE FILE:	Player.h -	Header file for the Player class
--
--	PROGRAM:		
--
--	FUNCTIONS:
--
--	DATE:		Feburary 28, 2013
--
--	DESIGNER:	Steve Lo
--
--	PROGRAMMER:	Steve Lo
--
--	NOTES:
--------------------------------------------------------------------------------------*/
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>

class Player {
private:
	long _id;
	int team;	//which team, attack/defend
	int _hp;
	string _class;
	Point _currentCoordinates;
	int _attackPoint;
	int _defendPoint;
	//perhaps some other status to the character
	int _movementSpeed;

	int _status;	//ready or not, which stage is he/she on...
	//etc...
  
public:
	long getID();
	void setID(long);
	int getHP();
	void setHP(int);
	string getClass();
	void setClass(string);
	Point getCoord();
	void setCoord(Point);
	int getAtkPoint();
	void setAtkPoint(int);
	int getDefPoint();
	void setDefPoint(int);
	int getMoveSpeed();
	void setMoveSpeed(int);
	int getStatus();
	void setStatus(int);
};
#endif
