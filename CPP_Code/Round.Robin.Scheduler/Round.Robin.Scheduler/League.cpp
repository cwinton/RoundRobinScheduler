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
    
    weeks.reserve(nWeeks);
    teams.reserve(nTeams);
    
    Team blankTeam;
    
    for (int i = 0; i < nTeams; i++)
    {
        blankTeam = new Team(nTeams, nTimeSlots, "")
    }
        teams.push_back(i);
}

bool League::addGame(Team home, Team away)
{
    
    return false;
}