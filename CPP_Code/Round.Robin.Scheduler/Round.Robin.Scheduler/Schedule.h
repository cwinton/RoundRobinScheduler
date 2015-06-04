//
//  Schedule.h
//  Round.Robin.Scheduler
//
//  Created by Corey Winton on 6/3/14.
//  Copyright (c) 2014 Corey Winton. All rights reserved.
//

#ifndef __Round_Robin_Scheduler__Schedule__
#define __Round_Robin_Scheduler__Schedule__

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <fstream>
#include <time.h>

typedef std::vector<int> Vector;
typedef std::vector<Vector> DoubleVector;
typedef std::vector<DoubleVector> TripleVector;

using std::vector;
static std::vector<int> FACTS = {1, 2, 6, 24, 120, 720, 5040, 40320};

#define DTTMF
class RRSchedule{
    char* FILENAME;
    
    int MAX_WAIT_TIME; // = 3;  // <= Max time a team can wait each night
    int MAX_PLAYED_GAP; // = 2; // <= Gap between min times played and max times played
    int MAX_WAIT_GAP; // = 2*MAX_WAIT_TIME;   // <= Gap between min team wait time and max team wait time
    int MAX_TIMESLOT_GAP; // = 2; // <=  Gap between count of min timeslot vs. max timeslot appearances
    int TIMESLOT_FUDGE; // = 2; // Allow teams to play in a timeslot # over "ideal"
    
    int max_weeks, max_times, max_courts, max_teams, max_per_time, skip_first;
    int max_per_night, min_per_night, w0_max_per_night, w0_min_per_night;
    bool fullSolution;
    bool week0;
    
    int total_wait_time, scaled_total_wait_time;
    double per_team_wait_time, scaled_per_team_wait_time;
    
    TripleVector weeks;

    DoubleVector timeslots;
    DoubleVector matchups;
    DoubleVector opponent_counts;
    DoubleVector this_week_matchups;
    DoubleVector total_this_week_played;
    DoubleVector timeslots_played;
    DoubleVector courts_played;
    DoubleVector timePermutes;
    
    Vector courts;
    Vector total_played;
    Vector this_week_played;
    Vector total_waiting;

    
    void allocate1D (Vector&, int);
    void allocate2D (DoubleVector & _invect, int row, int col);
    void allocate3D (TripleVector & _invect, int row, int col, int depth);
    
    void init1D(Vector& _invect, int);
    void init2D(DoubleVector& _invect, int, int);
    void init2DEmpty(DoubleVector &_invect, int row, int col);
    

    
    void print_Vector(Vector _invect, std::ostream&);
    void print_DoubleVector(DoubleVector _invect, std::ostream&);
    void print_TripleVector(TripleVector _invect, std::ostream&);

    
    bool isPresent(Vector, int);
    bool isPresent(DoubleVector, Vector);
    int week_strength(DoubleVector, int);
    int wait_time(DoubleVector week, int team);
    
    bool elsewhere(int, int);
    bool themself(int, int);
    bool that_night(int, int);
    bool overall(int, int);
    bool balanced_time(int, int);
    bool team_timeslot_check(int team);
    bool check_wait_time(int,int);
    bool wait_time_check(int);
    
    void compute_permutations();
    bool sort_times(DoubleVector &);
    DoubleVector reconfigureWeek(DoubleVector, Vector);
    int compute_waits(DoubleVector);

    void compute_strength();
    void update_strength();
    void scale_strength();
    double total_played_scale_factor();
    double waiting_scale_factor();
    double timeslot_scale_factor();

    
    void count_timeslot(Vector _courts, int slot);
    
    bool add_timeslot();
    bool add_week();
    void store_timeslot();
    
    bool check_feasible(int, int);
    bool teams_feasible(int, int);
    bool matchup_feasible(int, int);
    
    int VectMin(Vector);
    int VectMax(Vector);
    
    int DVectMin(DoubleVector);
    int DVectMax(DoubleVector);
    
public:
    RRSchedule(int max_weeks, int max_times, int max_courts, int max_teams, char* FILENAME, int SKIP_FIRST);
    bool add_game(int home, int away);
    bool add_game_with_feas_check(int,int);
    
    bool full_solution(){return fullSolution;}
    
    double wait_per_team(){return per_team_wait_time;}
    int total_wait(){return total_wait_time;}
    
    double scaled_wait_per_team(){return scaled_per_team_wait_time;}
    int scaled_total_wait(){return scaled_total_wait_time;}

    void print_schedule();
    
    
    int get_weeks() {return max_weeks; }
};

#endif /* defined(__Round_Robin_Scheduler__Schedule__) */
