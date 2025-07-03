#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<cstdlib>
#include<string>
#include<cmath>
#include<ctime>

using namespace std;

float gamespeed = 0.75;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void movePlayer(int& dirPlayer, float player[], int mushroom[][gameColumns], sf::Texture death, sf::Sprite& playerSprite, sf::RenderWindow& window,float centipede[][6],sf::Sound deathsound, const int c_size);
void drawMushroom(int mushroom[][gameColumns], sf::RenderWindow& window, sf::Sprite mushroomSprite[][gameColumns]);
void checkBulletMushroomCollision(int mushroom[][gameColumns], float bullet[][3], float centipede[][6], const int c_size, int cdir, int& size, int maxBullets);
void moveCentipede(float centipede[][6], sf::Clock centipedeClock[],sf::Sprite centipedeSprite[], int a, const int cdir, int mushroom[][gameColumns], bool& centipededead, const int c_size, int speed);
void checkCentipedeBulletCollision(float centipede[][6], float bullet[][3], const int c_size, int mushroom[][gameColumns], int head, int& score, int level, int maxBullets, sf::Sound kill);
bool checkCentipedeMushroomCollison(float centipede[][6], const int c_size, int mushroom[][gameColumns], int cdir,int a, int b);
void generateCentipede(float centipede[][6], int cdir, const int c_size, int head, sf::Texture cHeadLeftTexture, sf::Texture cBodyTexture, sf::Sprite centipedeSprite[], bool& test, bool& startcomplete, int speed, int& centipedeSpeed);
void drawCentipede(const int c_size, float centipede[][6],sf::Sprite centipedeSprite[],int cdir,int head,sf::Texture cHeadLeftTexture,sf::Texture cHeadRightTexture, sf::Texture cBodyTexture, sf::RenderWindow& window);
void centipedeSegment(float centipede[][6], int cdir, int head, int speed, const int c_size, sf::Clock& segmentclock, sf::Sprite centipedeSprite[],sf::Texture cHeadLeftTexture,sf::Texture cHeadRightTexture, int centipedeSpeed );
void centipedePlayerCollision(float centipede[][6], float player[], const int c_size, sf::Texture death, sf::Sprite& playerSprite, sf::RenderWindow& window, sf::Sound deathsound);
void killplayer(float player[], sf::Texture death, sf::Sprite& playerSprite, sf::RenderWindow& window, float centipede[][6],sf::Sound deathsound, const int c_size);
bool mainMenu(sf::RenderWindow& window, sf::Text& title, sf::Text& start, sf::Text& exit, bool& select, sf::Sprite backgroundSprite);
bool DeadMenu(sf::RenderWindow& window,sf::Sprite backgroundSprite, int score, sf::Font font);
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void moveBullet(float bullet[][3], sf::Clock bulletClock[], int a);
void drawBullet(sf::RenderWindow& window, float bullet[][3], sf::Sprite bulletSprite[], int a);





