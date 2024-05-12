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
    /* calculate euclidian distance */
    double distanceAB = PointOperations_calculateEuclidianDistance(POINT_A, POINT_B);
    double distanceBC = PointOperations_calculateEuclidianDistance(POINT_B, POINT_C);
    double distanceAC = PointOperations_calculateEuclidianDistance(POINT_A, POINT_C);

    /* calculate manhattan distance */
    double distanceAB = PointOperations_calculateManhattanDistance(POINT_A, POINT_B);
    double distanceBC = PointOperations_calculateManhattanDistance(POINT_B, POINT_C);
    double distanceAC = PointOperations_calculateManhattanDistance(POINT_A, POINT_C);

    return 0;
}

/* INTERNAL FUNCTIONS *****************************************************************************/