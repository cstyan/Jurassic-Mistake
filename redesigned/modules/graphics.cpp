#include "graphics.h"
#include <iostream>

#include <unistd.h>
#include <SFML/Graphics.hpp>

using namespace std;


#define ID_START 123
#define ID_QUIT 124


/* Graphics Thread entry point
 *
 * PRE:     
 * POST:    Window closed: no more graphics thread necessary.
 * RETURNS: nothing
 * NOTES:   Graphics init and main loop  
*/
void * init (void * in) {
   Graphics* g = (Graphics *)in;
   sf::RenderWindow window(sf::VideoMode(800, 600), "Client");
   g->window = &window;

    if (!g->font.loadFromFile("/usr/share/fonts/truetype/ttf-dejavu/DejaVuSans.ttf")) {
		cerr << ("error loading font") << endl ;
	}
	
	Button a(ID_START, sf::Vector2f(250,300), sf::Vector2f(300,50), g->font, "                Start Game");
	Button b(321, 	   sf::Vector2f(250,400), sf::Vector2f(300,50), g->font, "     Press this button for fun");
	Button c(ID_QUIT,  sf::Vector2f(250,500), sf::Vector2f(300,50), g->font, "                     Quit");

	g->clientGameLogic_.UIElements.insert(a);
	g->clientGameLogic_.UIElements.insert(b);
	g->clientGameLogic_.UIElements.insert(c);

	g->map_bg.loadFromFile("images/map.png");
	g->map.setTexture(g->map_bg);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonPressed){
				sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(window));
				for (std::set<Button>::iterator button = g->clientGameLogic_.UIElements.begin(); button != g->clientGameLogic_.UIElements.end(); ++button) {
					if (button->rect.getGlobalBounds().contains(mouse)) {
						cout << "Button ID:" << button->id << endl;
						if (button->id == ID_START){
							g->clientGameLogic_.start();
							g->clientGameLogic_.UIElements.clear();
							break;
						} else if (button->id == ID_QUIT) {
							exit(0);
						}
						//AddNewCalledButton(button->id);
					}
				}
			}
		}
		window.clear();

		if (g->clientGameLogic_.getCurrentState() == LOBBY) {
			g->drawLobby(window);
		} else if (g->clientGameLogic_.getCurrentState() == IN_GAME) {
			g->drawUnits(window);
			g->drawMap(window);
			g->drawHud(window, g);
			sf::Text state("In Game", g->font, 20);
			window.draw(state);

		}

		for (std::set<Button>::iterator button = g->clientGameLogic_.UIElements.begin(); button != g->clientGameLogic_.UIElements.end(); ++button)
		{
			Button b = *button;
			b.draw(window);
		}

		window.display();
	}

	return NULL;
}

/* Constructor
 *
 * PRE:     
 * POST:    
 * RETURNS: 
 * NOTES:   Creates a thread and starts running the module */
Graphics::Graphics(ClientGameLogic& clientGameLogic)
   : clientGameLogic_(clientGameLogic), window(NULL)
{
   pthread_t t;
   pthread_create(&t, NULL, init, (void*)this);
}

/* Draws the HUD.
 *
 * PRE:     
 * POST:    Current HUD is displayed
 * RETURNS: 
 * NOTES:    */
void Graphics::drawHud(sf::RenderWindow& window, Graphics* g)
{
    Button a(999,sf::Vector2f(020,575), sf::Vector2f(100,25), g->font, "Tower1");
	Button b(998,sf::Vector2f(130,575), sf::Vector2f(100,25), g->font, "Tower2");
	Button c(997,sf::Vector2f(240,575), sf::Vector2f(100,25), g->font, "Tower3");
	Button d(996,sf::Vector2f(350,575), sf::Vector2f(100,25), g->font, "Creep1");
	Button e(995,sf::Vector2f(460,575), sf::Vector2f(100,25), g->font, "Creep2");
	Button f(994,sf::Vector2f(570,575), sf::Vector2f(100,25), g->font, "Creep3");
	Button h(ID_QUIT,sf::Vector2f(680,575), sf::Vector2f(100,25),  g->font, "Quit");
	
	
	g->clientGameLogic_.UIElements.insert(a);
	g->clientGameLogic_.UIElements.insert(b);
	g->clientGameLogic_.UIElements.insert(c);
	g->clientGameLogic_.UIElements.insert(d);
	g->clientGameLogic_.UIElements.insert(e);
	g->clientGameLogic_.UIElements.insert(f);
	g->clientGameLogic_.UIElements.insert(h);
}

/* Draws the lobby.
 *
 * PRE:     
 * POST:    Current lobby is displayed
 * RETURNS: 
 * NOTES:    */
void Graphics::drawLobby(sf::RenderWindow& window)
{
	sf::Text title("Child's Play", font, 71);
	title.setPosition(sf::Vector2f(200, 0));
	window.draw(title);
}

/* Draws all current units.
 *
 * PRE:     
 * POST:    All current units are displayed
 * RETURNS: 
 * NOTES:    */
void Graphics::drawUnits(sf::RenderWindow& window)
{

	for (std::vector<CLIENT_UNIT>::iterator unit = clientGameLogic_.units.begin(); unit != clientGameLogic_.units.end(); ++unit)
	{
		//window.draw(button->rect);
	}
}

/* Draws the map
 *
 * PRE:     
 * POST:    The map is displayed
 * RETURNS: 
 * NOTES:    */
void Graphics::drawMap(sf::RenderWindow& window)
{
	window.draw(map);
}
