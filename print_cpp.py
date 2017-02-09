'''
Created on Jun 4, 2014

@author: coreywinton
'''
import datetime
import random
import numpy as np
import itertools
import timeit

def print_schedule(schedule):
    """Prints the schedule to be imported into excel"""
    
    team_defs = {
                0: "The Goat Bar",
                1: "Whiskey Drinks",
                2: "Brilliant!",
                3: "Cady",
                4: "Hands My Ass",
                5: "I Have No Idea",
                6: "Fireside",
                7: "TPS",
                8: "Peace Out",
                9: "The Cowfish"
                #8: "Pressure Tek",
                #9: "Fireside",
                #10: "Goodnight's",
# #             11: "Mikey",
# #             12: "Dirty Rotten Scoundrels",
# #             13: "The Goat Bar"}
#                  0: "Chris",
#                  1: "Charles",
#                  2: "Brian",
#                  3: "Jason",
#                  4: "Todd",
#                  5: "Thommy",
#                  6: "Stephen",
#                  7: "Other Division",
# #                  0: "Rose",
# #                  1: "Taylor",
# #                  2: "Matt",
# #                  3: "Daniel",
# #                  4: "Christian",
# #                  5: "Rubley",
# #                  6: "Tom Kotris",
# #                  7: "Other Division"
                }
    
    # Placeholder team defs
    #team_defs = {}
    #for i in range(max_teams):
    #    team_defs[i] = "Team {:03}".format(i)
        
    #team_defs[-1] = "Meeting"

    start_date = datetime.date(2016, 5, 25)
    
#    times = ["1:15 PM", "2:00 PM", "2:45 PM", "3:30 PM", "4:15 PM"]#, "9:15 PM"]
    times = ["6:45 PM", "7:45 PM", "8:45 PM"]#, "8:00 PM", "8:30 PM"]#, "9:15 PM"]#, "9:30 PM", "10:30PM"]
    #times = [""]
    #courts = ["Court 1", "Court 2", "Court 3", "Court 4"]
    courts = ["Game 1", "Game 2", "Game 3", "Game 4"]#, "Game 5"] #, "Court 2"]
    
    csv_sep = ","
    
    outfile = 'template_output.csv'
    file = open(outfile, 'w')
    
#    randomweeks = [week_i[0] for week_i in schedule]
    weekCount = 0
    for week in schedule:
        for time_iter, timeslot in enumerate(week):
#            timeslot = randomweeks.pop(random.randint(0,len(randomweeks)-1))
            for court_iter in range(max_courts):
                home_index = team_defs[int(timeslot[court_iter*2])]
                away_index = team_defs[int(timeslot[court_iter*2+1])]
                outstr ="Week {wk} {sep} {date} {sep} {timestr} {sep} {home} {sep} {away} {sep} {court}".format(wk = (weekCount%12)+1, date = start_date.strftime("%m/%d/%y"), sep = csv_sep, timestr = times[time_iter], home = home_index, away = away_index, court = courts[court_iter]) 
                print (outstr)
                file.write(outstr + "\n")
            print csv_sep
            file.write(csv_sep +"\n")
        print csv_sep
        file.write(csv_sep + "\n")
        start_date = start_date + datetime.timedelta(7)
        weekCount = weekCount + 1
        
def weekToMatrix(week):
    weekMat = np.zeros((max_times, max_teams), dtype='int32')
    weekMat[rowIndex, week] += 1
    
    #print weekMat
    if np.max(weekMat) == 1:
        return weekMat
    else:
        return None

def guageStrength(inMat, total_wait):
    firstPlay = np.argmax(inMat, axis=0)
    lastPlay = max_times - np.argmax(inMat[::-1], axis=0) - 1
    timePlay = inMat.sum(axis=0)
    
    #print firstPlay, lastPlay, timePlay
    
    strengthArray = lastPlay - firstPlay - timePlay + 1
    
    temp_total_wait = [x+y for x,y in zip(total_wait, strengthArray)]
    #return strengthArray, max(strengthArray)*100 + 10* list(strengthArray).count(max(strengthArray))*(max(strengthArray) - 1) + sum(strengthArray)
    return strengthArray, temp_total_wait, max(temp_total_wait)*100 + 10* list(temp_total_wait).count(max(temp_total_wait))*(max(temp_total_wait) - 1) + sum(temp_total_wait)

