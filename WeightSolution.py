'''
Created on Jun 2, 2014

@author: coreywinton
'''

def week_strength(week, team):
    # Defines how much time each team waits per night
    playcount = [int(team in timeslot) for timeslot in week]
    playtimes = sum(playcount)
    if playtimes > 0:
        firstplay = playcount.index(1)
        lastplay = len(playcount) -1 - playcount[::-1].index(1)
        return lastplay - firstplay - playtimes + 1
    else:
        return playtimes
            

def solution_strength(max_weeks = 9,max_times = 6,max_courts = 2,max_teams = 11,weeks = []):
    total_wait_time = 0
    sol_strength = []
    for week_num, week in enumerate(weeks):
        #Iterate SOS over each week
        sol_strength.append([])
        for team in range(max_teams):
            sol_strength[week_num].append(week_strength(week, team))
        total_wait_time += sum(sol_strength[week_num])
   
    return total_wait_time
    
    