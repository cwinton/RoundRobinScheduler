#include "defs.h"

int VectMin (Vector _invect)
{
    return *std::min_element(_invect.begin(), _invect.end());
}

void allocate1D(Vector& _invect, int dim)
{
    _invect.clear();
    _invect.reserve(dim);
}

void init1D(Vector& _invect, int row)
{
    _invect.clear();
    _invect.resize(row);
    std::fill(_invect.begin(), _invect.end(), 0);
}

void init2D(DoubleVector& _invect, int row, int col)
{
    _invect.clear();
    _invect.resize(row);
    for (int i = 0; i < row; i++)
        init1D(_invect[i], col);
}

void allocate2D(DoubleVector& _invect, int row, int col)
{
    _invect.clear();
    _invect.reserve(row);
    
}

void allocate3D(TripleVector & _invect, int row, int col, int depth)
{
    _invect.clear();
    _invect.reserve(row);
    for (int i = 0; i < row; i++)
    {
        allocate2D(_invect[i], col, depth);
    }
}

void print_Vector(Vector _invect, std::ostream &stream = std::cout)
{
    for (Vector::const_iterator iter = _invect.begin(); iter != _invect.end(); ++iter)
    {
        stream << *iter << ",";
    }
    stream << std::endl;
}

void print_Vector(Vector _invect)
{
    print_Vector(_invect, std::cout);
}

void print_DoubleVector(DoubleVector _invect, std::ostream &stream = std::cout)
{
    for (DoubleVector::const_iterator row = _invect.begin(); row != _invect.end(); ++row)
    {
        print_Vector(*row, stream);
        stream << std::endl;
    }
}
void print_DoubleVector(DoubleVector _invect)
{
    print_DoubleVector(_invect, std::cout);
}

void print_TripleVector(TripleVector _invect, std::ostream &stream = std::cout)
{
    for (TripleVector::const_iterator row = _invect.begin(); row != _invect.end(); ++row)
    {
        print_DoubleVector(*row, stream);
        stream << std::endl;
    }
}
void print_TripleVector(TripleVector _invect)
{
    print_TripleVector(_invect, std::cout);
}