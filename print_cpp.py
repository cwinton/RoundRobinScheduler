'''
Created on Jun 4, 2014

@author: coreywinton
'''
import datetime
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

    start_date = datetime.date(2014, 06, 12)
    
    times = ["6:45 PM", "7:15 PM", "7:45 PM", "8:15 PM", "8:45 PM", "9:15 PM"]
#    times = ["6:30 PM", "7:30 PM", "8:30 PM", "9:30 PM", "10:30PM"]

#    courts = ["Court 1", "Court 2", "Court 3", "Court 4"]
    courts = ["Court 1", "Court 2"]
    
    csv_sep = ";"
    
    outfile = 'excel_format.csv'
    file = open(outfile, 'w')
    
    for week in schedule:
        for time_iter, timeslot in enumerate(week):
            for court_iter in range(max_courts):
                home_index = team_defs[int(timeslot[court_iter*2])]
                away_index = team_defs[int(timeslot[court_iter*2+1])]
                outstr ="{date} {sep} {timestr} {sep} {home} {sep} {away} {sep} {court}".format(date = start_date.strftime("%m/%d/%y"), sep = csv_sep, timestr = times[time_iter], home = home_index, away = away_index, court = courts[court_iter]) 
                print (outstr)
                file.write(outstr + "\n")
            print csv_sep
            file.write(csv_sep +"\n")
        print csv_sep
        file.write(csv_sep + "\n")
        start_date = start_date + datetime.timedelta(7)


if __name__ == '__main__':
    cpp_output_file = "/Users/coreywinton/git/RoundRobinScheduling/CPP_Code/Round.Robin.Scheduler/Round.Robin.Scheduler/schedules.txt"
    file = open(cpp_output_file, 'r')
    
    data = file.readlines()
    
    fdata = [line.rstrip('\n').rstrip(',') for line in data if line.rstrip('\n') != '']
    
    max_weeks = int(fdata[0])
    max_times = int(fdata[1])
    max_courts = int(fdata[2])
    max_teams = int(fdata[3])
    
    
    print max_weeks, max_times, max_courts, max_teams
    
    week_data = fdata[5:5+max_weeks*max_times]
    print fdata    
    print week_data
    
    weeks = []
    
    week_data_lcv = 0
    for week in range(max_weeks):
        times = []
        for time in range(max_times):
            times.append(week_data[week_data_lcv].split(','))
            week_data_lcv += 1
        weeks.append(times)
        
    print weeks
    
    print_schedule(weeks)