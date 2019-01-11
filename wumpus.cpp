#include "wumpus.h"
#include <ctime> // for time()
#include <cstdlib> // for rand() and srand()

namespace wumpus
{
	//-------------------------------------------------------------
	//Private functions
	//-------------------------------------------------------------

	Dungeon::Dungeon()
	{
		//retry make the connections if not sucessfull
	
		rooms.clear();
		for (int i = 0; i < count_of_rooms; ++i)     //create rooms
			rooms.push_back(i);

		connect_rooms_to_dodecahedron(rooms);

		//shuffleRoom(rooms);


		// add the wumpus 
		///std::cout << "debugging countOfRooms: " << count_of_rooms << "\n";

		wumpus_room = get_random(0, count_of_rooms);
		rooms[wumpus_room].wumpus = true;

		// add pit
		for (int i = 0; i < count_of_pits; ++i)
		{
			int pit_room = get_random(0, count_of_rooms);
			// no wumpus or pit already are in the same room
			while ((rooms[pit_room].wumpus == true) || (rooms[pit_room].pit == true))
				pit_room = get_random(0, count_of_rooms);

			rooms[pit_room].pit = true;
		}

		// add bat
		for (int i = 0; i < count_of_bats; ++i)
		{
			int bat_room = get_random(0, count_of_bats);
			// no wumpus or pit or bat already are in the same room
			while ((rooms[bat_room].wumpus == true) || (rooms[bat_room].pit == true) || (rooms[bat_room].bat == true))
				bat_room = get_random(0, count_of_rooms);

			rooms[bat_room].bat = true;
		}

		// add player
		player_room = get_random(0, count_of_rooms);
		rooms[player_room].player = true;

		// wumpus or pit or bat or player are in the same room
		while ((rooms[player_room].wumpus == true) || (rooms[player_room].pit == true) || (rooms[player_room].bat == true))
			player_room = get_random(0, count_of_rooms);

		rooms[player_room].player = true;

		arrows = count_of_arrows;
	}

	void Dungeon::indicate_hazards()
	{
		bool found_pit = false;
		bool found_bat = false;

		//std::cout << "debugging indicate_hazards() fnc\n";
		
		for (const auto& x : rooms[player_room].brooms)
		{
			if (x->wumpus == true)
				std::cout << "I smell the wumpus!\n";
			else if (x->pit == true && found_pit == false)
			{	
				found_pit = true;
				std::cout << "I feel a breeze!\n";
			}
			else if (x->bat == true && found_bat == false)
			{
				found_bat = true;
				std::cout << "I hear a bat!\n";
			}
			else
				;
		}

		std::cout << "You are in room " << rooms[player_room].rnumber << "\n";
		std::cout << "You have " << arrows << " arrow(s) left\n";
		std::cout << "Tunnels lead to rooms " << rooms[player_room].brooms[0]->rnumber << ", " 
											<< rooms[player_room].brooms[1]->rnumber << " and " 
											<< rooms[player_room].brooms[2]->rnumber << "\n";
		std::cout << "what do you want to do? (M)ove or (S)hoot?\n";
		
	}

	bool Dungeon::shoot_arrow(std::vector<int> tar_rooms)
	{
		//try to shoot in the supplied tar rooms an arrow
		//if the wumpus is hit, returns true to indicate victory, moves the wumpus on fail

		--arrows;

		int curr_room = player_room;
		for (const auto& tr : tar_rooms)
		{
			bool room_reached = false;
			//check if neigbour room is one of the vectors
			for (const auto& x : rooms[curr_room].brooms)
			{
				if (x->rnumber == tr)
				{
					room_reached = true;
					// curr_room = x->rnumber;
					// wumpus hit
					if (rooms[tr].wumpus)
					{
						std::cout << "!!! YOU WON !!!: YOU KILLED THE WUMPUS IN ROOM" << rooms[tr].rnumber << "\n";
						return true;
					}
					else
					{
						std::cout << "Missing Shot!\n";
						if (arrows == 0)
						{
							std::cout << "YOU LOST !: You ran out of arrows\n";
							return true;
						}

						return false;
					}
						
				}
			}
			if (!room_reached) // if not end
			{
				std::cout << "Room " << tr << " could not be reached from arrow\n";
				return false;
			}
		
		}

	}
	
