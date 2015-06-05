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
    
    int total_wait_time, fitness_level;
    double per_team_wait_time, scaled_fitness_level;
    
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
    Vector total_waiting_by_team;

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///// ********** START TOOLS ///////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    void allocate1D (Vector&, int);
    void allocate2D (DoubleVector & _invect, int row, int col);
    void allocate3D (TripleVector & _invect, int row, int col, int depth);
    
    void init1D(Vector& _invect, int);
    void init2D(DoubleVector& _invect, int, int);
    void init2DEmpty(DoubleVector &_invect, int row, int col);
    
    void print_Vector(Vector _invect, std::ostream&);
    void print_DoubleVector(DoubleVector _invect, std::ostream&);
    void print_TripleVector(TripleVector _invect, std::ostream&);

    int VectMin(Vector);
    int VectMax(Vector);
    
    int DVectMin(DoubleVector);
    int DVectMax(DoubleVector);
    
    bool isPresent(Vector, int);
    bool isPresent(DoubleVector, Vector);
    
    void compute_permutations();
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///// ********** END TOOLS /////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///// ********** EVALUATIVE METHODS ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // DEPRECATE DUE TO SORTING
    //      int wait_time(DoubleVector week, int team);
    Vector compute_team_waits_for_week(DoubleVector);       // Compute how long each team will wait in a week
    void compute_total_team_waits();                        // Compute how long each team has waited (total_wait_time; per_team_wait_time)
    void update_total_team_waits();                         // Update wait times    (total_wait_time; per_team_wait_time)
    double total_played_scale_factor();                     // Return difference in total game played max/min
    double waiting_scale_factor();                          // Return team wait difference
    double timeslot_scale_factor();                         // Return frequency of appearence in timeslots
    void compute_fitness();                                 // Determine fitness of schedule based on wait time & others
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///// ********** END EVALUATIVE METHODS ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///// ********** SORTING ALGORITHMS ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    int compute_week_fitness(DoubleVector);                 // Compute fitness of timeslot sort
    DoubleVector reconfigureWeek(DoubleVector, Vector);     // Permutes the week's timeslots
    bool sort_times(DoubleVector &);                        // Sorts and selects "best" configuration
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///// ********** END SORTING ALGORITHMS ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///// ********** "ROLLUP" METHODS //////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    void count_timeslot(Vector _courts, int slot);      // Log which teams played in each timeslot
    void store_timeslot();      // Log the information in the timeslots when adding to week
    bool add_week();            // Adds a full set of timeslots to the week (after sorting)
    bool add_timeslot();        // Adds a full set of courts to a timeslot
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///// ********** END "ROLLUP" METHODS //////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///// ********** TEAM BASED  ///////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // DEPRECATED DUE TO SORTING:
    //    bool balanced_time(int, int);
    //    bool team_timeslot_check(int team);
    //    bool check_wait_time(int,int);
    //    bool wait_time_check(int);
    bool elsewhere(int, int);       // Playing elsewhere?
    bool themself(int, int);        // Playing themselves?
    bool that_night(int, int);      // Played too many times that night?
    bool overall(int, int);         // Played too many times relative to others?
    
    bool teams_feasible(int, int);  // Are the teams eligible to play
    bool matchup_feasible(int, int);    // Is the matchup eligible to happen
    bool check_feasible(int, int);  // Is the entire proposition eligible
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///// ********** END TEAM BASED/////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///// ********** PUBLIC METHODS ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    RRSchedule(int max_weeks, int max_times, int max_courts, int max_teams, char* FILENAME, int SKIP_FIRST);
    bool add_game(int home, int away);
    bool add_game_with_feas_check(int,int);
    
    bool full_solution(){return fullSolution;}
    
    double wait_per_team(){return per_team_wait_time;}
    int total_wait(){return total_wait_time;}
    
    double eval_scaled_fitness_level(){return scaled_fitness_level;}
    int eval_fitness_level(){return fitness_level;}

    void print_schedule();
    
    
    int get_weeks() {return max_weeks; }
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///// ********** END PUBLIC METHODS ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
};

#endif /* defined(__Round_Robin_Scheduler__Schedule__) */
