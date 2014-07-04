//
//  TimeSlot.h
//  Round.Robin.Scheduler
//
//  Created by Corey Winton on 6/28/14.
//  Copyright (c) 2014 Corey Winton. All rights reserved.
//

#ifndef __Round_Robin_Scheduler__TimeSlot__
#define __Round_Robin_Scheduler__TimeSlot__

#include "defs.h"
#include <iostream>
#include "Game.h"

class TimeSlot{
    std::vector<Game> games;
    std::string time_stamp;
    int slot_num;
};

#endif /* defined(__Round_Robin_Scheduler__TimeSlot__) */