int main()
{
	srand(time(0));

	// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	//window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(250, 0));

	// Initializing Background Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	bgMusic.play();
	bgMusic.setVolume(100);
	
	// Initializing Background.
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/Stars.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 225* 0.20)); // Reduces Opacity to 25%

	// Initializing Player and Player Sprites.
	float player[3] = {};
	player[x] = (gameColumns / 2) * boxPixelsX;
	player[y] = (gameColumns * 28 / 30) * boxPixelsY;
	player[exists]=1;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	playerTexture.loadFromFile("Textures/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Bullet and Bullet Sprites.
	const int maxBullets=30;
	float bullet[maxBullets][3] = {{0,0,0}};
	sf::Clock bulletClock[maxBullets];
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite[maxBullets];
	bulletTexture.loadFromFile("Textures/bullet.png");
	for(int a=0; a<maxBullets; a++)
	{
		bulletSprite[a].setTexture(bulletTexture);
		bulletSprite[a].setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	}
	
	//This clock is to give space between each bullet fired
	sf::Clock spaceclock;
	
	//Initializing mushrooms and mushroom sprites
	int mushroom[gameRows][gameColumns]={0};
	int numberOfMushrooms=rand()%11+20;
	sf::Texture mushroomTexture;
	sf::Sprite mushroomSprite[gameRows][gameColumns];
	mushroomTexture.loadFromFile("Textures/mushroom.png");
	for(int a=0; a<numberOfMushrooms; a++)
	{
		int col=rand()%gameColumns;
		int row=rand()%(gameRows-2)+1;
		if(row!=gameRows-1 && (row!=28 && col!=15))
		{
			mushroom[row][col]=2;	//set random 20-30 mushroom lives to 2
		}
		else
			a--;
	}
	for(int a=0; a<gameColumns; a++)
	{
		mushroom[0][a]=-2;		//setting up invisible mushrooms in first row so centipede can spawn and descend
	}
	for(int a=0; a<gameRows; a++)
	{
		for(int b=0; b<gameColumns; b++)
		{
			mushroomSprite[a][b].setTexture(mushroomTexture);
			mushroomSprite[a][b].setTextureRect(sf::IntRect(0,0,boxPixelsX,boxPixelsY));
		}
	}
	
	//Initializing centipede and centipede sprites and clocks
	const int cdir=3, head=4, speed=5, c_size=30;
	float centipede[c_size][6]={};
	int centipedeSpeed=2;
	int level=0;
	bool test=1, startcomplete=0;
	sf::Texture cBodyTexture;
	cBodyTexture.loadFromFile("Textures/c_body_left_walk.png");
	sf::Texture cHeadLeftTexture;
	cHeadLeftTexture.loadFromFile("Textures/c_head_left_walk.png");
	sf::Texture cHeadRightTexture;
	cHeadRightTexture.loadFromFile("Textures/c_head_right_walk.png");
	sf::Sprite centipedeSprite[c_size];
	sf::Clock centipedeClock[c_size];
	sf::Clock segmentclock;
	
	//Initializing Player death texture
	sf::Texture death;
	death.loadFromFile("Textures/explosion.png");
	
	//Initializing score text
	int score=0;
	sf::Font font;
	font.loadFromFile("Fonts/Minecrafter.Reg.ttf");
	sf::Text scoreT("", font, 35);
	scoreT.setPosition(resolutionX-8*boxPixelsX,-5);
	scoreT.setFillColor(sf::Color::White);
	
	//Initializing sound effects
	 sf::SoundBuffer firebuffer;
  	 firebuffer.loadFromFile("Sound Effects/spacefire.wav");
   	 sf::Sound fire;
   	 fire.setBuffer(firebuffer);
   	 fire.setVolume(20);
   	 
   	 sf::SoundBuffer deathbuffer;
  	 deathbuffer.loadFromFile("Sound Effects/death.wav");
   	 sf::Sound deathsound;
   	 deathsound.setBuffer(deathbuffer);
   	 deathsound.setVolume(50);
   	 
   	 sf::SoundBuffer killbuffer;
   	 killbuffer.loadFromFile("Sound Effects/kill.wav");
   	 sf::Sound kill;
   	 kill.setBuffer(killbuffer);
   	 kill.setVolume(50);
	
	//Initializing main menu objects
	sf::Text title("Centipede", font, 70);
	title.setPosition(280,100);
	sf::Text start("Start", font, 50);
	start.setPosition(300,400);
	start.setFillColor(sf::Color::Yellow);
	sf::Text exit("Exit", font, 50);
	exit.setPosition(300,600);
	bool select=1,gamestarting=1;
	
	
	
	
	int startCol;// = rand() % gameColumns - 2;
	
	
	
		
	while(window.isOpen()) {
		
		
		///////////////////////////////////////////////////////////////
		//                                                           //
		// Call Your Functions Here. Some have been written for you. //
		// Be vary of the order you call them, SFML draws in order.  //
		//                                                           //
		///////////////////////////////////////////////////////////////
		
		
			
		
		
		//Main menu
		if(gamestarting)
		{
			bool command=mainMenu(window, title, start, exit, select, backgroundSprite);
			gamestarting=0;
			if(command==0) //exit game
				return 0;
		}
		
		//end game
		if(player[exists]==0)
			return DeadMenu(window,backgroundSprite, score, font);
		
		//drawing basic play area elements(background and score)
		window.draw(backgroundSprite);
		scoreT.setString("Score: " + to_string(score));
		window.draw(scoreT);
		
		/////////////////////////generate new centipede//////////////////////
		bool complete=1;
		
		if(test)
		{
			if(startcomplete==0)
			{
				//random staring position for main centipede
				do
				{
					startCol=rand()%gameColumns-2;	
				}
				while(startCol<=0);
				centipede[0][x]=startCol*boxPixelsX;
				centipede[0][y]=0;
				centipede[0][exists]=true;
				centipede[0][cdir]=1;
				centipede[0][head]=true;
				centipede[0][speed]=centipedeSpeed;
				centipedeSprite[0].setTexture(cHeadLeftTexture);
				centipedeSprite[0].setTextureRect(sf::IntRect(0,0,28,boxPixelsY));
				
				//generating seperate segment heads in later levels
				if(level<=12)
				{
					for(int b=12-level; b<12; b++)	
					{
						
							centipede[b][x]=(rand()%gameColumns)*boxPixelsX;
							centipede[b][y]=0; 
							centipede[b][exists]=1;
							centipede[b][head]=1;
							centipede[b][cdir]=-1;
							centipede[b][speed]=2*centipedeSpeed;
							centipedeSprite[b].setTexture(cHeadRightTexture);
							centipedeSprite[b].setTextureRect(sf::IntRect(0,0,28,boxPixelsY));
							
						
					}
				}
			}
			
			
			//generating the rest of the main centipede
			static int a=1;
			for(; a<12-level; a++)	
			{
				if(centipede[a-1][x]==(startCol-1)*boxPixelsX || centipede[a-1][y]==boxPixelsY || centipede[a-1][exists]==0)
				{
					centipede[a][x]=startCol*boxPixelsX;
					centipede[a][y]=0;
					centipede[a][exists]=true;
					centipede[a][cdir]=1;
					centipede[a][speed]=centipedeSpeed;
					centipedeSprite[a].setTexture(cBodyTexture);
					centipedeSprite[a].setTextureRect(sf::IntRect(28,0,28,boxPixelsY));
						
				}
				else
				{
					complete=0;
					startcomplete=1;
					break;
				}
			} 
			
			//stop generating when complete
			if(complete)
			{
				
				test=0;
				a=1;
				
			}
		}
		
		
		
		/////////////new centipede when level is cleared////////////////
		bool centipededead=1;
		for(int a=0; a<c_size; a++)
		{
			if(centipede[a][exists])
				centipededead=0;
		}
		
		if(centipededead)
		{
			//reset bool variables that are needed to start new generation
			test=1;
			startcomplete=0;
			level++;
			
			//increase speed on alternate levels
			if(centipedeSpeed<16 && level%2==0)
				centipedeSpeed*=2;
			
			//reset all centipede parameters and change centipede color
			for(int a=0; a<c_size; a++)
			{
				centipede[a][0]=0;
				centipede[a][1]=0;
				centipede[a][2]=0;
				centipede[a][3]=0;
				centipede[a][4]=0;
				centipede[a][5]=0;
				if(level%3==0)
					centipedeSprite[a].setColor(sf::Color::Green);
				if(level%3==1)
					centipedeSprite[a].setColor(sf::Color::Cyan);
				if(level%3==2)
					centipedeSprite[a].setColor(sf::Color::Magenta);
				
			
			}
			//change mushroom color
			for(int a=0; a<gameRows; a++)
			{
				for(int b=0; b<gameColumns; b++)
				{
					if(level%3==0)
						mushroomSprite[a][b].setColor(sf::Color::White);
					if(level%3==1)
						mushroomSprite[a][b].setColor(sf::Color::Green);
					if(level%3==2)
						mushroomSprite[a][b].setColor(sf::Color::Magenta);
					
				}
			}		
		}
		
		//generate centipede segment if centipede enters player area
		bool generateSegment=0;
		for(int a=0; a<c_size; a++)
		{
			if(centipede[a][exists] && centipede[a][y]>=(gameRows-5)*boxPixelsY)
				generateSegment=1;
		}
		if(segmentclock.getElapsedTime().asSeconds() > (4 / gamespeed) && generateSegment)
			centipedeSegment(centipede,cdir,head,speed,c_size,segmentclock,centipedeSprite,cHeadLeftTexture, cHeadRightTexture, centipedeSpeed);
		
		
		
		//move centipede
		for(int a=0; a<c_size; a++)
		{	
			moveCentipede(centipede, centipedeClock,centipedeSprite, a, cdir, mushroom, centipededead, c_size, speed);
		}
		
		
		//check for bullet-centipede collision
		checkCentipedeBulletCollision(centipede, bullet, c_size, mushroom, head, score, level, maxBullets, kill);
		
		
		//draw centipede
		drawCentipede(c_size, centipede, centipedeSprite, cdir, head, cHeadLeftTexture,cHeadRightTexture, cBodyTexture,  window); 
		
		
		
		//draw mushrooms
		drawMushroom(mushroom,window,mushroomSprite);
		
		
		//check if mushroom is hit
		checkBulletMushroomCollision(mushroom,bullet, centipede, c_size, cdir,score, maxBullets);
		
		
		
		//fire bullets
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			
            		
			for(int a=0; a<maxBullets; a++)
			{
				//fire only after 150ms of previous bullet being fired
				if(bullet[a][exists]==false && (spaceclock.getElapsedTime().asMilliseconds()>= 150 / gamespeed))
				{
					fire.play();
					bullet[a][x] = player[x];
					bullet[a][y] = player[y] - boxPixelsY;
					bullet[a][exists] = true;
					spaceclock.restart();
					break;
				}
			}
		}
		
		//move and draw existing bullets
		for(int a=0; a<maxBullets; a++)
		{
			if (bullet[a][exists] == true ) 
			{
				moveBullet(bullet, bulletClock, a);
				drawBullet(window, bullet, bulletSprite, a );
				
			}
		}
		
		
		
		
		
		
		//draw and move player
		if(player[exists])
		{
			drawPlayer(window, player, playerSprite);
		
			int dirPlayer;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				dirPlayer=1;
				movePlayer(dirPlayer, player,mushroom, death, playerSprite, window, centipede, deathsound, c_size);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				dirPlayer=2;
				movePlayer(dirPlayer, player,mushroom, death, playerSprite, window, centipede,deathsound, c_size);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				dirPlayer=0;
				movePlayer(dirPlayer, player,mushroom, death, playerSprite, window, centipede,deathsound, c_size);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
				dirPlayer=3;
				movePlayer(dirPlayer, player,mushroom, death, playerSprite, window,centipede,deathsound,  c_size);
			}
		}
		
		
		
		
		//check for centipede-player collision
		 centipedePlayerCollision(centipede, player, c_size, death,  playerSprite, window, deathsound);
		

		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				return 0;
			}
		}		
		window.display();
		window.clear();
	}
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////


