// -*- coding: utf-8 -*-
// TSTdefs.h
// -----------------------------------------------------------------------------
//
// Started on <sáb 07-08-2021 00:58:59.720149236 (1628290739)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
//

#ifndef _TSTDEFS_H_
#define _TSTDEFS_H_

#include<stddef.h>

// predefined number of executions of each unit test
constexpr int NB_TESTS = 10000;

// Maximum length of the bitmaps: 8 Gbytes
constexpr size_t MAX_LENGTH = 64000000000;

// Maximun number of CSP variables and values for each variable
constexpr size_t NB_VARIABLES = 10000;
constexpr size_t NB_VALUES = 100;

#endif // _TSTDEFS_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
