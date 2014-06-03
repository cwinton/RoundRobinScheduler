'''
Created on Jun 2, 2014

@author: coreywinton
'''

from math import ceil, floor
from WeightSolution import solution_strength

from scheduleClass import ScheduleClass

max_weeks = 9
max_times = 6
max_courts = 2
max_teams = 11

# No team should play significantly more than another team / night
max_per_night = int(ceil((max_courts * max_times * 2) / (max_teams*1.0)))
min_per_night = int(floor((max_courts * max_times * 2) / (max_teams*1.0)))

def fill_bracket (home, away, courts, timeslots, total_played, potential_matchups, matchups, opponent_counts, weeks):
    # Iterate over all possible home/away team combos
    for home_iter in range(max_teams):
        home = (home + home_iter) % max_teams
        for away_iter in range(max_teams):
            away = (away + away_iter) % max_teams
            
            # Found two appropriate teams to play!
            if (check_team_feasible(home, away, courts, timeslots, total_played, max_teams, max_per_night) and
                check_team_feasible(away, home, courts, timeslots, total_played, max_teams, max_per_night) and
                check_matchup_feasible(home, away, potential_matchups, matchups, opponent_counts, max_teams)):
                
                this_week
                
            
    
    if len(weeks) == max_weeks:
        print "Solution Strength: ", solution_strength(weeks)
        return weeks, solution_strength(weeks)
    else:
        

if __name__ == '__main__':
    