//Display dead menu which includes final score when player dies
bool DeadMenu(sf::RenderWindow& window,sf::Sprite backgroundSprite, int score, sf::Font font)
{
		sf::Event menu;
		sf::Text deadT("You died", font, 70);
		deadT.setFillColor(sf::Color::Red);
		deadT.setPosition(305,250);
		sf::Text scoreT("", font, 40);
		scoreT.setPosition(275,500);
		scoreT.setString("Your score is " + to_string(score));
		
		sf::Text exit("Press enter to exit", font, 40);
		exit.setPosition(260,600);
		
		while(true)
		{
			window.clear();
			window.draw(backgroundSprite);
			window.draw(deadT);
			window.draw(scoreT);
			window.draw(exit);
			window.display();
			
			while (window.pollEvent(menu)) 
			{
				if (menu.type == sf::Event::Closed) 
				{
					return 0;
				}
			
				if(menu.type == sf::Event::KeyPressed && (menu.key.code == sf::Keyboard::Return))
				{
					return 0;
				}
			}	
		}
}



//Display Main menu when game starts
bool mainMenu(sf::RenderWindow& window, sf::Text& title, sf::Text& start, sf::Text& exit, bool& select, sf::Sprite backgroundSprite)
{
		sf::Event menu;
		while(true)
		{
			
			while (window.pollEvent(menu)) 
			{
				if (menu.type == sf::Event::Closed) 
				{
					return 0;
				}
				
				//selecting option between start and exit
				if(menu.type == sf::Event::KeyPressed && (menu.key.code == sf::Keyboard::Down || menu.key.code == sf::Keyboard::Up))
				{
				
					if(select==1)//(start)
					{
						start.setFillColor(sf::Color::White);
						exit.setFillColor(sf::Color::Yellow);
						select=0;
					}
					else if(select==0)//(exit)
					{
						start.setFillColor(sf::Color::Yellow);
						exit.setFillColor(sf::Color::White);
						select=1;
					}
					
				}
				
				//What to do when enter is pressed
				if(menu.type == sf::Event::KeyPressed && (menu.key.code == sf::Keyboard::Return))
				{
					if(select==1)
						return 1;//will start the game
					else
						return 0;//will exit the game
				}
				
				
				window.clear();
				window.draw(backgroundSprite);
				window.draw(title);
				window.draw(start);
				window.draw(exit);
				window.display();
	
			}
			
			
		}
		
}

