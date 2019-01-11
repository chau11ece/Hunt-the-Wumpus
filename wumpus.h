#pragma once
#ifndef WUMPUS_H
#define WUMPUS_H

#include <iostream>
#include <array>
#include <random>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

namespace wumpus
{
	struct Room
	{
		int rnumber;

		// pointer to 3 rooms next to this room.
		std::vector<Room*> brooms;

		bool wumpus;
		bool pit;
		bool bat;
		bool player;

		Room(int rnum) : wumpus(false), pit(false), bat(false), player(false), rnumber(rnum)
		{
			brooms.push_back(nullptr);
			brooms.push_back(nullptr);
			brooms.push_back(nullptr);
		}
	};

	class Dungeon
	{
	private:
		static const int count_of_arrows = 5;
		static const int count_of_rooms = 20;
		static const int count_of_pits = count_of_rooms / 6;
		static const int count_of_bats = count_of_rooms / 6;
		//**************************//
		std::vector<Room> rooms;
		//**************************//
		int arrows;
		int wumpus_room;
		int player_room;

		//bool connect_rooms();
		bool room_is_full_connected(const int con, const std::vector<int>& crooms);
		void connect_rooms_to_dodecahedron(std::vector<Room> &rooms);
		void shuffleRoom(std::vector<Room> &rooms);

	public:
		Dungeon();
		void indicate_hazards();
		bool shoot_arrow(std::vector<int> tar_rooms);
		bool move_wumpus();
		bool move_player(int target);
		void debug();	/// show the status of the cave for debug purpose
		std::vector<int> neighbour_rooms(int room);
		int current_room();

	};

	int get_random(int min, int max);
	static void swapRoom(Room &r1, Room &r2);
	//void shuffleRoom(std::vector<Room> &rooms);
	void hunt_wumpus();
	void init();
	int sel_room_to_move(Dungeon& d);
	std::vector<int> sel_room_to_shoot();

}

#endif