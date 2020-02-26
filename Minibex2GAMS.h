//
//  Minibex2AMPL.hpp
//  
//
//  Created by Raphaël Chenouard on 26/02/2020.
//

#ifndef Minibex2GAMS_H
#define Minibex2GAMS_H


#include <iostream>

#include <string>
#include <sstream>

#include "Expr2GAMS.h"

using namespace std;
using namespace ibex;


bool system2gams(const System& sys, const char* fname);


#endif /* Minibex2GAMS_H */
