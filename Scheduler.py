'''
Created on May 30, 2014

@author: coreywinton
'''
from math import ceil
from math import floor
from random import randint
import datetime
#import msvcrt as m


max_weeks = 9
max_times = 6
max_courts = 2
max_teams = 11

#max_played = 20
#max_opp = 2


# No team should play significantly more than another team / night
max_per_night = int(ceil((max_courts * max_times * 2) / (max_teams*1.0)))
min_per_night = int(floor((max_courts * max_times * 2) / (max_teams*1.0)))


def print_schedule(schedule):
    """Prints the schedule to be imported into excel"""
    
#     team_defs = {0: "Cady",
#              1: "Vertigo",
#              2: "Brilliant!",
#              3: "Balls to the Face",
#              4: "Hands My Ass",
#              5: "Bump, Set, Net",
#              6: "Whiskey Dinks",
#              7: "Pancakes After Sets",
#              8: "Pressure Tek",
#              9: "Fireside",
#              10: "Goodnight's",
#              11: "Mikey",
#              12: "Dirty Rotten Scoundrels",
#              13: "The Goat Bar"}
    
    # Placeholder team defs
    team_defs = {}
    for i in range(max_teams):
        team_defs[i] = "Team {:03}".format(i)

    start_date = datetime.date(2014, 06, 5)
    
    times = ["6:45 PM", "7:15 PM", "7:45 PM", "8:15 PM", "8:45 PM", "9:15 PM"]
#    times = ["6:30 PM", "7:30 PM", "8:30 PM", "9:30 PM", "10:30PM"]

#    courts = ["Court 1", "Court 2", "Court 3", "Court 4"]
    courts = ["Court 1", "Court 2"]
    
    csv_sep = ";"
    
    outputfile = "schedule.csv"
    f = open (outputfile, 'w')
    
    
    for week in schedule:
        for time_iter, timeslot in enumerate(week):
            for court_iter in range(max_courts):
                home_index = team_defs[timeslot[court_iter*2]]
                away_index = team_defs[timeslot[court_iter*2+1]]
                outstring = "{date} {sep} {timestr} {sep} {home} {sep} {away} {sep} {court}".format(date = start_date.strftime("%m/%d/%y"), sep = csv_sep, timestr = times[time_iter], home = home_index, away = away_index, court = courts[court_iter])
                print outstring
                f.write(outstring + "\n")
            print csv_sep
            f.write(csv_sep + "\n")
        print csv_sep
        f.write(csv_sep + "\n")
        start_date = start_date + datetime.timedelta(7)
        
    f.close()

def check_team_feasible (home, away, courts, timeslots, total_played):
    """ Check to ensure:
    1.  Not playing elsewhere
    2.  Not playing themself
    3.  Not played too much already that night
    4.  Not played twice more than another team overall
    5.  [optional] Max times played
    """
    elsewhere = home not in courts
    themself = home != away
    
    that_night = sum([time.count(home) for time in timeslots]) < max_per_night
    
    temp_play_count = calc_temp_play_count(total_played, courts, timeslots)
    overall = ((temp_play_count[home] - min(temp_play_count)) < 2) 
    
    #max_times_played = (temp_play_count[home] < max_played)
    
    return (elsewhere and 
            themself and
            that_night and
            overall)
    
            #max_times_played)
def calc_temp_play_count (total_played, courts, timeslots):
    """Calculate how many times a team has played 
    INCLUDING potential matchups this week
    """
    
    temp_play_count = [total_played[i] + courts.count(i) + sum([time.count(i) for time in timeslots]) for i in range(max_teams)]
    return temp_play_count
                         
def check_matchup_feasible (home, away, potential_matchups, matchups, opponent_counts):
    """ Check to ensure:
    1.  Matchup has not already occurred previously (deprecated)
    2.  Matchup is not scheduled for this week
    3.  Teams have not repeatedly played each other without playing other teams first
    """
    #previously = [home,away] not in matchups
    previously = True
    
    this_week = ([home,away] not in potential_matchups) and ([away,home] not in potential_matchups)
    
    temp_opp_count = calc_temp_opponents(opponent_counts, potential_matchups)
    min_home = min([temp_opp_count[home][i] for i in range(max_teams) if i != home])
    min_away = min([temp_opp_count[away][i] for i in range(max_teams) if i != away])
    home_freq = ((temp_opp_count[home][away] - min_home) == 0)
    away_freq = ((temp_opp_count[away][home] - min_away) == 0)
    
    return (previously and 
            this_week and
            home_freq and
            away_freq)               
                     
