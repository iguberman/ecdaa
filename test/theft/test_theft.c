//
// Created by Irina Guberman on 9/27/17.
//

#include "test_theft.h"

/* Add all the definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line arguments, initialization. */
    RUN_SUITE(groupkeygen);
    //RUN_SUITE(autoshrink);
    //RUN_SUITE(aux);
    //RUN_SUITE(bloom);
    //RUN_SUITE(error);
    //RUN_SUITE(integration);
    GREATEST_MAIN_END();        /* display results */
}
