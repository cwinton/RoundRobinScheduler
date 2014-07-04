//
//  League.h
//  Round.Robin.Scheduler
//
//  Created by Corey Winton on 6/28/14.
//  Copyright (c) 2014 Corey Winton. All rights reserved.
//

#ifndef __Round_Robin_Scheduler__League__
#define __Round_Robin_Scheduler__League__

#include <iostream>
#include "defs.h"
#include "Week.h"
#include "Team.h"


class League{
    std::vector<Week> weeks;
    std::vector<Team> teams;
    std::string start_date;
    std::string end_date;
    int nTeams;
    int nTimeSlots;
    int nCourts;
    int nWeeks;
    int wait_time;
    int min_played;
    
public:
    League(int _nteams, int _nWeeks, int _nTimeSlots, int _nCourts);
    
};

#endif /* defined(__Round_Robin_Scheduler__League__) */