def calc_temp_opponents(opponent_counts, potential_matchups):
    """Calculate how many times a team will have played other teams
    INCLUDING potential matchups this week
    """
    temp_opp_count = [[opponent_counts[h_ind][a_ind] for a_ind in range(max_teams)] for h_ind in range(max_teams)]
    for this_week_matchup in potential_matchups:
        tw_h = this_week_matchup[0]
        tw_a = this_week_matchup[1]
        temp_opp_count[tw_h][tw_a] += 1
        temp_opp_count[tw_a][tw_h] += 1
    return temp_opp_count

    
def find_home_away_pair(home_start, away_start, courts, timeslots, potential_matchups, matchups, opponent_counts, total_played):
    """ Find two teams that:
    1.  Are not playing elsewhere at the same time
    2.  Have not played each other more than twice
    3.  Have not already played too many times that night
    """
  
    for home_iter in range(max_teams):
        # Brute force iteration through home teams
        home = (home_start + home_iter) % max_teams
        for away_iter in range(max_teams):
            # Brute force iteration through away teams
            away = (away_start + away_iter) % max_teams
            
            # Check home team viability
            if (check_team_feasible(home, away, courts, timeslots, total_played) and
                # Check away team viability 
                check_team_feasible(away, home, courts, timeslots, total_played) and
                # Check home/away viability
                check_matchup_feasible(home, away, potential_matchups, matchups,opponent_counts)):
                    return home, away


def print_failure_info(this_week, fail_count, courts, timeslots, this_week_matchups, opponent_counts, total_played):
    """ Print out input at failure points
    """
    
    print" ************** SOLUTION FAILURE WEEK %i (%i)******************" % (this_week, fail_count)
    print "Courts: ", courts
    print "Timeslots: ", timeslots
    print "This week matches: ", this_week_matchups
    print "Opp Counts: ", opponent_counts
    print "Total Played: ", total_played
    
    temp_total_played = calc_temp_play_count (total_played, courts, timeslots)
    print "Temp Total Played: ", temp_total_played
    if (min(temp_total_played) == max(temp_total_played)):
        print "Equal Condition"

def backtrack(this_week, reboot_weeks, matchups, total_played, opponent_counts, weeks):
    """ When solutions fail, it is necessary to back up a timestep 
    Need to remove the matchups and relevant information """
    # Variable number of rebound weeks
    for reb_week in range(min(reboot_weeks, this_week)):
        print "%%%%% Removing Previous Week %%%%%%"
        # Remove matchups
        for remove_ind in range(max_times * max_courts):
            bad_home = matchups[-1][0]
            bad_away = matchups[-1][1]
            total_played[bad_home] -= 1
            total_played[bad_away] -= 1
            opponent_counts[bad_home][bad_away] -= 1
            opponent_counts[bad_away][bad_home] -= 1
            del matchups[-1]
        this_week = this_week - 1
        print "week size: ", len(weeks),
        del weeks[-1]
        print len(weeks)
    reboot_weeks += 2
    
    return this_week, reboot_weeks, matchups, total_played, opponent_counts, weeks
    
def failure_mode(home, away, courts, timeslots, this_week_matchups, matchups, opponent_counts, total_played, fail_count, reboot_weeks, this_week, weeks):
    
    # Print info
    print_failure_info(this_week, fail_count, courts, timeslots, this_week_matchups, opponent_counts, total_played)
    
    # Reset current week 
    courts = []
    timeslots = []
    this_week_matchups = []
                

    # Too Many Failures, back up a step
    if (fail_count % max_teams == 0) and (fail_count > 0):
        this_week, reboot_weeks, matchups, total_played, opponent_counts, weeks = backtrack(this_week, reboot_weeks, matchups, total_played, opponent_counts, weeks)

    # Pick a new team
    teams = find_home_away_pair(randint(0,max_teams), randint(0,max_teams), courts, timeslots, this_week_matchups, matchups, opponent_counts, total_played)
    
    # Return new teams
    return teams, this_week, reboot_weeks, matchups, total_played, opponent_counts, weeks

