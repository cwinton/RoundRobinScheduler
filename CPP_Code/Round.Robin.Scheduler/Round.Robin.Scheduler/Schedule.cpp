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
    stream << '\n' << std::flush;
}


void RRSchedule::print_DoubleVector(DoubleVector _invect, std::ostream &stream = std::cout)
{
    for (DoubleVector::const_iterator row = _invect.begin(); row != _invect.end(); ++row)
    {
        print_Vector(*row, stream);
        stream << '\n' << std::flush;
    }
}


void RRSchedule::print_TripleVector(TripleVector _invect, std::ostream &stream = std::cout)
{
    for (TripleVector::const_iterator row = _invect.begin(); row != _invect.end(); ++row)
    {
        print_DoubleVector(*row, stream);
        stream << '\n' << std::flush;
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
        return 0;
        //return max_times;
    }
        
}

int RRSchedule::wait_time(DoubleVector week, int team)
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
        
        // waiting time = total time - time of first play - number of times played
        int wait_time = int(week.size()) - (firstplay + playtimes);
        return wait_time;
    }
    
    else
    {
        return 0;
    }

}

void RRSchedule::compute_strength()
{
    total_wait_time = 0;
    init1D(total_waiting, max_teams);
    
    int teamWait;
    for (TripleVector::const_iterator week = weeks.begin(); week != weeks.end(); ++ week)
    {
        for (int team = 0; team < max_teams; team++)
        {
            teamWait = week_strength(*week, team);
            total_wait_time += teamWait;
            total_waiting[team] += teamWait;
        }
    }
    
    // Scale by total wait time discrepencies
    //total_wait_time = total_wait_time * std::max((VectMax(total_played) - VectMin(total_played)),1);
    
    per_team_wait_time = total_wait_time * 1.0 / (max_weeks * max_teams * 1.0);

}

int RRSchedule::VectMin (Vector _invect)
{
    return *std::min_element(_invect.begin(), _invect.end());
}

int RRSchedule::VectMax(Vector _invect)
{
    return *std::max_element(_invect.begin(), _invect.end());
}