	bool Dungeon::move_wumpus()
	{
		//moves the wumpus with a chance of 75% to a new room
		//if player room is entered true, return for game over
		if (get_random(1, 4) == 4)	// no movement on 4
			return false;
		else
		{
			rooms[wumpus_room].wumpus = false;
			wumpus_room = rooms[wumpus_room].brooms[get_random(0, 2)]->rnumber;
			rooms[wumpus_room].wumpus = true;
			if (rooms[wumpus_room].player)
			{
				std::cout << "YOU LOST !: Wumpus enters your room and eats you\n";
				return true;
			}
		}
		return false;
	}

	bool Dungeon::move_player(int dest)
	{
		//try to move player to the selected room
		//if deadly hazard like pit or wumpus is found return game over = true;
		//if bat is found choose new random room free from hazards to put the player
		for (const auto& r : rooms[player_room].brooms)
		{
			if (r->rnumber == dest)
			{
				if (r->wumpus == true)
				{
					std::cout << "YOU LOST !: You got eaten by the wumpus\n";
					return true;
				}
				else if (r->pit == true)
				{
					std::cout << "YOU LOST !: You fell in a bottomless pit\n";
					return true;
				}
				else if (r->bat == true)
				{
					std::cout << "Gigantic bat appeared!!!\n";
					std::cout << "You got dragged to a new room\n";
					int rnd_room = get_random(0, count_of_rooms);
					std::cout << "rnd_room: " << rnd_room << "\n";
					//std::cout << "rooms[rnd_room].wumpus status: " << rooms[rnd_room].wumpus << "\n";
					//std::cout << "rooms[rnd_room].pit status: " << rooms[rnd_room].wumpus << "\n";
					//std::cout << "rooms[rnd_room].bat status: " << rooms[rnd_room].bat << "\n";
					//std::cout << "rooms[rnd_room].player status: " << rooms[rnd_room].player << "\n";
					// Only put player in empty room
					while (rooms[rnd_room].wumpus == true || rooms[rnd_room].pit == true || rooms[rnd_room].bat == true || rooms[rnd_room].player == true)
					{
						rnd_room = get_random(0, count_of_rooms);
					}
					//std::cout << "Debugging here, is it ok at this step?\n";
					rooms[player_room].player = false;
					player_room = rnd_room;
					rooms[player_room].player = true;
					return false;
					
				}
				else
				{
					rooms[player_room].player = false;
					player_room = dest;
					rooms[player_room].player = true;
					return false;
				}
			}
			else
			{
				;
			}
		}

		std::cerr << "Dungeon::move_player: Unknown target room entered!\n";
		return false;

	}

	void Dungeon::debug()
	{
		for (const auto& i : rooms)
		{
			std::cout << "Room " << i.rnumber << "connected to: ";
			for (const auto& j : i.brooms)
			{
				if (j != nullptr)
					std::cout << j->rnumber << " ";
				else
					std::cout << "np" << " ";
			}
			std::cout << " ";
			if (i.wumpus)
				std::cout << "wumpus:" << i.wumpus << " ";
			else if (i.pit)
				std::cout << "pit:" << i.pit << " ";
			else if (i.bat)
				std::cout << "bat:" << i.bat << " ";
			else if (i.player)
				std::cout << "player:" << i.player << " ";
			else
				;
			std::cout << "\n";
		}
	}

	std::vector<int> Dungeon::neighbour_rooms(int room)
	{
		std::vector<int> vect_nrooms;
		for (const auto& elem : rooms[room].brooms)
			vect_nrooms.push_back(elem->rnumber);
		return vect_nrooms;
	}

