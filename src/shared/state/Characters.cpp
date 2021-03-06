#include <string>
#include <iostream>

#include "define.hpp"
#include "Characters.h"

using namespace std;
using namespace state;

Characters::~Characters()
{
	cout << "Characters: " << id << " deleted" << endl;
}

/* Create a character of id=id with a defined owner, stats depending on the ID and position to (0,0) */
Characters::Characters(Player* player, CharactersID id): id(id), stats(this, id), position(this), owner(player)
{
	std::vector<std::vector<unsigned int>> matrix;
	for(unsigned int i = 0; i < 501; i++)
		matrix.push_back(std::vector<unsigned int>(501,5));

	Attacks attack1(501,501,100,1,matrix);
	attacks.push_back(attack1);
	for(unsigned int i = 0; i < 501; i++)
		matrix.push_back(std::vector<unsigned int>(501,25));

	Attacks attack2(501,501,100,2,matrix);
	attacks.push_back(attack2);
	for(unsigned int i = 0; i < 501; i++)
		matrix.push_back(std::vector<unsigned int>(501,2));

	Attacks attack3(501,501,100,3,matrix);
	attacks.push_back(attack3);
	for(unsigned int i = 0; i < 501; i++)
		matrix.push_back(std::vector<unsigned int>(501,18));

	Attacks attack4(501,501,100,4,matrix);
	attacks.push_back(attack4);
	for(unsigned int i = 0; i < 501; i++)
		matrix.push_back(std::vector<unsigned int>(501,9));

	Attacks attack5(501,501,100,5,matrix);
	attacks.push_back(attack5);

	cout << "Characters: " << id << " created" << endl;

	//Statistics my_stat(20, 21, 22);
	//stats.set_statistics(my_stat);
	//cout << "\tstats: life_point = " << stats().get_attack_point() << endl;
	//cout << "\tstats: attack_point = " << stats.get_attack_point() << endl;
	//cout << "\tstats: move_point = " << stats.get_move_point() << endl;
}

CharactersID Characters::get_id() const{ return id; }

Attacks& Characters::get_attack(unsigned int i) { return attacks[i]; }

unsigned int Characters::get_number_of_attacks(){
	return(attacks.size());
}

void Characters::addAttack (Attacks& attaque){
	if(attacks.size() < MAX_NB_ATTACK)
	{
		attacks.push_back(attaque);
	}
	return;
}

void Characters::reset_stats()
{
	stats.reset_all_but_life(id);
}

Player* Characters::get_Player() { return owner; }
