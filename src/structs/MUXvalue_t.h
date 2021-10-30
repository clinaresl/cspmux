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

        // INVARIANT: values store inmutable information of any type
        T _value;

    public:

        // Default constructors are disabled
        value_t () = delete;

        // Explicit constructors - given the information to store
        value_t (const T value) :
            _value { value }
        {}

        // accessors
        const T& get_value () const
        { return _value; }

        // return whether two values defined over the same type are the same or
        // not. This definition depends also upon the definition of equality for
        // the type T
        bool operator==(const value_t<T>& right) const {
            return _value == right.get_value ();
        }

        // Likewise, define whether they are different
        bool operator!=(const value_t<T>& right) const {
            return _value != right.get_value ();
        }

        // Importantly, raw values have to provide the relational operator <
        bool operator<(const value_t<T>& right) const {
            return _value < right.get_value ();
        }
};

#endif // _VALUE_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