	int Dungeon::current_room()
	{
		return player_room;
	}

	//-------------------------------------------------------------
	//Private functions
	//-------------------------------------------------------------

	bool Dungeon::room_is_full_connected(const int con, const std::vector<int>& crooms)
	{
		//checks if the room has already 3 connections so it is on the black list
		for (const auto& cr : crooms)	// room is on the black list
		{
			if (cr == con)
				return true;
		}
		return false;

	}

	void Dungeon::connect_rooms_to_dodecahedron(std::vector<Room> &rooms)
	{
		//std::vector<Room> dodecahedron;
		rooms[0].brooms = { &rooms[1], &rooms[4], &rooms[7] };		//0
		rooms[1].brooms = { &rooms[9], &rooms[2], &rooms[0] };		//1
		rooms[2].brooms = { &rooms[11], &rooms[3], &rooms[1] };		//2
		rooms[3].brooms = { &rooms[2], &rooms[13], &rooms[4] };		//3
		rooms[4].brooms = { &rooms[3], &rooms[5], &rooms[0] };		//4
		rooms[5].brooms = { &rooms[4], &rooms[14], &rooms[6] };		//5
		rooms[6].brooms = { &rooms[16], &rooms[7], &rooms[5] };		//6
		rooms[7].brooms = { &rooms[0], &rooms[6], &rooms[10] };		//7
		rooms[8].brooms = { &rooms[9], &rooms[18], &rooms[11] };		//8
		rooms[9].brooms = { &rooms[10], &rooms[8], &rooms[1] };			//9
		rooms[10].brooms = { &rooms[7], &rooms[19], &rooms[9] };		//10
		rooms[11].brooms = { &rooms[8], &rooms[12], &rooms[2] };		//11
		rooms[12].brooms = { &rooms[11], &rooms[17], &rooms[13] };		//12
		rooms[13].brooms = { &rooms[12], &rooms[14], &rooms[3] };		//13
		rooms[14].brooms = { &rooms[15], &rooms[5], &rooms[13] };		//14
		rooms[15].brooms = { &rooms[17], &rooms[16], &rooms[14] };		//15
		rooms[16].brooms = { &rooms[6], &rooms[15], &rooms[19] };		//16
		rooms[17].brooms = { &rooms[18], &rooms[15], &rooms[12] };		//17
		rooms[18].brooms = { &rooms[19], &rooms[17], &rooms[8] };		//18
		rooms[19].brooms = { &rooms[10], &rooms[16], &rooms[18] };		//19
	}

	// Generate a random number between min and max (inclusive)
	// Assumes srand() has already been called
	int get_random(int min, int max)
	{
		max -= 1;

		static const double fraction = 1.0 / (static_cast<double>(RAND_MAX)+1.0);  // static used for efficiency, so we only calculate this value once
		// evenly distribute the random number across our range
		return static_cast<int>(rand() * fraction * (max - min + 1) + min);
	}

	static void swapRoom(Room &a,  Room &b)
	{
		Room temp = a;
		a = b;
		b = temp;
	}

	void Dungeon::shuffleRoom(std::vector<Room> &room)
	{
		// Step through each card in the deck
		for (int index = 0; index < count_of_rooms; ++index)
		{
			// Pick a random card, any card
			int swapIndex = get_random(0, count_of_rooms);
			// Swap it with the current card
			swapRoom(room[index], room[swapIndex]);
		}
	}

