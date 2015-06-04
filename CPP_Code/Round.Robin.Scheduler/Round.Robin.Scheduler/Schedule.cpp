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
/* Computes how long a team waited for a COMPLETED WEEK */
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
/* Computes how long a team HAS BEEN WAITING 
 This is for building a week in progress.  It eliminates whether a team is eligible to
 play another game given how long it's waited
 
 Could be deprecated if a post-sort is implemented
 */
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
/* Computes how long each team has waited for a schedule */
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

int RRSchedule::DVectMin(DoubleVector _invect)
{
    int global_min = max_per_time;
    for (DoubleVector::const_iterator tslot = _invect.begin(); tslot != _invect.end(); ++tslot)
    {
        global_min = std::min(VectMin(*tslot), global_min);
        
    }
    return global_min;
}

int RRSchedule::DVectMax(DoubleVector _invect)
{
    int global_max = 0;
    for (DoubleVector::const_iterator tslot = _invect.begin(); tslot != _invect.end(); ++tslot)
    {
        global_max = std::max(VectMax(*tslot), global_max);
        
    }
    return global_max;
}

int RRSchedule::VectMin (Vector _invect)
{
    int min = *std::min_element(_invect.begin(), _invect.end());
    return min;
}

int RRSchedule::VectMax(Vector _invect)
{
    return *std::max_element(_invect.begin(), _invect.end());
}

double RRSchedule::total_played_scale_factor()
{
    // Return a factor indicating that the total number of games played per team is similar
    return std::max((VectMax(total_played) - VectMin(total_played)),1);
}

double RRSchedule::waiting_scale_factor()
{
    // Return a coefficient indicating the difference in how long some teams wait with respect to others
    return std::max((VectMax(total_waiting) - VectMin(total_waiting)),1);
}

double RRSchedule::timeslot_scale_factor()
{
    // Return a coefficient indicating whether teams play in the same timeslot or different ones over the course of the season
    int total_timeslot_discrepancy = 0;
    for (DoubleVector::const_iterator tslot = timeslots_played.begin(); tslot != timeslots_played.end(); ++tslot)
    {
        total_timeslot_discrepancy += VectMax(*tslot) - VectMin(*tslot);
    }
    return ((1.0*std::max(total_timeslot_discrepancy, 1)) / max_teams);
    //return 1.0;
}

void RRSchedule::scale_strength()
{
    //scale solution wait time
    
    scaled_total_wait_time =      10*total_wait_time
                                + 100* total_played_scale_factor()
                                + 5* waiting_scale_factor()
                                + 1* timeslot_scale_factor();
    
    /*scaled_total_wait_time = total_wait_time * std::max(100*(VectMax(total_played) - VectMin(total_played))
                                                        + 10*(VectMax(total_waiting) - VectMin(total_waiting)) //, 1);
                                                        + 1 *(DVectMax(timeslots_played) - DVectMin(timeslots_played)), 1);
    *///1 * (VectMax(timeslots_played) - VectMin(timeslots_played))
    scaled_per_team_wait_time = scaled_total_wait_time * 1.0 / (max_weeks * max_teams * 1.0);

}



bool RRSchedule::add_week()
{
    bool mincheck = (min_per_night <= VectMin(this_week_played));
    if (week0)
    {
        int temp_min_per_night = (max_courts * (max_times - skip_first) * 2) / max_teams;
        mincheck = temp_min_per_night <= VectMin(this_week_played);
    }
    
    bool noWait = false;
    if (mincheck)
    {
        //print_DoubleVector(timeslots);
        if (not week0)
        {
            noWait = sort_times(timeslots);
        }
        else
        {
            noWait = true;
        }
        //print_DoubleVector(timeslots);
        if (noWait)
        {
            weeks.push_back(timeslots);
            store_timeslot();
            
            // No longer first week after successful addition of timeslot
            week0 = false;
            
            allocate2D(timeslots, max_times, max_courts*2);
            init2D(this_week_matchups, max_teams, max_teams);
            
            total_this_week_played.push_back(this_week_played);
            
            init1D(this_week_played,max_teams);
            
            compute_strength();
            scale_strength();
            
            printf ("Current Size: %d weeks (Max Weeks: %d).  Total Wait Time: %d   Scaled Strength: %d\n", int(weeks.size()), max_weeks, total_wait_time, scaled_total_wait_time);
            
            std::cout << weeks.size() << std::endl;
            
            if (weeks.size() == max_weeks)
            {
                fullSolution = true;
            }
            else{
                printf ("Not full Solution\n");
                
            }
        }
    }
    return mincheck and noWait;
}

