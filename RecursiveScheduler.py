'''
Created on Jun 2, 2014

@author: coreywinton
'''

from scheduleClass import ScheduleClass

max_weeks = 9
max_times = 6
max_courts = 2
max_teams = 11

good_schedules = []     

mgs = [2*max_times]


def test_recursion(schedule, count):
    if count == 0:
        print schedule.total_played
    else:
        print count
        schedule.add_game(0,1)
        test_recursion(schedule, count - 1)

def find_schedule(schedule, home, away):
    if schedule.add_game_with_feas_check(home, away):
        if schedule.fullSolution:
                   
            if (schedule.total_wait_time < min(mgs)):
                good_schedules.append(schedule)
                mgs.append(schedule.total_wait_time)
                print
                print schedule.solution_strength(), schedule.weeks
            return schedule
        else:
            for h_iter in range(max_teams):
                for a_iter in range(max_teams):
                    new_schedule = ScheduleClass(old_schedule = schedule)
                    find_schedule(new_schedule, (home+h_iter)%max_teams, (away+a_iter)%max_teams)
                    
    
    
if __name__ == '__main__':
    schedule = ScheduleClass(max_weeks, max_times, max_courts, max_teams)
    
    find_schedule(schedule, 0,1)
    
    print mgs
    
#     for sched in good_schedules:
#        $ print sched.weeks
#         print sched.solution_strength()
        