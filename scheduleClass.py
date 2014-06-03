'''
Created on Jun 2, 2014

@author: coreywinton
'''

from math import ceil, floor

class ScheduleClass():
    '''
    classdocs
    '''
    
    weeks = []
    timeslots = []
    courts = []
    matchups = []
    this_week_matchups = []
    
    temp_play_count = []
    
    def week_strength(self, week, team):
        # Defines how much time each team waits per night
        playcount = [int(team in timeslot) for timeslot in week]
        playtimes = sum(playcount)
        if playtimes > 0:
            firstplay = playcount.index(1)
            lastplay = len(playcount) -1 - playcount[::-1].index(1)
            return lastplay - firstplay - playtimes + 1
        else:
            # Penalize for a team not playing
            return len(playcount)*2
    
    def solution_strength(self):
        total_wait_time = 0
        for week in self.weeks:
            for team in range(self.max_teams):
                total_wait_time+= self.week_strength(week, team)
        self.total_wait_time = total_wait_time
        return total_wait_time
    
    
    
    def store_timeslot(self):
        for good_matchup in self.this_week_matchups:
            home_m, away_m = good_matchup
            
            self.opponent_counts[home_m][away_m] += 1
            self.opponent_counts[away_m][home_m] += 1
            
            self.total_played[home_m] += 1
            self.total_played[away_m] += 1
            
            self.temp_play_count = [0 for i in range(self.max_teams)]

            
            self.matchups.append(good_matchup)
            
    def add_week(self):
        self.weeks.append(self.timeslots)
        self.store_timeslot()
        self.timeslots = []
        self.this_week_matchups = []
    
    def add_timeslot(self):
        self.timeslots.append(self.courts)
        self.courts = []
        
        if len(self.timeslots) == self.max_times:
            self.add_week()
    
    def teams_feasible(self, home, away):
        """ Check to ensure:
        1.  Not playing elsewhere
        2.  Not playing themself
        3.  Not played too much already that night
        4.  Not played twice more than another team overall
        """
        elsewhere = (home not in self.courts) and (away not in self.courts)
        themself = home != away
        
        that_night_home = sum([time.count(home) for time in self.timeslots]) < self.max_per_night
        that_night_away = sum([time.count(home) for time in self.timeslots]) < self.max_per_night
        
        overall_home = ((self.temp_play_count[home] - min(self.temp_play_count)) < 2)
        overall_away = ((self.temp_play_count[away] - min(self.temp_play_count)) < 2)
        
        return (elsewhere and
                themself and
                that_night_home and that_night_away and
                overall_home and overall_away)
        
        
    
    def matchup_feasible(self, home, away):
        """ Check to ensure:
        1.  Matchup is not scheduled for this week
        2.  Teams have not repeatedly played each other without playing other teams first
        """
        this_week = ([home,away] not in self.this_week_matchups) and ([away,home] not in self.this_week_matchups)
        
        min_home = min([temp_opp_count[home][i] for i in range(max_teams) if i != home])
        min_away = min([temp_opp_count[away][i] for i in range(max_teams) if i != away])
        home_freq = ((temp_opp_count[home][away] - min_home) == 0)
        away_freq = ((temp_opp_count[away][home] - min_away) == 0)       
    
    def add_game (self, home, away):
        # Check viability of home/away pair
        if (self.teams_feasible(home,away) and
            self.matchup_feasible(home,away)):
            
            # Add to matchups if acceptable
            self.this_week_matchups.append([home,away])
            self.courts.append(home)
            self.courts.append(away)
            
            self.temp_play_count[home] += 1
            self.temp_play_count[away] += 1
        
        if len(self.courts) == self.max_courts * 2:
            self.add_timeslot()
        
        


    def __init__(self, max_weeks, max_times, max_courts, max_teams):
        '''
        Constructor
        '''
        
        # Define the problem
        self.max_weeks = max_weeks
        self.max_times = max_times
        self.max_courts = max_courts
        self.max_teams = max_teams
        
        # Storage Terms
        self.total_played = [0 for i in range(max_teams)]
        self.opponent_counts = [[0 for x in range(max_teams)] for y in range(max_teams)]
        self.temp_play_count = [0 for i in range(max_teams)]

        
        # No team should play significantly more than another team / night
        self.max_per_night = int(ceil((max_courts * max_times * 2) / (max_teams*1.0)))
        self.min_per_night = int(floor((max_courts * max_times * 2) / (max_teams*1.0)))
        