void RRSchedule::update_strength()
{
    compute_strength();
    for (int team = 0; team < max_teams; team++)
    {
        total_wait_time += week_strength(timeslots, team);
    }
}

void RRSchedule::count_timeslot(Vector _courts, int slot)
{
    for (Vector::const_iterator iter = _courts.begin(); iter != _courts.end(); ++iter)
    {
        if (*iter >= 0)
        {
            timeslots_played[*iter][slot] ++;
        }
    }
}

bool RRSchedule::add_timeslot()
{
    count_timeslot(courts, int(timeslots.size()));

    timeslots.push_back(courts);

    //print_DoubleVector(timeslots);
    allocate1D(courts, max_courts*2);
    
    if ((timeslots.size() == max_times) or ((weeks.size() == 0) and (timeslots.size() + skip_first == max_times)))
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

bool RRSchedule::themself(int home, int away)
{
    // Ensure the teams do not play themselves
    return home != away;
}

bool RRSchedule::elsewhere(int home, int away)
{
    // Ensure the teams are not playing elsewhere at the current time
    return (    not isPresent(courts, home)) and
                (not isPresent(courts, away)
            );
}

bool RRSchedule::that_night(int home, int away)
{
    int effective_max;
    
    if (week0)
        effective_max = w0_max_per_night;
    else
        effective_max = max_per_night;
    
    // Ensure the teams have not played too much already that night
    return (    (this_week_played[home] < effective_max) and
                (this_week_played[away] < effective_max)
                );
}

bool RRSchedule::overall(int home, int away)
{
    // Ensure the teams have not too many more times than another team
    int min_played = VectMin(total_played);
    return (    (total_played[home] - min_played <= MAX_PLAYED_GAP) and
                (total_played[away] - min_played <= MAX_PLAYED_GAP)
            );
}

bool RRSchedule::team_timeslot_check(int team)
// Deprecate due to sorting
{
    
    return true;
    
    /*
    // Ensure the team has not played too many times in this timeslot
    int this_time = int(timeslots.size());
    int team_count = timeslots_played[team][this_time];
    int all_min = VectMin(timeslots_played[team]);
    
    return (   (team_count - all_min <= MAX_TIMESLOT_GAP) and
                (team_count < max_per_time)
            );
    */
}

bool RRSchedule::balanced_time(int home, int away)
// Deprecated due to sorting
{
    // Ensure the teams have not played too frequently in the given timeslot
    return true;

    /*
     return (team_timeslot_check(home) and
            team_timeslot_check(away));
    */
    
}

bool RRSchedule::wait_time_check(int team)
// Deprecated due to sorting
{
    return true;
    // Ensure the team has not exceeded the max wait time and that it hasn't waited more than any other team
/*    return (    (wait_time(timeslots, team) <= MAX_WAIT_TIME)
                and (total_waiting[team] + wait_time(timeslots, team) <= VectMin(total_waiting) + MAX_WAIT_GAP)
            );
*/
}

bool RRSchedule::check_wait_time(int home, int away)
// Deprecated due to sorting
{
    // Ensure the team will not have waited too long
    return true;
    
    //return wait_time_check(home) and wait_time_check(away);
}

bool RRSchedule::teams_feasible(int home, int away)
/* Check to ensure:
1.  Not playing themself
2.  Not playing elsewhere
3.  Not played too much already that night
4.  Not played twice more than another team overall
5.  Not played too many times in that timeslot
6.  Will not force a wait of <x> hours
*/
{
    return  (
             themself(home, away)
             and elsewhere(home,away)
             and that_night(home,away)
             and overall(home,away)
             //and balanced_time(home,away)
             //and check_wait_time(home,away)
            );
}

bool RRSchedule::check_feasible(int home, int away)
/* Check to ensure:
 1. The teams are feasible
 2. The matchup is feasible
 3. The schedule is appropriately sized
*/
{
    
    return (
            teams_feasible(home,away)
            and matchup_feasible(home,away)
            and weeks.size() < max_weeks
            );
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
    
    if ((home >= 0) and (away >=0))
    {
        total_played[home]++;
        total_played[away]++;
    
        this_week_played[home] ++;
        this_week_played[away] ++;
    
        opponent_counts[home][away] ++;
        opponent_counts[away][home] ++;
    }
    
//    print_Vector(courts);
    
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

void RRSchedule::init2DEmpty(DoubleVector &_invect, int row, int col)
{
    _invect.clear();
    _invect.resize(row);
    for (int i = 0; i < row; i++)
        allocate1D(_invect[i], col);
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

void RRSchedule::compute_permutations()
{
    std::vector<int> new_permute;
    
    for (int lcv = 0; lcv < max_times; lcv++)
    {
        new_permute.push_back(lcv);
    }
    
    do
    {
        timePermutes.push_back(new_permute);
    }
    while (next_permutation(new_permute.begin(), new_permute.begin()+max_times));

    //    std::cout << timePermutes.size() << std::endl;
    //    print_DoubleVector(timePermutes);
}

int RRSchedule::compute_waits(DoubleVector _week)
/* Computes the waits per team and then returns a weighted score based on those waits */
{
    DoubleVector team_played_counts;
    Vector team_wait;
    
    init2DEmpty(team_played_counts, max_teams, max_per_night);
    
    int tslotNum = 0;
    for (DoubleVector::const_iterator _tslot = _week.begin(); _tslot != _week.end(); ++_tslot)
    {
        for (Vector::const_iterator team = _tslot->begin(); team != _tslot->end(); ++team)
        {
            //std::cout << *team << std::endl;
            team_played_counts[*team].push_back(tslotNum);
            //print_DoubleVector(team_played_counts);
        }
        tslotNum ++;
    }
    

    //print_DoubleVector(team_played_counts);
    for (DoubleVector::const_iterator _team = team_played_counts.begin(); _team != team_played_counts.end(); ++_team)
    {
        //print_Vector(*_team);
        
        //std::cout << VectMin(*_team) << VectMax(*_team) << _team->size() << std::endl;
        team_wait.push_back(VectMax(*_team) - VectMin(*_team) - int(_team->size()) + 1);
        
    }
    //print_Vector(team_wait);
    
    // Return longest wait_time * 10 + sum of wait times
//    return team_wait;
    return 100 * VectMax(team_wait)
            + 10 * (VectMax(team_wait) - 1) * int(std::count(team_wait.begin(), team_wait.end(), VectMax(team_wait)))
            + std::accumulate(team_wait.begin(), team_wait.end(), 0);
    
}

DoubleVector RRSchedule::reconfigureWeek(DoubleVector _week, Vector _permute)
{
    DoubleVector newWeek;
    //allocate2D(newWeek, max_times, max_games*2);
    
    for (Vector::const_iterator tSlotNum = _permute.begin(); tSlotNum != _permute.end(); ++tSlotNum)
    {
        newWeek.push_back(_week[*tSlotNum]);
    }
    
    return newWeek;
}

bool RRSchedule::sort_times(DoubleVector &_week)
{
    int minScore = max_teams * 100 * max_times;
    int score;
    Vector team_waits_now;
    DoubleVector bestWeek;
    Vector bestWaits;
    
    //print_DoubleVector(_week);

    // Loop through all permutations of timeslots
    for (DoubleVector::const_iterator permute = timePermutes.begin(); permute != timePermutes.end(); ++permute)
    {
        DoubleVector testWeek = reconfigureWeek(_week, *permute);
        score = compute_waits(testWeek);
        //team_waits_now = compute_waits(testWeek);
        //score = VectMax(team_waits_now) * 10 + std::accumulate(team_waits_now.begin(), team_waits_now.end(), 0);
        if (score < minScore)
        {
            bestWeek = testWeek;
            minScore = score;
            //bestWaits = team_waits_now;
//            print_DoubleVector(bestWeek);
//            print_Vector(*permute);
            //print_Vector(team_waits_now);
            std::cout << score << std::endl;
        }
    }
    
    //return (VectMax(bestWaits) <= min_per_night);

//    if( VectMax(bestWaits) <= 1)
//    {
        timeslots = bestWeek;
        return true;
//    }
//    else
//    {
//        return false;
//    }

}

RRSchedule::RRSchedule(int _max_weeks, int _max_times, int _max_courts, int _max_teams, char* _FILENAME, int SKIP_FIRST)
{
    max_weeks = _max_weeks;
    max_times = _max_times;
    max_courts = _max_courts;
    max_teams = _max_teams;
    skip_first = SKIP_FIRST;

    
    FILENAME = _FILENAME;
    
    week0 = (skip_first > 0);   // Check if special considerations need to be made for the first week (allowing a meeting)

    min_per_night = (max_courts * max_times * 2) / max_teams;
    w0_min_per_night = (max_courts * (max_times-skip_first) * 2) / max_teams;

    max_per_night = (max_courts * max_times * 2) / max_teams + ( ((max_courts * max_times * 2) % max_teams) != 0);
    w0_max_per_night = (max_courts * (max_times - skip_first) * 2) / max_teams + ( ((max_courts * (max_times - skip_first) * 2) % max_teams) != 0);
    
    MAX_PLAYED_GAP = 2; // <= Gap between min times played and max times played
    
    if (max_per_night > 2) then:
    {
        MAX_WAIT_TIME = max_per_night;   // <= Max time a team can wait each night
    }
    else
    {
        MAX_WAIT_TIME = min_per_night;
    }
    
    MAX_WAIT_GAP = max_weeks * MAX_WAIT_TIME;   // <= Gap between min team wait time and max team wait time
    
    MAX_TIMESLOT_GAP = max_weeks; // <=  Gap between count of min timeslot vs. max timeslot appearances
    TIMESLOT_FUDGE = 2*max_per_night; // Allow teams to play in a timeslot # over "ideal"
    max_per_time = 2*max_weeks; //(max_weeks * max_courts * 2) / max_teams + ( ((max_weeks * max_courts * 2) % max_teams) != 0) + TIMESLOT_FUDGE;

    
    fullSolution = false;
    total_wait_time = 0;
    
    init1D(this_week_played, max_teams);
    allocate1D(courts, max_courts*2);
    init1D(total_played, max_teams);
    init1D(total_waiting, max_teams);
    
    init2D(opponent_counts, max_teams, max_teams);
    allocate2D(timeslots, max_times, max_courts*2);
    allocate2D(matchups, max_weeks * max_times * max_courts, 2);
    init2D(this_week_matchups, (max_times-skip_first)*max_courts, 2);
    init2D(timeslots_played, max_teams, max_times);
    allocate2D(timePermutes, FACTS[max_times], max_times);
    compute_permutations();
    allocate2D(total_this_week_played,max_weeks, max_teams);
    
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
    if (skip_first > 0)
    {
        Vector oneSlot;
        DoubleVector fakeTimes;
        for (int nogame = 0; nogame < max_courts*2; nogame++)
        {
            oneSlot.push_back(-1);
        }
        for (int noTimes = 0; noTimes < skip_first; noTimes ++)
        {
            fakeTimes.push_back(oneSlot);
        }
        
        print_DoubleVector(fakeTimes);
        print_DoubleVector(fakeTimes, outputfile);
    }
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
    
    std::cout << '\n' << std::flush << "Weekly Play Counts:" << '\n' << std::flush;
    outputfile << '\n' << std::flush << "Weekly Play Counts:" << '\n' << std::flush;
    print_DoubleVector(total_this_week_played);
    print_DoubleVector(total_this_week_played, outputfile);
    
    
    std::cout << '\n' << std::flush << "Timeslot Counts:" << '\n' << std::flush;
    outputfile << '\n' << std::flush << "Timeslot Counts:" << '\n' << std::flush;
    print_DoubleVector(timeslots_played);
    print_DoubleVector(timeslots_played, outputfile);
    
    outputfile << "Hours Waiting: " << total_wait_time << '\n' << std::flush;
    outputfile << "Hours Per Team: " << per_team_wait_time << '\n' << std::flush;
    outputfile << "Scaled Strength: " << scaled_total_wait() << '\n' << std::flush;
    
    outputfile << "\nWaits Per Team: " << std::flush;
    std::cout << "\nWaits Per Team: \n" << std::flush;
    print_Vector(total_waiting);
    print_Vector(total_waiting, outputfile);
   
    
    outputfile.close();
}