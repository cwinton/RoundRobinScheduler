//
//  Schedule.cpp
//  Round.Robin.Scheduler
//
//  Created by Corey Winton on 6/3/14.
//  Copyright (c) 2014 Corey Winton. All rights reserved.
//

#include "Schedule.h"

/*
template <typename T>
T **AllocateTwoDArray( int nRows, int nCols)
{
    T **dynamicArray;
    
    dynamicArray = new T*[nRows];
    for( int i = 0 ; i < nRows ; i++ )
        dynamicArray[i] = new T [nCols];
    
    return dynamicArray;
}

template <typename T>
void FreeTwoDArray(T** dArray)
{
    delete [] *dArray;
    delete [] dArray;
}

template<typename T>
T ***AllocateThreeDArray( int nRows, int nCols, int depth)
{
    T ***dynamicArray;
    
    dynamicArray = new T**[nrows];
    for (int i = 0; i < nRows; i++)
        dynamicArray[i] = AllocateTwoDArray<T>(nCols, depth)
    
    return dynamicArray
}

template<typename T>
void FreeThreeDArray(T*** dArray)
{
    
}
 */

void RRSchedule::print_Vector(Vector _invect, std::ostream &stream = std::cout)
{
    for (Vector::const_iterator iter = _invect.begin(); iter != _invect.end(); ++iter)
    {
        stream << *iter << ",";
    }
    stream << std::endl;
}


void RRSchedule::print_DoubleVector(DoubleVector _invect, std::ostream &stream = std::cout)
{
    for (DoubleVector::const_iterator row = _invect.begin(); row != _invect.end(); ++row)
    {
        print_Vector(*row, stream);
        stream << std::endl;
    }
}


void RRSchedule::print_TripleVector(TripleVector _invect, std::ostream &stream = std::cout)
{
    for (TripleVector::const_iterator row = _invect.begin(); row != _invect.end(); ++row)
    {
        print_DoubleVector(*row, stream);
        stream << std::endl;
    }
}

void RRSchedule::store_timeslot()
{
    for (DoubleVector::const_iterator row = this_week_matchups.begin(); row != this_week_matchups.end(); ++row)
    {
        matchups.push_back(*row);
    }
}

bool RRSchedule::isPresent(Vector _vect, int item)
{
    return (std::find(_vect.begin(), _vect.end(), item) != _vect.end());
}

bool RRSchedule::isPresent(DoubleVector _vect, Vector item)
{
    return (std::find(_vect.begin(), _vect.end(), item) != _vect.end());
}

int RRSchedule::week_strength(DoubleVector week, int team)
{
    Vector playcount;
    for (DoubleVector::const_iterator tslot = week.begin(); tslot != week.end(); ++tslot)
    {
        playcount.push_back(isPresent(*tslot, team));
    }
    
    int playtimes = std::accumulate(playcount.begin(), playcount.end(), 0);
    
    if (playtimes > 0)
    {
        int firstplay = int(std::find(playcount.begin(), playcount.end(), 1) - playcount.begin());
        
        int lastplay = int(playcount.rend() - std::find(playcount.rbegin(), playcount.rend(), 1)) - 1;
        
        return lastplay - firstplay - playtimes + 1;
    }
    else
    {
        return max_times;
    }
        
}

void RRSchedule::compute_strength()
{
    total_wait_time = 0;
    for (TripleVector::const_iterator week = weeks.begin(); week != weeks.end(); ++ week)
    {
        for (int team = 0; team < max_teams; team++)
        {
            total_wait_time += week_strength(*week, team);
        }
    }
    per_team_wait_time = total_wait_time * 1.0 / (max_weeks * max_teams * 1.0);

}

int RRSchedule::VectMin (Vector _invect)
{
    return *std::min_element(_invect.begin(), _invect.end());
}

bool RRSchedule::add_week()
{
    weeks.push_back(timeslots);
    store_timeslot();

    allocate2D(timeslots, max_times, max_courts*2);
    init2D(this_week_matchups, max_teams, max_teams);
    
    bool mincheck = (min_per_night <= VectMin(this_week_played));

    init1D(this_week_played,max_teams);
    
    compute_strength();

    if (weeks.size() == max_weeks)
    {
       fullSolution = true;
    }
    
    return mincheck;
}

void RRSchedule::update_strength()
{
    compute_strength();
    for (int team = 0; team < max_teams; team++)
    {
        total_wait_time += week_strength(timeslots, team);
    }
}

bool RRSchedule::add_timeslot()
{
    timeslots.push_back(courts);
    
    allocate1D(courts, max_courts*2);
    
    if (timeslots.size() == max_times)
        return add_week();
    else
        update_strength();
        return true;
}


bool RRSchedule::matchup_feasible(int home, int away)
{
/*  Check to ensure:
1.  Matchup is not scheduled for this week
2.  Teams have not repeatedly played each other without playing other teams first
*/
    Vector hVec, aVec;
    hVec.push_back(home); hVec.push_back(away);
    aVec.push_back(away); aVec.push_back(home);
    bool this_week = (not isPresent(this_week_matchups, hVec)) and (not isPresent(this_week_matchups, aVec));
    
    int min_played_home, min_played_away;;
    
    Vector home_counts (opponent_counts[home]);
    home_counts.erase(home_counts.begin() + home);
    min_played_home = VectMin(home_counts);
    
    Vector away_counts(opponent_counts[away]);
    away_counts.erase(away_counts.begin() + away);
    min_played_away = VectMin(away_counts);
    
    bool freq = ((opponent_counts[home][away] - min_played_home) == 0) and ((opponent_counts[away][home] - min_played_away) == 0);

    return this_week and freq;
}

