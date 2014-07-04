//
//  Team.h
//  Round.Robin.Scheduler
//
//  Created by Corey Winton on 6/27/14.
//  Copyright (c) 2014 Corey Winton. All rights reserved.
//

#ifndef __Round_Robin_Scheduler__Team__
#define __Round_Robin_Scheduler__Team__

#include "defs.h"


using namespace std;

class Game;

class Team{
    string name;                // Name of team

    int total_wait_time;        // How much time the team has spent waiting between matches
    int games_played_total;     // How many games the team has played
    int games_played_week;      // How many game the team has played this week
    int this_week_wait;         // How long waiting this week
    
    int min_opp_count;          // Minimum times played another team
    
    bool currently_playing;     // Storage for a team is currently playing in a time slot
    
    Vector opponent_counts;     // How many times the team has played each other team
    Vector opponent_list;       // This week's matchups
    Vector timeslots_played;    // 1 if playing in a timeslot; 0 otherwise
    
    std::vector<Game*> games_played; // Store all the games this team participates in
    
    
    
public:
    void set_name(string _name){name = _name;}
    string get_name(){return name;}
    
    void add_timeslot();
    void add_game(int opponent);
    
    Team(int _num_teams, int _num_times, string _name);
    Team();
    
    
};

#endif /* defined(__Round_Robin_Scheduler__Team__) */