	void hunt_wumpus()
	{
		init();
		
		//std::cout << "what the fuck here?\n";

		while (true)
		{
			Dungeon d;
			while (true)
			{
				d.indicate_hazards();
				/// (M)ove or (S)hoot
				char ans;
				std::cin >> ans;
				
				if (std::cin.fail())
				{
					std::cin.clear();
					std::cin.ignore(999, '\n');
					continue;
				}

				bool gameOver = false;

				if (ans == 'm' || ans == 'M')
					gameOver = d.move_player(sel_room_to_move(d));
				
				else if (ans == 's'|| ans == 'S')
				{
					gameOver = d.shoot_arrow(sel_room_to_shoot());
					if (gameOver == true)
						break;
					gameOver = d.move_wumpus();
				}
				else
					;
				
				if (gameOver == true)
					break;
			}

			std::cout << "Press any key to start a new game or (q)uit to end game\n";
			char choice;
			std::cin >> choice;
			if (choice == 'q' || choice == 'Q')
				break;
		
		}
		
	}


	void init()
	{
		std::cout <<
			"Welcome to \"Hunt the Wumpus\"!\n"
			"The wumpus lives in a cave of rooms. Each room has 3 tunnels leading to\n"
			"other rooms. (Look at a dodecahedron to see how this works - if you don't know\n"
			"what a dodecahedron is, ask someone or try google).\n"
			"\n"
			"Hazards\n"
			"Bottomless pits - two rooms have bottomless pits in them. If you go there, you\n"
			"fall into the pit (and lose!)\n"
			"Super bats - two other rooms have super bats. If you go there, a bat grabs you\n"
			"and takes you to some other room at random. (Which may be troublesome).\n"
			"\n"
			"Wumpus\n"
			"The wumpus is not bothered by hazards(he has sucker feet and is too big for a\n"
			"bat to lift).Usually he is asleep.Two things wake him up : you shooting an\n"
			"arrow or you entering his room.\n"
			"\n"
			"If the wumpus wakes he moves (p = .75) one room or stays still(p = .25). After\n"
			"that, if he is where you are, he eats you up and you lose!\n"
			"\n"
			"You\n"
			"Each turn you may move or shoot a crooked arrow.\n"
			"Moving: you can move one room (thru one tunnel).\n"
			"Arrows : you have 5 arrows. You lose when you run out of arrows. EACH ARROW CAN GO FROM 1\n"
			"TO 3 ROOMS. You aim by telling the computer the rooms you want the arrow to go\n"
			"to. If the arrow can\'t go that way (if no tunnel) it moves at random to the\n"
			"next room. If the arrow hits the wumpus, you win. If the arrow hits you, you\n"
			"lose.\n"
			"\n"
			"Warnings\n"
			"When you are one room away from a wumpus or hazard, the computer says :\n"
			"\n"
			"Wumpus: \"I smell the wumpus\"\n"
			"Bat : \"I hear a bat\"\n"
			"Pit : \"I feel a breeze\"\n"
			"\n\n"
			"Press any key to start\n";
		
		char c;
		std::cin.get(c);
	}

	int sel_room_to_move(Dungeon& d)
	{
		int target = 0;		/// default room value
		while (true)
		{
			std::cout << "To where ? \n";
			// room number
			std::cin >> target;

			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(32767, '\n');
				continue;
			}

			std::vector<int> nextRoom = d.neighbour_rooms(d.current_room());
			
			if (target == nextRoom[0] || target == nextRoom[1] || target == nextRoom[2])
				return target;
			else
			{
				std::cout << "Please enter room's number adjacent to curent room !\n";
				return false;
			}
		}
	}

	std::vector<int> sel_room_to_shoot()
	{
		for (;;)
		{
			std::cout << "Enter the rooms you want to shoot the arrow (e.g. 2, 19, ...)\n";

			std::string input;
			std::cin >> input;

			std::istringstream ist{ input };

			std::vector<int> target_rooms;

			bool bad_input = false;

			while (!ist.eof()) {

				int room_number;
				ist >> room_number;

				if (ist.fail()) {
					bad_input = true;
					break;
				}

				target_rooms.push_back(room_number);

			}

			if (bad_input) {
				continue;
			}
			else
				return target_rooms;
		}

	}
}
