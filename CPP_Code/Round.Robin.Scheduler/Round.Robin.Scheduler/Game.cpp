//
//  Game.cpp
//  Round.Robin.Scheduler
//
//  Created by Corey Winton on 6/28/14.
//  Copyright (c) 2014 Corey Winton. All rights reserved.
//

#include "Game.h"
#include "Team.h"


Game::Game(Team* _home, Team* _away, int _court, int _time, int _week)
{
    home_ptr = _home;
    away_ptr = _away;
    court = _court;
    time = _time;
    week = _week;
}