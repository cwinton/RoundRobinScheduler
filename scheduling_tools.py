'''
Created on Jun 2, 2014

@author: coreywinton
'''

def calc_temp_opponents(opponent_counts, potential_matchups, max_teams):
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

def calc_temp_play_count (total_played, courts, timeslots, max_teams):
    """Calculate how many times a team has played 
    INCLUDING potential matchups this week
    """
    
    temp_play_count = [total_played[i] + courts.count(i) + sum([time.count(i) for time in timeslots]) for i in range(max_teams)]
    return temp_play_count


def check_matchup_feasible (home, away, potential_matchups, matchups, opponent_counts, max_teams):
    """ Check to ensure:
    1.  Matchup has not already occurred previously (deprecated)
    2.  Matchup is not scheduled for this week
    3.  Teams have not repeatedly played each other without playing other teams first
    """
    #previously = [home,away] not in matchups
    previously = True
    
    this_week = ([home,away] not in potential_matchups) and ([away,home] not in potential_matchups)
    
    temp_opp_count = calc_temp_opponents(opponent_counts, potential_matchups, max_teams)
    min_home = min([temp_opp_count[home][i] for i in range(max_teams) if i != home])
    min_away = min([temp_opp_count[away][i] for i in range(max_teams) if i != away])
    home_freq = ((temp_opp_count[home][away] - min_home) == 0)
    away_freq = ((temp_opp_count[away][home] - min_away) == 0)
    
    return (previously and 
            this_week and
            home_freq and
            away_freq)               

def check_team_feasible (home, away, courts, timeslots, total_played, max_teams, max_per_night):
    """ Check to ensure:
    1.  Not playing elsewhere
    2.  Not playing themself
    3.  Not played too much already that night
    4.  Not played twice more than another team overall
    """
    elsewhere = home not in courts
    themself = home != away
    
    that_night = sum([time.count(home) for time in timeslots]) < max_per_night
    
    temp_play_count = calc_temp_play_count(total_played, courts, timeslots, max_teams)
    overall = ((temp_play_count[home] - min(temp_play_count)) < 2) 
    
    #max_times_played = (temp_play_count[home] < max_played)
    
    return (elsewhere and 
            themself and
            that_night and
            overall)