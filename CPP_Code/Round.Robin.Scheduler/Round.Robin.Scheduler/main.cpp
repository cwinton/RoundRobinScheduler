//
//  main.cpp
//  Round.Robin.Scheduler
//
//  Created by Corey Winton on 6/3/14.
//  Copyright (c) 2014 Corey Winton. All rights reserved.
//

#include <iostream>
#include "defs.h"
#include "Schedule.h"




int best_wait_time;
int max_weeks = 9;
int max_times = 6;
int max_courts = 2;
int max_teams = 20;




int find_schedule(RRSchedule schedule, int home, int away)
{
    if (schedule.add_game_with_feas_check(home, away) and schedule.total_wait() < best_wait_time)
    {
        if (schedule.full_solution())
        {
            best_wait_time = schedule.total_wait();
            std::cout << "***************************" << std::endl;
            std::cout << "Time Slots Waiting: " << best_wait_time << "(" << schedule.wait_per_team() << " per team)" << std::endl;
            schedule.print_schedule();
            std::cout << "Time Slots Waiting: " << best_wait_time << "(" << schedule.wait_per_team() << " per team)" << std::endl;
            std::cout << "***************************" << std::endl;
            
            return best_wait_time;
        }
        else
        {
            #pragma omp parallel for
            for (int h_iter = 0; h_iter < max_teams; h_iter++)
                for (int a_iter = 0; a_iter < max_teams; a_iter++)
                {
                    find_schedule(schedule, (home+h_iter)%max_teams, (away+a_iter)%max_teams);
                }
        }
    }
    
    return 0;
}

int main(int argc, const char * argv[])
{

    



    best_wait_time = max_weeks * max_times * max_courts*2;
    //best_wait_time = 128;
    std::cout << "HERE WE GO!!!! " << best_wait_time << std::endl;
    RRSchedule newSched(max_weeks, max_times, max_courts, max_teams);
    
    
#if defined(_OPENMP)
#pragma omp parallel
    // >> ~/homebrew/bin/g++-4.9 -o rrscheduler -fopenmp main.cpp Schedule.cpp defs.cpp
    // >> ./rrscheduler
    {
        int home, away;
        home = omp_get_thread_num() %max_teams;
        away = (home+ rand()) % max_teams;
        printf("[%d] Home: %d, Away: %d\n", omp_get_thread_num(), home, away);
        find_schedule(newSched, home, away);
    }
#else
    printf("Running Serial\n");
    find_schedule(newSched, 0, 1);
#endif

    
/*    for (int home = 0; home < 500; home ++)
    {
        for (int away = 0; away < 500; away ++)
        {
            newSched.add_game_with_feas_check((home)%max_teams,(away)%max_teams);
        }
    }

    
    newSched.print_schedule();
    newSched.compute_strength();
 */
    return 0;
}