bool RRSchedule::teams_feasible(int home, int away)
/* Check to ensure:
1.  Not playing elsewhere
2.  Not playing themself
3.  Not played too much already that night
4.  Not played twice more than another team overall
*/
{
    bool elsewhere = (not isPresent(courts, home)) and (not isPresent(courts, away));
    bool themself = (home != away);
    
    bool that_night = (this_week_played[home] < max_per_night) and (this_week_played[away] < max_per_night);
    
    int min_played = VectMin(total_played);
    
    bool overall = (total_played[home] - min_played < 2) and (total_played[away] - min_played < 2);
    
    return elsewhere and themself and that_night and overall;
}

bool RRSchedule::check_feasible(int home, int away)
{
    return (teams_feasible(home,away) and matchup_feasible(home,away) and weeks.size() < max_weeks);
}

bool RRSchedule::add_game_with_feas_check(int home, int away)
{
    if (check_feasible(home, away))
        return add_game(home, away);
    else
        return false;
}

bool RRSchedule::add_game(int home, int away)
{
    Vector mu;
    mu.push_back(home);
    mu.push_back(away);
    
    this_week_matchups.push_back(mu);
    
    courts.push_back(home);
    courts.push_back(away);
    
    total_played[home]++;
    total_played[away]++;
    
    this_week_played[home] ++;
    this_week_played[away] ++;
    
    opponent_counts[home][away] ++;
    opponent_counts[away][home] ++;
    
    if (courts.size() == max_courts * 2)
        return add_timeslot();
    else
        return true;
}

void RRSchedule::allocate1D(Vector& _invect, int dim)
{
    _invect.clear();
    _invect.reserve(dim);
}

void RRSchedule::init1D(Vector& _invect, int row)
{
    _invect.clear();
    _invect.resize(row);
    std::fill(_invect.begin(), _invect.end(), 0);
}

void RRSchedule::init2D(DoubleVector& _invect, int row, int col)
{
    _invect.clear();
    _invect.resize(row);
    for (int i = 0; i < row; i++)
        init1D(_invect[i], col);
}

void RRSchedule::allocate2D(DoubleVector& _invect, int row, int col)
{
    _invect.clear();
    _invect.reserve(row);

}

void RRSchedule::allocate3D(TripleVector & _invect, int row, int col, int depth)
{
    _invect.clear();
    _invect.reserve(row);
    for (int i = 0; i < row; i++)
    {
        allocate2D(_invect[i], col, depth);
    }
}

RRSchedule::RRSchedule(int _max_weeks, int _max_times, int _max_courts, int _max_teams)
{
    max_weeks = _max_weeks;
    max_times = _max_times;
    max_courts = _max_courts;
    max_teams = _max_teams;
    
    min_per_night = (max_courts * max_times * 2) / max_teams;
    max_per_night = (max_courts * max_times * 2) / max_teams + ( ((max_courts * max_times * 2) % max_teams) != 0);
    
    fullSolution = false;
    total_wait_time = 0;
    
    init1D(this_week_played, max_teams);
    allocate1D(courts, max_courts*2);
    init1D(total_played, max_teams);
    
    init2D(opponent_counts, max_teams, max_teams);
    allocate2D(timeslots, max_times, max_courts*2);
    allocate2D(matchups, max_weeks * max_times * max_courts, 2);
    init2D(this_week_matchups, max_times*max_courts, 2);
    
    allocate3D(weeks, max_weeks, max_times, max_courts*2);
}

void RRSchedule::print_schedule()
{
    
    std::ofstream outputfile;
    outputfile.open ("/Users/coreywinton/git/RoundRobinScheduling/CPP_Code/Round.Robin.Scheduler/Round.Robin.Scheduler/schedules.txt");
    outputfile << max_weeks << std::endl;
    outputfile << max_times << std::endl;
    outputfile << max_courts << std::endl;
    outputfile << max_teams << std::endl;
    
    
    std::cout << "Weeks:" << std::endl;
    outputfile << "Weeks:" << std::endl;
    print_TripleVector(weeks);
    print_TripleVector(weeks, outputfile);
    
    std::cout << "TimeSlots:" << std::endl;
    print_DoubleVector(timeslots);
    
    std::cout << "Courts:" << std::endl;
    print_Vector(courts);
    
    
    std::cout << std::endl << "Play Counts:" << std::endl;
    outputfile << std::endl << "Play Counts:" << std::endl;
    print_Vector(total_played);
    print_Vector(total_played, outputfile);
    
    std::cout << std::endl << "Matchup Counts:" << std::endl;
    outputfile << std::endl << "Matchup Counts:" << std::endl;
    print_DoubleVector(opponent_counts);
    print_DoubleVector(opponent_counts, outputfile);
    
    outputfile << "Hours Waiting: " << total_wait_time << std::endl;
    outputfile << "Hours Per Team: " << per_team_wait_time << std::endl;
    
    outputfile.close();
}