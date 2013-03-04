#include "Player.h"

Player::Player(const int& teamID, const int& playerID, const int& hp, const int& xPos, const int& yPos, const int& type):
 MobileUnit(teamID, hp, xPos, yPos){
    //init some things?
 }

int Player::getID(){
    return playerID_
}

int Player::getType(){
    return playerType_;
}

void Player::setType(const int& type){
    playerType_ = type;
}

void Player::Run(){
    //code to execute movement, attacks, etc.
}
