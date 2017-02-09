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



int max_weeks = 1; //9
int max_times = 4;
int max_courts = 4;
int max_teams = 8;
int SKIP_FIRST = 0;  // Give the director first timeslot of the year as a "explanatory period" and don't put games there

int best_wait_time;
int best_fitness;

char FILENAME[] = "/Users/rditlcww/git/RoundRobinScheduling/CPP_Code/Round.Robin.Scheduler/Round.Robin.Scheduler/schedulesTest.txt";






int find_schedule(RRSchedule schedule, int home, int away, int counter)
{
    //counter ++;
    //printf("%d \n", counter);
    if (schedule.add_game_with_feas_check(home, away) and schedule.on_Track(best_wait_time, best_fitness))
    {
        if (schedule.full_solution())
        {
            std::cout << "Solution Found.  Score to beat: " << best_fitness << std::endl;
            if (schedule.eval_fitness_level() < best_fitness)
            {
                best_wait_time = schedule.total_wait();
                best_fitness = schedule.eval_fitness_level();
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
    srand(time(NULL));

    best_wait_time = max_weeks * max_times * max_courts*2;
    best_fitness = best_wait_time * 1000000;
    //best_wait_time = 128;
    std::cout << "HERE WE GO!!!! " << best_wait_time << std::endl;
    RRSchedule newSched(max_weeks, max_times, max_courts, max_teams, FILENAME, SKIP_FIRST);
    
    find_schedule(newSched, rand() % max_teams, rand() % max_teams, 0);

    
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

