// -*- coding: utf-8 -*-
// MUXvalue_t.h
// -----------------------------------------------------------------------------
//
// Started on <lun 09-08-2021 01:13:55.417940436 (1628464435)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Definition of values of any type

#ifndef _VALUE_T_H_
#define _VALUE_T_H_

// Class definition
//
// Definition of values of any type
template<class T>
class value_t {

    private:

        // INVARIANT: values store non-mutable information of any type
        T _value;

    public:

        // Default constructors are disabled
        value_t () = delete;

        // Explicit constructors - given the information to store
        value_t (T value) :
            _value { value }
        {}

        // accessors
        const T& get_value () const
        { return _value; }

};

#endif // _VALUE_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