//sequence to execute when player dies
void killplayer(float player[], sf::Texture death, sf::Sprite& playerSprite, sf::RenderWindow& window, float centipede[][6], sf::Sound deathsound, const int c_size)
{

	//play death sound and animation
	deathsound.play();
	sf::Clock deathClock;
	sf::Sprite deathSprite;
	deathSprite.setTexture(death);
	deathSprite.setPosition(player[x],player[y]);
	player[exists]=0;
	static int count=0;

	while(count<=5)
	{
		
		
		if(deathClock.getElapsedTime().asMilliseconds()>190)
		{
			count++;
			deathClock.restart();
			
		}
		deathSprite.setTextureRect(sf::IntRect(boxPixelsX*count, 0, boxPixelsX, boxPixelsY));
		window.draw(deathSprite);
		window.display();
		
		
	}
	
	//kill centipede too
	for(int a=0; a<c_size; a++)
	{
		centipede[a][exists]=0;
	}
	
	

}


//check if centipede hit player
void centipedePlayerCollision(float centipede[][6], float player[], const int c_size, sf::Texture death, sf::Sprite& playerSprite, sf::RenderWindow& window,sf::Sound deathsound)
{
	bool killplayerb=0;
	
	//check for each segment if it enters the player hit box
	for(int a=0; a<c_size; a++)
	{
		if(centipede[a][exists])
		{
			if(player[y]>centipede[a][y]-boxPixelsY && player[y]<centipede[a][y]+boxPixelsY && player[x]<centipede[a][x]+boxPixelsX && player[x]>=centipede[a][x])
				killplayerb=1;
			if(player[y]>centipede[a][y]-boxPixelsY && player[y]<centipede[a][y]+boxPixelsY && player[x]>centipede[a][x]-boxPixelsX && player[x]<=centipede[a][x])
				killplayerb=1;	
			
			if(player[x]>centipede[a][x]-boxPixelsX && player[x]<centipede[a][x]+boxPixelsX && player[y]<centipede[a][y]+boxPixelsY && player[y]>=centipede[a][y])
				killplayerb=1;
					
			if(player[x]>centipede[a][x]-boxPixelsX && player[x]<centipede[a][x]+boxPixelsX && player[y]>centipede[a][y]-boxPixelsY && player[y]<=centipede[a][y])
				killplayerb=1;
		}
	}
	
	//if it does hit player, execute kill player sequence
	if(killplayerb)
	{
	
		killplayer(player, death, playerSprite, window, centipede,deathsound, c_size);
	}
		
}


//generate seperate segment when main centipede enters player area
void centipedeSegment(float centipede[][6], int cdir, int head, int speed, const int c_size, sf::Clock& segmentclock, sf::Sprite centipedeSprite[],sf::Texture cHeadLeftTexture,sf::Texture cHeadRightTexture, int centipedeSpeed )
{
	
		static int g=c_size-1;
		if(g==0)
			g=c_size-1;
		if(!centipede[g][exists])
		{
			//sometimes spawn from left side
			if(g%2==0)
				centipede[g][x]=(1)*boxPixelsX;
				
			//sometime spawn from right(alternating)
			else
				centipede[g][x]=(gameColumns-2)*boxPixelsX;
				
			
			centipede[g][y]=0;
			centipede[g][exists]=1;
			centipede[g][cdir]=-1;
			centipede[g][head]=1;
			centipede[g][speed]=centipedeSpeed;
			centipedeSprite[g].setTexture(cHeadLeftTexture);
			centipedeSprite[g].setTextureRect(sf::IntRect(0,0,28,boxPixelsY));
			g--;
			segmentclock.restart();
			return;
		}
		else
			g--;
			
				
	
}


