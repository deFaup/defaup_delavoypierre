/* Includes */
#include <iostream>
#include "define.hpp"
#include "Scene.h"

using namespace std;
using namespace state;
using namespace render;

sf::View window_view;
sf::Vector2f position_in_map[2]; //[1] is the position with mouse coordinates

// layer info temp
sf::String info; sf::Text text; sf::Font font;

Scene::~Scene(){ std::cout << "Scene deleted" << std::endl; }

/* Scene constructor share its reference to the GameState */
Scene::Scene(sf::RenderWindow& window, GameState& gameState) 
	: renderWindow(window), gameState(gameState), background(gameState), characters(gameState)
{
	std::cout << "Scene created" << std::endl;
	init_window(window_view, renderWindow, position_in_map);
}


/* draw our surfaces with the draw method of SFML (called from draw method in Surface) */
void Scene::draw()
{
	//const sf::Vector2f init_position = position_in_map[0];
	//update_view(window_view, renderWindow, position_in_map, init_position);

	background.setSurface(renderWindow); // first the background	
	characters.setSurface(renderWindow); // on top of the background the characters

	/* Layer info temp */
	Statistics& stats = gameState.current_player->get_current_character()->stats;

	info = "Current Player: " + gameState.current_player->name + "\n\n";
	info += "Current Character Statistics:\nlife point : ";
	info += std::to_string(stats.get_life_point());
	info += "\nmove point : ";
	info += std::to_string(stats.get_move_point());
	info += "\nattack point : ";
	info += std::to_string(stats.get_attack_point());

	// choix de la cha�ne de caract�res � afficher
	text.setString(info);

	// choix de la couleur du texte
	//text.setFillColor(sf::Color::Black);

	// choix du style du texte
	text.setStyle(sf::Text::Bold /*| sf::Text::Underlined*/);

	// puis, dans la boucle de dessin, entre window.clear() et window.display()
	renderWindow.draw(text);
}

/* config the view position for the beginning */
void Scene::init_window(sf::View& window_view, sf::RenderWindow& renderWindow, sf::Vector2f (&position_in_map) [2])
{
	int width(0), height(0);
	state::Map::get_screen_dimensions(width, height);
	width -= 10;	height -= 150;
	renderWindow.create(sf::VideoMode(width, height), "SFML game", sf::Style::Titlebar | sf::Style::Close);
	renderWindow.setFramerateLimit(20);

//	window_view.setSize(width, height);
	window_view.setSize(3000, 2000);
	window_view.setCenter(1500, 1000);

	// set the first position in the game: temporary
/*	position_in_map[0].x = position_in_map[1].x = width/2 ;
	position_in_map[0].y = position_in_map[1].y = height/2;
	sf::Mouse::setPosition((sf::Vector2i)position_in_map[0]);

	// set the view center so that the top left corner of the map is the top left corner of the window
	window_view.setCenter(position_in_map[0]);

	//position_in_map[0].x = 0; 	position_in_map[0].y = 0;
*/
	renderWindow.setView(window_view);
	renderWindow.setPosition(sf::Vector2i(0, 0));

	/* Layer info temp */
	// choix de la police � utiliser
	font.loadFromFile("res/arial.ttf");
	text.setFont(font); // font est un sf::Font

	// choix de la taille des caract�res
	text.setCharacterSize(50); // exprim�e en pixels, pas en points !
}

