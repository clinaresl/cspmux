// -*- coding: utf-8 -*-
// main.cc
// -----------------------------------------------------------------------------
//
// Started on <jue 05-08-2021 23:21:24.555922969 (1628198484)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Description
//

// include
#include <iostream>

#include "cspmux.h"

// definitions
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

using namespace std;

// main
int main(int argc, char *argv[]) {

    bmap_t bmap (8);

    cout << " Length: " << bmap.size () << endl;
    bmap.set (2, true);
    bmap.set (7, true);

    cout << " Contents: (setting 2 and 7)" << endl;
    for (int i=0;i<bmap.size ();i++) {
        cout << "\t[" << i << "]: " << bmap.get (i) << endl;
    }
    cout << endl;

    bmap.set (2, false);

    cout << " Contents: (resetting 2)" << endl;
    for (int i=0;i<bmap.size ();i++) {
        cout << "\t[" << i << "]: " << bmap.get (i) << endl;
    }
    cout << endl;

    bmap.set (6, false);
    bmap.set (7, false);

    cout << " Contents: (resetting 6 and 7 )" << endl;
    for (int i=0;i<bmap.size ();i++) {
        cout << "\t[" << i << "]: " << bmap.get (i) << endl;
    }
    cout << endl;

    bmap.set (3, true);
    bmap.set (3, true);

    cout << " Contents: (setting 3 twice)" << endl;
    for (int i=0;i<bmap.size ();i++) {
        cout << "\t[" << i << "]: " << bmap.get (i) << endl;
    }

    cout << "--------------------------------------" << endl;

    multibmap_t multibmap (10, 15);
    multibmap.set (0, 3, true);
    multibmap.set (7, 2, false);

    cout << " multibmap[0, 3]: " << multibmap[0][3] << endl;
    cout << " multibmap[7, 2]: " << multibmap[7][2] << endl;

    return EXIT_SUCCESS;
}


// Local Variables:
// mode:c++
// fill-column:80
// End:
