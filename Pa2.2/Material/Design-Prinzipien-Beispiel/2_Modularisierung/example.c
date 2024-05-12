/***************************************************************************************************

  (c) NewTec GmbH System-Entwicklung und Beratung 2024   -   www.newtec.de

***************************************************************************************************/

/***************************************************************************************************
@file example.c

@brief This file represents a example module to show the diffrent design principles.
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include <stdio.h>
#include "PointOperation.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/* VARIABLES **************************************************************************************/

/* EXTERNAL FUNCTIONS *****************************************************************************/

int main(void) 
{
    Point *pointA = PointOperations_getPointPointer(POINT_A);
    Point *pointB = PointOperations_getPointPointer(POINT_B);
    Point *pointC = PointOperations_getPointPointer(POINT_C);

    /* calculate euclidian distance */
    double distanceAB = PointOperations_calculateEuclidianDistance(*pointA, *pointB);
    double distanceBC = PointOperations_calculateEuclidianDistance(*pointB, *pointC);
    double distanceAC = PointOperations_calculateEuclidianDistance(*pointA, *pointC);

    /* calculate manhattan distance */
    double distanceAB = PointOperations_calculateManhattanDistance(*pointA, *pointB);
    double distanceBC = PointOperations_calculateManhattanDistance(*pointB, *pointC);
    double distanceAC = PointOperations_calculateManhattanDistance(*pointA, *pointC);

    return 0;
}

/* INTERNAL FUNCTIONS *****************************************************************************/