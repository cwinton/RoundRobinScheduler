//
//  Week.h
//  Round.Robin.Scheduler
//
//  Created by Corey Winton on 6/28/14.
//  Copyright (c) 2014 Corey Winton. All rights reserved.
//

#ifndef __Round_Robin_Scheduler__Week__
#define __Round_Robin_Scheduler__Week__

#include <iostream>
#include "TimeSlot.h"

class Week{
    std::vector<TimeSlot> timeSlots;
    std::string date_str;
    std::string location;
    int wait_time;
    int week_num;
};
#endif /* defined(__Round_Robin_Scheduler__Week__) */
