#include <string>
#include <iostream>

#include "define.hpp"
#include "Player.h"

using namespace state;
using namespace std;

unsigned int Player::characters_range = MAX_NB_CHARACTER;

Player::Player() : name("Un-named Player") 
{
	//cout << "Player: " << name << " created" << " char max= " << characters_range << endl;
}

Player::Player (string name) : name(name) 
{
	cout << "Player: " << name << " created" << endl;
}

Player::~Player() 
{
	cout << "Player: " << name << " deleted" << endl;
}

/* create and add a character to the player */
shared_ptr<Characters> Player::new_character(const CharactersID id)
{
	shared_ptr<Characters> ptr = 0;
	if (get_number_of_characters() < characters_range)
	{
		ptr = make_shared<Characters>(this, id);
		characters.push_back(ptr);
		current_character = *(characters.begin());
	}
	return ptr;
}

/* get a reference to your character number i */
std::shared_ptr<Characters>& Player::get_character(unsigned int i)
{
	if (i >= get_number_of_characters())
	{
		throw std::invalid_argument("you can't access to a non existent character");
	}
	else
		return characters[i];
}

/* for GameEngine: tells which character you are using */
void Player::select_character(unsigned int character_index)
{
	if (character_index < characters.size()) //you can only select a character among those that you have
		current_character = characters[character_index];
}

unsigned int Player::get_number_of_characters() const{
	return(characters.size());
}

void Player::set_characters_range(unsigned int range)
{
	if (range < MAX_NB_CHARACTER)
		characters_range = range;
	else
		characters_range = MAX_NB_CHARACTER;
}

std::shared_ptr<Characters> Player::get_current_character(){
	return(current_character);
}

void Player::delete_character(unsigned int i){
	if(characters.size() > 1){
		characters.erase(characters.begin() + i);	
		current_character = characters[i];
	}
}


