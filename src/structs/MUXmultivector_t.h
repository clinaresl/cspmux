// -*- coding: utf-8 -*-
// MUXmultivector.h
// -----------------------------------------------------------------------------
//
// Started on <jue 12-08-2021 12:06:13.307181515 (1628762773)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
// Implementation of an array of vectors

#ifndef _MUXMULTIVECTOR_H_
#define _MUXMULTIVECTOR_H_

#include<algorithm>
#include<vector>

// Class definition
//
// Definition of a multivector
class multivector_t {

    private:

        // INVARIANT: A multivector consists of an array of vectors that
        // contains the mutexes of each entry
        std::vector<std::vector<size_t>> _mutex;

    public:

        // The default constructor is strictly forbidden
        multivector_t () = delete;

        // default copy and move constructors
        multivector_t (multivector_t&) = default;
        multivector_t (multivector_t&&) = default;

        // default copy and move assignments
        multivector_t& operator=(multivector_t&) = default;
        multivector_t& operator=(multivector_t&&) = default;

        // Explicit constructor - given the length of the array. Note that
        // implicit casting is forbidden
        explicit multivector_t (const size_t len) :
            _mutex { std::vector<std::vector<size_t>>(len, std::vector<size_t>()) }
        {}

        // accessors

        // Return true if and only if the specified value is found in the i-th
        // vector. the following service is provided solely for testing purposes
        bool find (const size_t i, const size_t value) const {
            return (std::find (_mutex[i].begin (),
                               _mutex[i].end (),
                               value) != _mutex[i].end ());
        }

        // get the i-th vector
        const std::vector<size_t>& operator[] (const size_t i) const {
            return _mutex[i];
        }

        // set the value j in the i-th vector
        void set (const size_t i, const size_t j) {
            _mutex[i].push_back (j);
        }

        // return whether two multivectors are identical or not. This service is
        // provided for testing purposes
        bool operator==(const multivector_t& right) const;

        // Likewise, define whether two multivectors are different. This service
        // is provided for testing purposes
        bool operator!= (const multivector_t& right) const {
            return !((*this) == right);
        }

        // return the number of entries in the multivector
        size_t size () const {
            return _mutex.size ();
        }
};

#endif // _MUXMULTIVECTOR_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
