//
//  defs.h
//  Round.Robin.Scheduler
//
//  Created by Corey Winton on 6/27/14.
//  Copyright (c) 2014 Corey Winton. All rights reserved.
//

#ifndef Round_Robin_Scheduler_defs_h
#define Round_Robin_Scheduler_defs_h

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <fstream>

using std::vector;

typedef std::vector<int> Vector;
typedef std::vector<Vector> DoubleVector;
typedef std::vector<DoubleVector> TripleVector;


int VectMin(Vector);
void allocate1D (Vector&, int);
void allocate2D (DoubleVector & _invect, int row, int col);
void allocate3D (TripleVector & _invect, int row, int col, int depth);

void init1D(Vector& _invect, int);
void init2D(DoubleVector& _invect, int, int);

void print_Vector(Vector _invect, std::ostream&);
void print_DoubleVector(DoubleVector _invect, std::ostream&);
void print_TripleVector(TripleVector _invect, std::ostream&);

void print_Vector(Vector);
void print_DoubleVector(DoubleVector);
void print_TripleVector(TripleVector);


#endif
