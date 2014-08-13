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


typedef std::vector<int> Vector;
typedef std::vector<Vector> DoubleVector;
typedef std::vector<DoubleVector> TripleVector;

using std::vector;

class RRSchedule{
    const static int MAX_WAIT_TIME = 2;  // <=
    const static int MAX_PLAYED_GAP = 2; // <
    const static int MAX_WAIT_GAP = 2;   // <=
    
    char* FILENAME;
    
    int max_weeks, max_times, max_courts, max_teams;
    int max_per_night, min_per_night;
    bool fullSolution;
    
    int total_wait_time, scaled_total_wait_time;
    double per_team_wait_time, scaled_per_team_wait_time;
    
    TripleVector weeks;

    DoubleVector timeslots;
    DoubleVector matchups;
    DoubleVector opponent_counts;
    DoubleVector this_week_matchups;
    
    Vector courts;
    Vector total_played;
    Vector this_week_played;
    Vector total_waiting;

    
    void allocate1D (Vector&, int);
    void allocate2D (DoubleVector & _invect, int row, int col);
    void allocate3D (TripleVector & _invect, int row, int col, int depth);
    
    void init1D(Vector& _invect, int);
    void init2D(DoubleVector& _invect, int, int);
    

    
    void print_Vector(Vector _invect, std::ostream&);
    void print_DoubleVector(DoubleVector _invect, std::ostream&);
    void print_TripleVector(TripleVector _invect, std::ostream&);

    
    bool isPresent(Vector, int);
    bool isPresent(DoubleVector, Vector);
    int week_strength(DoubleVector, int);
    int wait_time(DoubleVector week, int team);

    void compute_strength();
    void update_strength();
    void scale_strength();
    
    
    bool add_timeslot();
    bool add_week();
    void store_timeslot();
    
    bool check_feasible(int, int);
    bool teams_feasible(int, int);
    bool matchup_feasible(int, int);
    
    int VectMin(Vector);
    int VectMax(Vector);
    
public:
    RRSchedule(int max_weeks, int max_times, int max_courts, int max_teams, char* FILENAME);
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
