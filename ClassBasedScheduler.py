'''
Created on Jun 2, 2014

@author: coreywinton
'''
from scheduleClass import ScheduleClass

def find_home_away_pair (schedule, home_start, away_start):
    for home_iter in range(max_teams):
        home = (home_start + home_iter) % max_teams
        for away_iter in range(max_teams):
            away = (away_start + away_iter) % max_teams
            
            if schedule.check_feasible(home, away):
                return home,away

if __name__ == '__main__':
    max_weeks = 9
    max_times = 6
    max_courts = 2
    max_teams = 11
    
    schedule = ScheduleClass(max_weeks, max_times, max_courts, max_teams)
    
    home = 0
    away = 0
    
    while len(schedule.weeks) < max_weeks:
        home,away = find_home_away_pair(schedule, home,away)
        schedule.add_game(home, away)
        
    print "Times Played: ", schedule.total_played
    print "Opponent Counts: ", schedule.opponent_counts 
    print "Wait Time / Team / Night: ", schedule.solution_strength()
        