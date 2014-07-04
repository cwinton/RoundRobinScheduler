//
//  Game.h
//  Round.Robin.Scheduler
//
//  Created by Corey Winton on 6/28/14.
//  Copyright (c) 2014 Corey Winton. All rights reserved.
//

#ifndef __Round_Robin_Scheduler__Game__
#define __Round_Robin_Scheduler__Game__

#include <iostream>
#include "defs.h"

class Team;

class Game{
    Team *home_ptr;
    Team *away_ptr;
    int home, away;
    int court, time, week;
    
public:
    Game(Team* _home, Team* _away, int _court, int _time, int _week);
};

#endif /* defined(__Round_Robin_Scheduler__Game__) */