def add_matchup(teams, this_week_matchups, courts, timeslots, reboot_weeks, this_week, weeks, opponent_counts, total_played, matchups, fail_count):
    
    home, away = teams
    # Add them to this week's matchups
    this_week_matchups.append([home,away])
    courts.append(home)
    courts.append(away)
    


    # IF all the courts are full, move to the next timeslot
    if (len(courts) == max_courts * 2):
        # Save court info & reset
        timeslots.append(courts)
        courts = []
        home = home + 1
        away = away + 1
    
        # IF all the timeslots are full, store the week's info
        if (len(timeslots) == max_times):
            
            
            played = [0] * max_teams
            for time in timeslots:
                for i in range(max_teams):
                    played[i] += (i in time)
            print played
            if (min(played) >= min_per_night):
                fail_count = 0
                reboot_weeks = max(1, reboot_weeks-1)
                this_week += 1
                print this_week
                weeks.append(timeslots)
                for good_matchup in this_week_matchups:
                    home_m,away_m = good_matchup
                    opponent_counts[home_m][away_m] += 1
                    opponent_counts[away_m][home_m] += 1
                    
                    total_played[home_m] += 1
                    total_played[away_m] += 1
                    matchups.append(good_matchup)
            else:
                fail_count += 1
                home = home + randint(0,max_teams)
                away = away + randint(0,max_teams)
    
            timeslots = []
            this_week_matchups = []
    return home,away, this_week_matchups, courts, timeslots, reboot_weeks, this_week, weeks, opponent_counts, total_played, matchups, fail_count

if __name__ == '__main__':
    
    this_week = 0
    
    home = 0
    away = 0
    
    matchups = []
    courts = []
    timeslots = []
    weeks = []
    this_week_matchups = []
    
    reboot_weeks = 1
    
    # Count games played:
    total_played = [0 for i in range(max_teams)]
    
    # Count times playing each other
    opponent_counts =[[0 for x in range(max_teams)] for y in range(max_teams)]
    
     
    # Preseed because of previous information!!!
    #################
#     weeks.append([[0,1,2,3,4,5,6,7],[8,9,10,11,12,13,0,2],[1,3,4,6,5,7,8,10]])
#     for pre_time in weeks[0]:
#         for i in range(len(pre_time)/2):
#             home = pre_time[i*2]
#             away = pre_time[i*2+1]
#             
#             matchups.append([home,away])
#             
#             opponent_counts[home][away] += 1
#             opponent_counts[away][home] += 1
#             
#             total_played[home] += 1
#             total_played[away] += 1
#             
#     this_week = 1;
#     home = 9
#     away = 11
    
    #################
    

    fail_count = 0
    
    while this_week < max_weeks:
        
        # First, find a pair of teams that can play
        teams = find_home_away_pair(home, away, courts, timeslots, this_week_matchups, matchups, opponent_counts, total_played)
        # Failure results from:
        #   1 --- No suitable teams meet criteria
        #   2 --- Schedule was inappropriate (some teams did not play frequently enough)
        while ((teams == None) or 
              ((fail_count % max_teams == 0) and (fail_count > 0))):
            teams, this_week, reboot_weeks, matchups, total_played, opponent_counts, weeks = failure_mode(home, away, courts, timeslots, this_week_matchups, matchups, opponent_counts, total_played, fail_count, reboot_weeks, this_week, weeks)

            fail_count += 1
        
        home, away, this_week_matchups, courts, timeslots, reboot_weeks, this_week, weeks, opponent_counts, total_played, matchups, fail_count = add_matchup(teams, this_week_matchups, courts, timeslots, reboot_weeks, this_week, weeks, opponent_counts, total_played, matchups, fail_count)

    for week in weeks:
        print week
      
    print_schedule(weeks)
    
    print "Times Played Counts: ", total_played

    

    print "Opponent counts: ", opponent_counts
    
    
    # Count Matchup Frequencies:
    
        
        
            
                