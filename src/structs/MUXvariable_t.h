// -*- coding: utf-8 -*-
// MUXvariable_t.h
// -----------------------------------------------------------------------------
//
// Started on <lun 09-08-2021 15:55:45.466802289 (1628517345)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Definition of a CSP variable

#ifndef _MUXVARIABLE_T_H_
#define _MUXVARIABLE_T_H_

#include<string>

using namespace std;

// Class definition
//
// Definition of a CSP variable
class variable_t {

    private:

        // A variable consists of a name and, optionally, a description
        string _name;
        string _description;

    public:

        // Default constructors are forbidden
        variable_t () = delete;

        // Explicit constructors - at least the name has to be given and,
        // optionally, a description
        variable_t (string name) :
            _name { name },
            _description { "" }
        {}
        variable_t (string name, string description) :
            _name { name },
            _description { description }
        {}

        // accessors

        // get the name of this variable
        const string& get_name () const {
            return _name;
        }

        // get the description of this variable
        const string& get_description () const {
            return _description;
        }

        // set the description of this variable
        void set_description (string description) {
            _description = description;
        }

        // two variables are the same if and only if they have strictly the same
        // name (in spite of the description)
        bool operator==(const variable_t& right) const {
            return _name == right.get_name ();
        }

        // Likewise, define whether they are different
        bool operator!=(const variable_t& right) const {
            return _name != right.get_name ();
        }
};

#endif // _MUXVARIABLE_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