def buildPermute(tPermute, games):
    #home =  [games[2*i] for i in tPermute]
    #away = [games[2*i+1] for i in tPermute]
    #games = np.reshape([j for i in zip(home,away) for j in i], (-1, max_teams))
    
    #return games
    return [games[i] for i in tPermute]

def extractGames(week):
    return sum(week, [])

def findBestPermute(week, total_wait):
    minScore = max_teams*10*10
    for nPermute, tPermute in enumerate(timePermutes):
        #games = extractGames(week)
        weekMat = weekToMatrix(buildPermute(tPermute, week))
        #if weekMat != None:
        strArry, temp_total_wait, strength = guageStrength(weekMat, total_wait)
        
        if strength < minScore:
            minScore = strength
            bestPermutes = [tPermute]
            allArrys = [strArry]
            print minScore, bestPermutes, strArry, temp_total_wait
        if strength == minScore:
            bestPermutes.append(tPermute)
            allArrys.append(strArry)
            print minScore, bestPermutes, strArry, temp_total_wait
    return minScore, bestPermutes, allArrys

#def findNextWeek(schedule, total_wait, weekNum):
#    if weekNum == length(schedule):
#        return 
def sort_schedule(in_schedule):
    
    curScore = 100*100*max_weeks
    #findNextWeek(schedule, total_wait, 0)
    for iters in range(len(in_schedule)): #search several times
        schedule = [i for i in in_schedule]
        total_wait = [0]*max_teams

        for weekNum, week in enumerate(schedule):
            print 
            print schedule[weekNum]
            minScore = max_teams*10*10
            bestPermute = -1
            
            minScore, bestPermutes, allArrys = findBestPermute(week, total_wait)
            permuteChoice = random.randint(0, len(bestPermutes)-1)
            bestPermute = bestPermutes[permuteChoice]
            strArry = allArrys[permuteChoice]
            print permuteChoice, bestPermute
            total_wait = [x+y for x,y in zip(total_wait, strArry)]
            schedule[weekNum] = [week[i] for i in bestPermute]
            print schedule[weekNum]
            print
        print total_wait
        
        if minScore < curScore:
            curScore = minScore
            bestSchedule = [i for i in schedule]
            bestWait = [i for i in total_wait]
    
            print "****************************"
            print "New Best Schedule"
            print curScore, bestWait
            print "****************************"
        else:
            print "****************************"
            print "Worse Schedule"
            print minScore, "(", curScore, ")", total_wait
            print "****************************"
    return bestSchedule
        

if __name__ == '__main__':
    #cpp_output_file = "/Users/rditlcww/git/RoundRobinScheduling/CPP_Code/Round.Robin.Scheduler/Round.Robin.Scheduler/schedulesTest.txt"
    cpp_output_file = "/Users/rditlcww/git/RoundRobinScheduling/CPP_Code/Round.Robin.Scheduler/Round.Robin.Scheduler/goodSchedule.txt"
    file = open(cpp_output_file, 'r')
    
    data = file.readlines()
    
    fdata = [line.rstrip('\n') for line in data if line.rstrip('\n') != '']
    
    bestStart = (len(fdata) - 1) - fdata[::-1].index('***NEW SCHEDULE***')
    max_weeks = int(fdata[bestStart + 1])
    max_times = int(fdata[bestStart + 2])
    max_courts = int(fdata[bestStart + 3])
    max_teams = int(fdata[bestStart + 4])
    
    
    print max_weeks, max_times, max_courts, max_teams
    
    week_data = fdata[bestStart + 6:bestStart + 6+max_weeks*max_times]
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
    scheduleInt = [[[int(i) for i in Etimes if i != ''] for Etimes in Eweeks] for Eweeks in weeks]
    rowIndex = [[i]*max_courts*2 for i in range(max_times)]
    
    timePermutes = list(itertools.permutations([i for i in range(max_times)]))
    print scheduleInt
    print rowIndex
    
    #bestSchedule = sort_schedule(scheduleInt)
    #print_schedule(bestSchedule)
    print_schedule(scheduleInt)