bool RRSchedule::add_week()
{
    bool mincheck = (min_per_night <= VectMin(this_week_played));
    
    if (mincheck)
    {
        weeks.push_back(timeslots);
        store_timeslot();

        allocate2D(timeslots, max_times, max_courts*2);
        init2D(this_week_matchups, max_teams, max_teams);
        
        

        init1D(this_week_played,max_teams);
        
        compute_strength();
        
        printf ("Current Size: %d weeks.  Total Wait Time: %d \n", int(weeks.size()), total_wait_time);

        if (weeks.size() == max_weeks)
        {
            //scale solution wait time
            scaled_total_wait_time = total_wait_time * (10*std::max(VectMax(total_played) - VectMin(total_played),1) + std::max(VectMax(total_waiting) - VectMin(total_waiting), 1));
            scaled_per_team_wait_time = scaled_total_wait_time * 1.0 / (max_weeks * max_teams * 1.0);
            fullSolution = true;
        }
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
    bool freq = false;
    
    if (this_week)
    {
        int min_played_home, min_played_away;;
        
        Vector home_counts (opponent_counts[home]);
        home_counts.erase(home_counts.begin() + home);
        min_played_home = VectMin(home_counts);
        
        Vector away_counts(opponent_counts[away]);
        away_counts.erase(away_counts.begin() + away);
        min_played_away = VectMin(away_counts);
        
        freq = ((opponent_counts[home][away] - min_played_home) == 0) and ((opponent_counts[away][home] - min_played_away) == 0);
        
        if (freq)
        {
            // Only return true if all tests pass
            return true;
        }
    }
    
    // Otherwise return false
    return false;
}

bool RRSchedule::teams_feasible(int home, int away)
/* Check to ensure:
1.  Not playing themself
2.  Not playing elsewhere
3.  Not played too much already that night
4.  Not played twice more than another team overall
5.  Will not force a wait of <x> hours
*/
{
    bool themself = false;
    bool elsewhere = false;
    bool that_night = false;
    bool overall = false;

    // Nesting in if-then was stupid
    // Eager evaluation
/*    if (int(weeks.size()) > 0)
    {
        printf("\n");
    }
 */
    themself = (home != away);
    if (themself)
    {
        elsewhere = (not isPresent(courts, home)) and (not isPresent(courts, away));
        if (elsewhere)
        {
            
            that_night = (this_week_played[home] < max_per_night) and (this_week_played[away] < max_per_night);
            if (that_night)
            {
                int min_played = VectMin(total_played);
                overall = (total_played[home] - min_played < MAX_PLAYED_GAP) and (total_played[away] - min_played < MAX_PLAYED_GAP);
                if (overall)
                {

                    
                    int home_wait_time = wait_time(timeslots, home);
                    bool home_wait = (home_wait_time <= MAX_WAIT_TIME) and (total_waiting[home] + home_wait_time <= VectMin(total_waiting) + MAX_WAIT_GAP);
                    
                    if (home_wait)
                    {
                        int away_wait_time = wait_time(timeslots, away);
                        bool away_wait = (away_wait_time <= MAX_WAIT_TIME) and (total_waiting[away] + away_wait_time <= VectMin(total_waiting) + MAX_WAIT_GAP);
                        
                        return away_wait;
                        
                    }
                }
            }
        }
    }
    
    
    
    // Otherwise return false
    return false;
}

bool RRSchedule::check_feasible(int home, int away)
{
    if (weeks.size() < max_weeks)
    {
        if (teams_feasible(home, away))
        {
            if (matchup_feasible(home, away))
            {
                return true;
            }
        }
    }
    return false;
//    return (teams_feasible(home,away) and matchup_feasible(home,away) and weeks.size() < max_weeks);
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

RRSchedule::RRSchedule(int _max_weeks, int _max_times, int _max_courts, int _max_teams, char* _FILENAME)
{
    max_weeks = _max_weeks;
    max_times = _max_times;
    max_courts = _max_courts;
    max_teams = _max_teams;
    
    FILENAME = _FILENAME;
    
    min_per_night = (max_courts * max_times * 2) / max_teams;
    // Relax the min per night requirement?
    if (min_per_night > 1)
    {
        min_per_night--;
    }
    max_per_night = (max_courts * max_times * 2) / max_teams + ( ((max_courts * max_times * 2) % max_teams) != 0);
    
    fullSolution = false;
    total_wait_time = 0;
    
    init1D(this_week_played, max_teams);
    allocate1D(courts, max_courts*2);
    init1D(total_played, max_teams);
    init1D(total_waiting, max_teams);
    
    init2D(opponent_counts, max_teams, max_teams);
    allocate2D(timeslots, max_times, max_courts*2);
    allocate2D(matchups, max_weeks * max_times * max_courts, 2);
    init2D(this_week_matchups, max_times*max_courts, 2);
    
    allocate3D(weeks, max_weeks, max_times, max_courts*2);
    
    // clear old file
    std::ofstream outputfile;
    outputfile.open(FILENAME);
    outputfile.close();
}

void RRSchedule::print_schedule()
{
    
    std::ofstream outputfile;
    outputfile.open (FILENAME, std::ios::app);
    
    outputfile << "\n\n***NEW SCHEDULE***\n\n" << std::flush;
    outputfile << max_weeks << '\n' << std::flush;
    outputfile << max_times << '\n' << std::flush;
    outputfile << max_courts << '\n' << std::flush;
    outputfile << max_teams << '\n' << std::flush;
    
    
    std::cout << "Weeks:" << '\n' << std::flush;
    outputfile << "Weeks:" << '\n' << std::flush;
    print_TripleVector(weeks);
    print_TripleVector(weeks, outputfile);
    
    std::cout << "TimeSlots:" << '\n' << std::flush;
    print_DoubleVector(timeslots);
    
    std::cout << "Courts:" << '\n' << std::flush;
    print_Vector(courts);
    
    
    std::cout << '\n' << std::flush << "Play Counts:" << '\n' << std::flush;
    outputfile << '\n' << std::flush << "Play Counts:" << '\n' << std::flush;
    print_Vector(total_played);
    print_Vector(total_played, outputfile);
    
    std::cout << '\n' << std::flush << "Matchup Counts:" << '\n' << std::flush;
    outputfile << '\n' << std::flush << "Matchup Counts:" << '\n' << std::flush;
    print_DoubleVector(opponent_counts);
    print_DoubleVector(opponent_counts, outputfile);
    
    outputfile << "Hours Waiting: " << total_wait_time << '\n' << std::flush;
    outputfile << "Hours Per Team: " << per_team_wait_time << '\n' << std::flush;
    
    outputfile << "\nWaits Per Team: " << std::flush;
    std::cout << "\nWaits Per Team: \n" << std::flush;
    print_Vector(total_waiting);
    print_Vector(total_waiting, outputfile);
   
    
    outputfile.close();
}