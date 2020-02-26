//
//  Minibex2AMPL.hpp
//  
//
//  Created by Raphaël Chenouard on 07/01/2020.
//

#ifndef Minibex2AMPL_H
#define Minibex2AMPL_H


#include <iostream>

#include <string>
#include <sstream>

#include "Expr2AMPL.h"

using namespace std;
using namespace ibex;


bool system2ampl(const System& sys, const char* fname);


#endif /* Minibex2AMPL_H */
