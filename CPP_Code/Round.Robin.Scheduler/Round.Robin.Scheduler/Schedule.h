//
//  Schedule.h
//  Round.Robin.Scheduler
//
//  Created by Corey Winton on 6/3/14.
//  Copyright (c) 2014 Corey Winton. All rights reserved.
//

#ifndef __Round_Robin_Scheduler__Schedule__
#define __Round_Robin_Scheduler__Schedule__

#include "defs.h"


class RRSchedule{
    int max_weeks, max_times, max_courts, max_teams;
    int max_per_night, min_per_night;
    bool fullSolution;
    
    int total_wait_time;
    double per_team_wait_time;
    
    TripleVector weeks;

    DoubleVector timeslots;
    DoubleVector matchups;
    DoubleVector opponent_counts;
    DoubleVector this_week_matchups;
    
    Vector courts;
    Vector total_played;
    Vector this_week_played;


    
    bool isPresent(Vector, int);
    bool isPresent(DoubleVector, Vector);
    int week_strength(DoubleVector, int);
    void compute_strength();
    void update_strength();
    
    
    bool add_timeslot();
    bool add_week();
    void store_timeslot();
    
    bool check_feasible(int, int);
    bool teams_feasible(int, int);
    bool matchup_feasible(int, int);

    
public:
    RRSchedule(int max_weeks, int max_times, int max_courts, int max_teams);
    bool add_game(int home, int away);
    bool add_game_with_feas_check(int,int);
    
    bool full_solution(){return fullSolution;}
    
    double wait_per_team(){return per_team_wait_time;}
    double total_wait(){return total_wait_time;}
    
    void print_schedule();
    
    int get_weeks() {return max_weeks; }
};

#endif /* defined(__Round_Robin_Scheduler__Schedule__) */
