//
//  Team.cpp
//  Round.Robin.Scheduler
//
//  Created by Corey Winton on 6/27/14.
//  Copyright (c) 2014 Corey Winton. All rights reserved.
//

#include "Team.h"
#include "Game.h"

void Team::add_timeslot()
{
    /* The timeslot was added successfully.  Therefore update stats from the timeslot */
    
    if (currently_playing)
    {
        games_played_week++;
        
    }
    
    timeslots_played.push_back(int(currently_playing));
    
    currently_playing = false;
    
}

void Team::add_game(int opponent)
{
    /* Add a matchup against an opponent */
    currently_playing = true;
    opponent_list.push_back(opponent);
    opponent_counts[opponent]++;
    
    /* If the previous count for that opponent was the min
     Check what the new min is */
    if (opponent_counts[opponent] - 1 == min_opp_count)
    {
        min_opp_count = VectMin(opponent_counts);
    }
    
}

Team::Team(int _num_teams, int _num_times, string _name = "")
{
    currently_playing = false;
    games_played_total = 0;
    games_played_week = 0;
    min_opp_count = 0;
    name = _name;
    init1D(opponent_counts, _num_teams);
    allocate1D(opponent_list, 0);
    this_week_wait = 0;
    init1D(timeslots_played, _num_times);
    total_wait_time = 0;
}

Team::Team()
{
    currently_playing = false;
    games_played_total = 0;
    games_played_week = 0;
    min_opp_count = 0;
    allocate1D(opponent_list, 0);
    this_week_wait = 0;
    total_wait_time = 0;
   
}