//set centipede texture according to direction and head
void drawCentipede(const int c_size, float centipede[][6],sf::Sprite centipedeSprite[],int cdir,int head,sf::Texture cHeadLeftTexture,sf::Texture cHeadRightTexture, sf::Texture cBodyTexture, sf::RenderWindow& window)
{
	for(int a=0; a<c_size; a++)
	{
			if(centipede[a][exists])
			{	
			
				if(centipede[a][cdir]==-1 )
				{	
					if(centipede[a][head])
					{
						centipedeSprite[a].setTexture(cHeadLeftTexture);
					}
					else
						centipedeSprite[a].setTextureRect(sf::IntRect(28,0,28,boxPixelsY));
					
					
				}
				else if(centipede[a][cdir]==1)
				{
					if(centipede[a][head])
					{
						centipedeSprite[a].setTexture(cHeadRightTexture);
					}
					else
						centipedeSprite[a].setTextureRect(sf::IntRect(28,0,28,boxPixelsY));
				}
				
				//draw
				centipedeSprite[a].setPosition(centipede[a][x],centipede[a][y]);
				window.draw(centipedeSprite[a]);
				for(int b=0; b<c_size; b++)
				{
					if(centipede[b][head] && centipede[b][exists])
						window.draw(centipedeSprite[b]);
				}
			}
	}
}




//return bool if centipede collides with mushroom
bool checkCentipedeMushroomCollison(float centipede[][6], const int c_size, int mushroom[][gameColumns], int cdir, int a, int b)
{
	for(int c=0; c<c_size; c++)
	{
		if(centipede[c][exists])
		{
			//if moving right
			if(centipede[c][cdir]==1)
			{
				if(centipede[c][x]>=(b*boxPixelsX)-32 && centipede[c][x]<(b*boxPixelsX))
					return 1;
			}
			
			//if moving left
			else if(centipede[c][cdir]==-1)
			{
				if(centipede[c][x]<=(b*boxPixelsX)+32 && centipede[c][x]>(b*boxPixelsX))
					return 1;
			}
		}
	}
	return 0;
}

//fragment centipede upon being hit
void checkCentipedeBulletCollision(float centipede[][6], float bullet[][3], const int c_size, int mushroom[][gameColumns], int head, int& score, int level, int maxBullets, sf::Sound kill)
{
	for(int a=0; a<c_size; a++)
	{
		for(int b=0; b<maxBullets; b++)
		{
			if(centipede[a][exists])
			{
				//if bullet enters centipede segment's hitbox
				if(bullet[b][exists] && bullet[b][x]>=centipede[a][x]-16 && bullet[b][x]<=centipede[a][x]+16 && bullet[b][y]>=centipede[a][y]-boxPixelsY && bullet[b][y]<=centipede[a][y])
				{
				
					//kill bullet and centipede segment
					bullet[b][exists]=false;
					centipede[a][exists]=false;
					kill.play();
					
					//scoring
					if(centipede[a][head])
						score+=20;
					else
						score+=10;
					
					
					//mushroom generation at point of impact
					int mushX;
					if(centipede[a][x]/boxPixelsX>=gameColumns-1)
						mushX=gameColumns-1;
					else if(centipede[a][3]==-1)
						mushX=centipede[a][x]/boxPixelsX;
					else if(centipede[a][3]==1)
						mushX=round(centipede[a][x]/boxPixelsX)+1;
						
					int mushY=centipede[a][y]/boxPixelsY;
					centipede[a+1][head]=true;	//make the centipede segment before into head
					
					
					if(mushY<gameRows-5)
						mushroom[mushY][mushX]=2;//generate normal mushroom 
					else
						mushroom[mushY][mushX]=10;//generate poisonous mushroom in player area
						
						
					
					//killing whole centipede if head is hit
					if(centipede[a][head] && level==0)
					{
					
						//find length of that centipede segment
						int length=a;
						for(int k=0; k<c_size; k++)
						{
							if(centipede[k][exists])
							{
								if(centipede[a][3]==-1 && centipede[a][y]==centipede[k][y])
								{
									if(centipede[k][x]==centipede[a][x]+((k-a)*32))
										length++;
								}
								else if(centipede[a][3]==1 && centipede[a][y]==centipede[k][y])
								{
									if(centipede[k][x]==centipede[a][x]-((k-a)*32))
										length++;
									
									
								}
								for(int z=0; z<c_size; z++)
								{
									if(centipede[k][y]==centipede[z][y]+boxPixelsY && centipede[k][x]==centipede[z][x])
										length++;
								}
							}
							
							
						}
						
						//kill all segments behind that head
						for(int k=a; k<=length; k++)
						{
						
							
							if(centipede[k][exists])
							{
								centipede[k][exists]=false;
								if(centipede[k][head])
									score+=10;
								else
									score+=10;
									
								//generate mushrooms
								if(centipede[k][x]/boxPixelsX>=gameColumns-1)
									mushX=gameColumns-1;
								else if(centipede[k][3]==-1)
										mushX=centipede[k][x]/boxPixelsX;
								else if(centipede[a][3]==1)
									mushX=round(centipede[k][x]/boxPixelsX)+1;
								mushY=centipede[k][y]/boxPixelsY;
								if(mushY<gameRows-5)
									mushroom[mushY][mushX]=2;
								else
									mushroom[mushY][mushX]=10;
							}
							
						}
						if(centipede[length+1][exists])
							centipede[length+1][head]=true;
					}
					return;
					
					
					
				}
			}
		}
	}
}


