#include "calibration.hpp"

//define checkerboard size corners 
#define ROWS 7
#define COLS 10

int main()
{
    calibrateStereoCamera("../images/left", "../images/right", ROWS, COLS);
}