/* If we hit the border of the window then we move the view accordingly */
void Scene::update_view(sf::View& window_view, sf::RenderWindow& renderWindow, sf::Vector2f(&position_in_map)[2], sf::Vector2f init_position)
{
	unsigned int x = sf::Mouse::getPosition().x;
	unsigned int y = sf::Mouse::getPosition().y;
	position_in_map[1].x = position_in_map[0].x + x - init_position.x;
	position_in_map[1].y = position_in_map[0].y + y - init_position.y;

	//temporary
	int map_size[2] = { 0 };
	gameState.map.get_dimensions(map_size[0], map_size[1]);

	if (x >= (unsigned int)renderWindow.getPosition().x + renderWindow.getSize().x -100)
		position_in_map[0].x += 10;

	if (x <= (unsigned int)renderWindow.getPosition().x + 100) {
		std::cout << "going left1 " << std::endl;
		position_in_map[0].x -= 10;
	}
		
	if (y >= (unsigned int)renderWindow.getPosition().y + renderWindow.getSize().y - 100) {
		position_in_map[0].y += 10;
	}
		
	if (y <= (unsigned int)renderWindow.getPosition().y + 100) {
		cout << "going up1 " << endl;
		position_in_map[0].y -= 10;
	}
		

	if (position_in_map[1].x >= map_size[0]) {
		cout << "going left2 " << endl;
		position_in_map[0].x -= 10;
	}
		
	else if (position_in_map[1].x <= 0)
		position_in_map[0].x += 10;

	if (position_in_map[1].y >= map_size[1]) {
		cout << "going up2 " << endl;
		position_in_map[0].y -= 10;
	}
		
	else if (position_in_map[1].y <= 0)
		position_in_map[0].y += 10;

	renderWindow.setView(window_view);
	window_view.setCenter(position_in_map[0]);
	cout << x << " " << y << endl;
	cout << position_in_map[1].x << " " << position_in_map[1].y << endl;
	cout << position_in_map[0].x << " " << position_in_map[0].y << endl << endl;

}

void Scene::stateChanged(state::Event& event)
{
	if (event.hasChanged(state::EventID::Map_maskChanged))
		background.update();
	
	else if (event.hasChanged(state::EventID::Character_positionChanged))
	{
		EventCharacters* event_character = (EventCharacters*) &event;

		if (event_character->changed_character) // if ptr to character is not null
			characters.update_one_character(event_character->changed_character);
		else
			characters.update();
	}

	else if (event.hasChanged(state::EventID::Character_isDead))
	{
		EventCharacters* event_character = (EventCharacters*) &event;
		Player* player = event_character->changed_character->get_Player();

		//std::cout << "Scene::stateChanged Player: " << player->name << " character: ";
		//std::cout << event_character->changed_character->get_id() << " is dead !" << std::endl;

		unsigned int index(0);
		/* find the shared ptr to the dead character in the player characters vector */
		for (unsigned int i = 0; i < player->get_number_of_characters(); i++)
		{
			if ((*player).get_character(i).get() == event_character->changed_character)
			{
				index = i;
				break;
			}
		}
		// in this part we delete all shared ptr to the dead character
		//shared_ptr<state::Characters> temp = player->get_character(index);
		//std::cout << "references to this character= " << temp.use_count() << endl; //7 or 6 (5 if 1 in Attack)
		
		// delete in Player (current_character and vector characters)
		player->delete_character(index);
		//std::cout << "references to this character= " << temp.use_count() << endl; //5

		// delete in GameState (vector characters)
		gameState.delete_character(event_character->changed_character);
		//std::cout << "references to this character= " << temp.use_count() << endl; //4

		// delete in render::Character (vector characters)
		characters.delete_character(event_character->changed_character);
		//std::cout << "references to this character= " << temp.use_count() << endl; //3

		//std::cout << "Scene::stateChanged character: END\n";

	}//2 once we exit this namespace and 0 when we leave engine::Attack::execute (or only 1 in Attack)

	else if (event.hasChanged(state::EventID::Player_isDead))
	{
		EventPlayer* event_player = (EventPlayer*) &event;
		Player* player = event_player->changed_player;
		//std::cout << "Scene::stateChanged  Player: " << player->name << " is dead !" << std::endl;

		//unsigned int index(0);
		//find the shared ptr to the dead character in the player characters vector
		//shared_ptr<state::Player> temp;
		//for (unsigned int i = 0; i < gameState.get_number_of_player(); i++)
		//{
		//	if (gameState.get_player(i).get() == player)
		//	{
		//		index = i;
		//		temp = gameState.get_player(index);
		//		break;
		//	}
		//}
		//std::cout << "references to this character= " << temp.use_count() << endl; //2 or 3

		// in this part we delete all shared ptr to the dead player
		gameState.delete_player(player);
		//std::cout << "Scene::stateChanged Player: END\n";
	}
}