//includes everything related to moving centipede
void moveCentipede(float centipede[][6], sf::Clock centipedeClock[],sf::Sprite centipedeSprite[], int a, const int cdir, int mushroom[][gameColumns], bool& centipededead, const int c_size, int speed)
{
	
	//bool variables that will be used to determine if centipede is changing Y coordinate
	static bool movingdown[30]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} , mush[30]={0};
	
	//temp variables that will store the future Y coordinate when a centipede is to move down
	static int temp[30]={0};
	static float centipedeclock=2;
	
	//stores the Y-orientaion of each centipede segment(moving down or moving up)
	static int Y[30]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	
	//reset variables when centipede dies
	if(centipededead)
	{
		for(int a=0; a<c_size; a++)
		{
			movingdown[a]=1;
			mush[a]=0;
			temp[a]=0;
			Y[a]=1;
		}
	}
	
	if(!centipede[a][exists])
		return;
	
	
	//reverse Y-orientation when centipede reaches bottom of screen or when it goes back up to the maximum of player area
	if(centipede[a][y]>=(gameRows-6)*boxPixelsY && centipede[a][y]<(gameRows-5)*boxPixelsY && Y[a]==-1)
		Y[a]=1;
	else if(centipede[a][y]>(gameRows-1)*boxPixelsY && Y[a]==1)
		Y[a]=-1;
		
	
	//handle centipede segment overlapping(by reversing direction)
	for(int p=0; p<c_size; p++)
	{
		for(int q=0; q<c_size; q++)
		{
			if(Y[p]==Y[q] && p!=q && centipede[p][exists] && centipede[q][exists] && centipede[p][cdir]==centipede[q][cdir]  && centipede[p][x]==centipede[q][x]  && centipede[p][y]==centipede[q][y])
			
				if(centipede[p][4] && !centipede[q][4])
				{
					centipede[p][cdir]*=-1;
					
				}
					
					
				else if(centipede[q][4] && !centipede[p][4])
				{
					centipede[p][cdir]*=-1;
				
				}
					
				else
				{
					centipede[p][cdir]*=-1;
					
				}	
		}
	}
		
	
	//centipede segment colliding with mushroom
	for(int m=0; m<gameRows; m++)
	{
		for(int n=0; n<gameColumns; n++)
		{
			
			//if segment is moving left
			if( centipede[a][cdir]==-1 && mush[a] ||( centipede[a][cdir]==-1 && centipede[a][x]==(n+1)*boxPixelsX && centipede[a][y]==m*boxPixelsY && mushroom[m][n]!=0))
			{
				if(centipedeClock[a].getElapsedTime().asMilliseconds() < centipedeclock/ gamespeed)
					return;
					
				//handle when centipede is on the right most end and mushroom is next to it
				if(centipede[a][x]==(gameColumns-1)*boxPixelsX)
				{
					centipede[a][y]+=Y[a]*centipede[a][speed];
					mush[a]=1;
					centipedeClock[a].restart();
					if(centipede[a][y]==temp[a]+(Y[a]*2*boxPixelsX))
					{
						movingdown[a]=0;
						mush[a]=0;
						temp[a]=centipede[a][y];
						centipede[a][cdir]*=1;
						
					}
					return;
				}
				
				//moving down elsewhere
				else
				{
					centipede[a][y]+=Y[a]*centipede[a][speed];
					mush[a]=1;
					centipedeClock[a].restart();
					if(centipede[a][y]==temp[a]+(Y[a]*boxPixelsY))
					{
						movingdown[a]=0;
						mush[a]=0;
						temp[a]=centipede[a][y];
						centipede[a][cdir]*=-1;
					}
					return;
				}
			}
			
			//if segment moving right
			else if( centipede[a][cdir]==1 && mush[a] ||( centipede[a][cdir]==1 && centipede[a][x]==(n-1)*boxPixelsX && centipede[a][y]==m*boxPixelsY && mushroom[m][n]!=0))
			{
				if(centipedeClock[a].getElapsedTime().asMilliseconds() < centipedeclock/ gamespeed)
					return;
					
				//handle if segment is on leftmost end and mushroom is next to it
				if(centipede[a][x]==0)
				{
					centipede[a][y]+=Y[a]*centipede[a][speed];
					mush[a]=1;
					centipedeClock[a].restart();
					if(centipede[a][y]==temp[a]+(Y[a]*2*boxPixelsY))
					{
						movingdown[a]=0;
						mush[a]=0;
						temp[a]=centipede[a][y];
						centipede[a][cdir]*=1;
					}
					return;
				}
				
				//moving down elsewhere
				else
				{
					centipede[a][y]+=Y[a]*centipede[a][speed];
					mush[a]=1;
					centipedeClock[a].restart();
					if(centipede[a][y]==temp[a]+(Y[a]*1*boxPixelsY))
					{
						movingdown[a]=0;
						mush[a]=0;
						temp[a]=centipede[a][y];
						centipede[a][cdir]*=-1;
					}
					return;
				}
			}
			
			
		}
	}
	
	
	//generic moving down sequence when centipede reaches window boundary
	if(movingdown[a] &&(centipede[a][x]<=0 || centipede[a][x]>=(gameColumns-1)*boxPixelsX))
	{
		if(centipedeClock[a].getElapsedTime().asMilliseconds() < centipedeclock/ gamespeed)
			return;
		else
		{
			centipede[a][y]+=Y[a]*centipede[a][speed];
			centipedeClock[a].restart();
			if(centipede[a][y]==temp[a]+(Y[a]*1*boxPixelsY))
			{
				movingdown[a]=0;
				temp[a]=centipede[a][y];
				centipede[a][cdir]*=-1;
				
			}
			return;
		}
	}
	
	//moving to the left
	if(centipede[a][cdir]==-1)
	{
		if(centipedeClock[a].getElapsedTime().asMilliseconds() < centipedeclock/ gamespeed)
			return;
		else
		{
			centipede[a][x]-=centipede[a][speed];
			movingdown[a]=1;
			centipedeClock[a].restart();
			return;
		}
	}
	
	//moving to the right
	if(centipede[a][cdir]==1)
	{
			if(centipedeClock[a].getElapsedTime().asMilliseconds() < centipedeclock/ gamespeed)
				return;
			else
			{
				centipede[a][x]+=centipede[a][speed];
				movingdown[a]=1;
				centipedeClock[a].restart();
				return;
			}
	}
	
	
	
	
}




