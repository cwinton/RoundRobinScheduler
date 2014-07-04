//
//  League.cpp
//  Round.Robin.Scheduler
//
//  Created by Corey Winton on 6/28/14.
//  Copyright (c) 2014 Corey Winton. All rights reserved.
//

#include "League.h"


League::League(int _nteams, int _nWeeks, int _nTimeSlots, int _nCourts)
{
    nTeams = _nteams;
    nWeeks = _nWeeks;
    nTimeSlots = _nTimeSlots;
    nCourts = _nCourts;
    
    weeks.resize(nWeeks);
    teams.resize(nTeams);
}