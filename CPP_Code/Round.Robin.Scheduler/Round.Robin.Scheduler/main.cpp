//
//  main.cpp
//  Round.Robin.Scheduler
//
//  Created by Corey Winton on 6/3/14.
//  Copyright (c) 2014 Corey Winton. All rights reserved.

// >> ~/homebrew/bin/g++-4.9 -o rrscheduler -fopenmp main.cpp Schedule.cpp
// >> ./rrscheduler
//

#include <iostream>
#include "Schedule.h"



int max_weeks = 9;
int max_times = 6;
int max_courts = 2;
int max_teams = 11;

int best_wait_time;
int scaled_best_wait_time;

char FILENAME[] = "/Users/rditlcww/git/RoundRobinScheduling/CPP_Code/Round.Robin.Scheduler/Round.Robin.Scheduler/schedulesTest.txt";






int find_schedule(RRSchedule schedule, int home, int away, int counter)
{
    //counter ++;
    //printf("%d \n", counter);
    if (schedule.add_game_with_feas_check(home, away) and schedule.total_wait() < best_wait_time)
    {
        if (schedule.full_solution())
        {
            std::cout << "Solution Found" << std::endl;
            if (schedule.scaled_total_wait() < scaled_best_wait_time)
            {
                best_wait_time = schedule.total_wait();
                scaled_best_wait_time = schedule.scaled_total_wait();
/*                printf("*****************************");
                printf("Time Slots Waiting: %d, (%f per team)", best_wait_time, schedule.wait_per_team());
                schedule.print_schedule();
                printf("Time Slots Waiting: %d, (%f per team)", best_wait_time, schedule.wait_per_team());
                printf("*****************************");
*/
                std::cout << "***************************" << std::endl;
                std::cout << "Time Slots Waiting: " << best_wait_time << "(" << schedule.wait_per_team() << " per team)" << std::endl;
                schedule.print_schedule();
                std::cout << "Time Slots Waiting: " << best_wait_time << "(" << schedule.wait_per_team() << " per team)" << std::endl;
                std::cout << "***************************" << std::endl;
                
                return best_wait_time;
            }
        }
        else
        {
            #pragma omp parallel for
            for (int h_iter = 0; h_iter < max_teams; h_iter++)
                for (int a_iter = 0; a_iter < max_teams; a_iter++)
                {
                    find_schedule(schedule, (home+h_iter)%max_teams, (away+a_iter)%max_teams, counter);
                }
        }
    }
    
    return 0;
}

int main(int argc, const char * argv[])
{

    
#pragma omp parallel
    {
        // >> ~/homebrew/bin/g++-4.9 -std=c++11 -o rrscheduler -fopenmp main.cpp Schedule.cpp
        // >> ./rrscheduler
    printf("OMP: Hello World \n");
    }


    best_wait_time = max_weeks * max_times * max_courts*2;
    scaled_best_wait_time = best_wait_time * 100000;
    //best_wait_time = 128;
    std::cout << "HERE WE GO!!!! " << best_wait_time << std::endl;
    RRSchedule newSched(max_weeks, max_times, max_courts, max_teams, FILENAME);
    
    find_schedule(newSched, 0, 1, 0);

    
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