//bullet hitting mushroom check
void checkBulletMushroomCollision(int mushroom[][gameColumns], float bullet[][3], float centipede[][6], const int c_size, int cdir, int& score, int maxBullets)
{
	static sf::Clock mushClock[gameRows][gameColumns];
	for(int a=0; a<gameRows; a++)
		{
			for(int b=0; b<gameColumns; b++)
			{
				for(int c=0; c<maxBullets; c++)
				{
					if( ((bullet[c][exists]==true)&&(bullet[c][y]>=(a-1)*boxPixelsY) && (bullet[c][y]<(a)*boxPixelsY)) && ((bullet[c][x]>=b*boxPixelsX-16)&&(bullet[c][x]<b*boxPixelsX+16)) && (mushroom[a][b]==1 || mushroom[a][b]==2 || mushroom[a][b]==10 || mushroom[a][b]==9 ))
					{
						
						bullet[c][exists]=false;
						
						//decrease life of mushroom if it is full
						if(mushroom[a][b]==2 || mushroom[a][b]==10 )
							mushroom[a][b]--;
							
						//if it is half destroyed, decrease life only after making sure there is no centipede collision with mushroom taking place anymore, meanwhile disappear the mushroom
						else if(mushroom[a][b]==1 || mushroom[a][b]==9 )
						{
							bool colliding=0;
							for(int d=0; d<c_size; d++)
							{	
								if(checkCentipedeMushroomCollison(centipede, c_size, mushroom, cdir,a,b))
									colliding=1;
							}
							if(colliding)
							{
								mushroom[a][b]=-1;
								score++;
								mushClock[a][b].restart();
							}
							else
							{
								mushroom[a][b]=0;
								score++;
							}
								

						}
					}					
				}
			}
		}
		//actually kill the mushroom after centipede has passed
		for(int a=0; a<gameRows; a++)
		{
			for(int b=0; b<gameColumns; b++)
			{
				if(mushroom[a][b]==-1)
				{
					if(mushClock[a][b].getElapsedTime().asMilliseconds()>1200)
						mushroom[a][b]=0;
				}
			}
		}
}


//draw mushrooms according to their lives/poisonous
void drawMushroom(int mushroom[][gameColumns], sf::RenderWindow& window, sf::Sprite mushroomSprite[][gameColumns])
{
	for(int a=0; a<gameRows; a++)
	{
		for(int b=0; b<gameColumns; b++)
		{
			
			//normal mushroom with 2 lives
			if(mushroom[a][b]==2)
			{
				mushroomSprite[a][b].setTextureRect(sf::IntRect(0,0,boxPixelsX,boxPixelsY));
				mushroomSprite[a][b].setPosition( b*boxPixelsX, a*boxPixelsY);
				window.draw(mushroomSprite[a][b]);
				
			}
			
			//normal mushroom with 1 life
			else if(mushroom[a][b]==1)
			{
				mushroomSprite[a][b].setTextureRect(sf::IntRect(96,0,boxPixelsX,boxPixelsY));
				mushroomSprite[a][b].setPosition( b*boxPixelsX, a*boxPixelsY);
				window.draw(mushroomSprite[a][b]);
			}
			
			//poisonous mushroom with 2 lives
			else if(mushroom[a][b]==10)
			{
				mushroomSprite[a][b].setTextureRect(sf::IntRect(0,32,boxPixelsX,boxPixelsY));
				mushroomSprite[a][b].setPosition( b*boxPixelsX, a*boxPixelsY);
				window.draw(mushroomSprite[a][b]);
			}
			
			//poisonous mushroom with 1 life
			else if(mushroom[a][b]==9)
			{
				mushroomSprite[a][b].setTextureRect(sf::IntRect(96,32,boxPixelsX,boxPixelsY));
				mushroomSprite[a][b].setPosition( b*boxPixelsX, a*boxPixelsY);
				window.draw(mushroomSprite[a][b]);
			}
		}
	}
}

