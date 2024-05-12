/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       PointOperation.c
 *
 * This module contains existing points and provide operations to calculate distances.
 */
/**************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include <stdio.h>
#include "PointOperation.h"

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

static Point* getPoint(PointId pointId);

/* VARIABLES **************************************************************************************/

static Point pointA;
static Point pointB;
static Point pointC;

/* EXTERNAL FUNCTIONS *****************************************************************************/

extern void PointOperations_init(void)
{
    pointA.xPosition = 0;
    pointA.yPosition = 0;
    pointB.xPosition = 3;
    pointB.yPosition = 4;
    pointC.xPosition = 4;
    pointC.yPosition = 7;
}

extern double PointOperations_calculateEuclidianDistance(PointId firstPointId, PointId secondPointId)
{
    Point* firstPoint = getPoint(firstPointId);
    Point* secondPoint = getPoint(secondPointId);

    return sqrt(pow((firstPoint->xPosition - secondPoint->xPosition), 2) + pow((firstPoint->yPosition - secondPoint->yPosition), 2));
}

extern double PointOperations_calculateManhattanDistance(PointId firstPointId, PointId secondPointId)
{
    Point* firstPoint = getPoint(firstPointId);
    Point* secondPoint = getPoint(secondPointId);

    return abs(firstPoint->xPosition - secondPoint->xPosition) + abs(firstPoint->yPosition - secondPoint->yPosition);
}

/* INTERNAL FUNCTIONS *****************************************************************************/

static Point* getPoint(PointId pointId)
{
    Point* pointToReturn;

    switch (pointId)
    {
        case POINT_A:
            pointToReturn = &pointA;
            break;
        case POINT_B:
            pointToReturn = &pointB;
            break;
        case POINT_C:
            pointToReturn = &pointC;
            break;
        default:
            break;
    }

    return pointToReturn;
}