//move player
void movePlayer(int& dirPlayer, float player[], int mushroom[][gameColumns], sf::Texture death, sf::Sprite& playerSprite, sf::RenderWindow& window, float centipede[][6],sf::Sound deathsound, const int c_size)
{

	//move left
	if(dirPlayer==0)
	{
		bool move=1;
		for(int a=0; a<gameRows; a++)
		{
			for(int b=0; b<gameColumns; b++)
			{
			
				//if hits poisounous mushroom, kill player
				if(mushroom[a][b]==10 || mushroom[a][b]==9)

				{
					if((player[y]>a*boxPixelsY-boxPixelsY && player[y]<a*boxPixelsY+boxPixelsY && player[x]==b*boxPixelsX+boxPixelsX))
					{
						killplayer(player, death, playerSprite, window, centipede,deathsound, c_size);
						return;
					}
				}
				
				
				//do not allow movement if normal mushroom is colliding
				if(mushroom[a][b]==1 || mushroom[a][b]==2)
				{
					if((player[y]>a*boxPixelsY-boxPixelsY && player[y]<a*boxPixelsY+boxPixelsY && player[x]==b*boxPixelsX+boxPixelsX))
						move=0;
				}
				
				//do not allow movement if on window boundary
				else if(player[x]==0)
					move=0;
					
			}
		}
		if(move)
			player[x]-=2;
	}
	
	
	//move up
	if(dirPlayer==1)
	{
		bool move=1;
		for(int a=0; a<gameRows; a++)
		{
			for(int b=0; b<gameColumns; b++)
			{
				//poison mushroom collision
				if(mushroom[a][b]==10 || mushroom[a][b]==9)
				{
					if((player[x]>b*boxPixelsX-boxPixelsX && player[x]<b*boxPixelsX+boxPixelsX && player[y]==a*boxPixelsY+boxPixelsY))
					{
						killplayer(player, death, playerSprite, window, centipede,deathsound, c_size);
						return;
					}
				}
				
				
				//normal mushroom collision
				if(mushroom[a][b]==1 || mushroom[a][b]==2)
				{
					if((player[x]>b*boxPixelsX-boxPixelsX && player[x]<b*boxPixelsX+boxPixelsX && player[y]==a*boxPixelsY+boxPixelsY))
						move=0;
				}
				
				//do not allow to leave player area(5 rows)
				else if(player[y]==(resolutionY-(5*boxPixelsY)))
					move=0;
					
			}
		}
		if(move)
			player[y]-=2;
		
	}
	
	
	//move down
	if(dirPlayer==2)
	{
		bool move=1;
		for(int a=0; a<gameRows; a++)
		{
			for(int b=0; b<gameColumns; b++)
			{
				//poison mushroom
				if(mushroom[a][b]==10 || mushroom[a][b]==9)
				{
					if((player[x]>b*boxPixelsX-boxPixelsX && player[x]<b*boxPixelsX+boxPixelsX && player[y]==a*boxPixelsY-boxPixelsY))
					{
						killplayer(player, death, playerSprite, window,centipede,deathsound, c_size);
						return;
					}
				}	
				
				
				//normal mushroom collison
				if(mushroom[a][b]==1 || mushroom[a][b]==2)
				{
					if((player[x]>b*boxPixelsX-boxPixelsX && player[x]<b*boxPixelsX+boxPixelsX && player[y]==a*boxPixelsY-boxPixelsY))
						move=0;
				}
				
				//boundary check
				else if(player[y]==(resolutionY-boxPixelsY))
					move=0;
					
			}
		}
		if(move)
			player[y]+=2;
		
	}
	
	//move right
	if(dirPlayer==3)
	{
		bool move=1;
		for(int a=0; a<gameRows; a++)
		{
			for(int b=0; b<gameColumns; b++)
			{
			
				//poison mushroom
				if(mushroom[a][b]==10 || mushroom[a][b]==9)
				{
					if((player[y]>a*boxPixelsY-boxPixelsY && player[y]<a*boxPixelsY+boxPixelsY && player[x]==b*boxPixelsX-boxPixelsX))
					{
						killplayer(player, death, playerSprite, window,centipede,deathsound, c_size);
						return;
					}
					
				}
				
				
				//normal mushroom
				if(mushroom[a][b]==1 || mushroom[a][b]==2)
				{
					if((player[y]>a*boxPixelsY-boxPixelsY && player[y]<a*boxPixelsY+boxPixelsY && player[x]==b*boxPixelsX-boxPixelsX))
						move=0;
				}
				
				//boundary check
				else if(player[x]==(boxPixelsX*(gameRows-1)))
					move=0;
					
			}
		}
		if(move)
			player[x]+=2;
		
	}
	dirPlayer=5;
}

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}
void moveBullet(float bullet[][3], sf::Clock bulletClock[], int a) {
	if (bulletClock[a].getElapsedTime().asMilliseconds() < 3)
		return;

	bulletClock[a].restart();
	bullet[a][y] -= 20;	
	if (bullet[a][y] <-32)
		bullet[a][exists] = false;
}
void drawBullet(sf::RenderWindow& window, float bullet[][3], sf::Sprite bulletSprite[], int a) {
	bulletSprite[a].setPosition(bullet[a][x], bullet[a][y]);
	window.draw(bulletSprite[a]);
}









