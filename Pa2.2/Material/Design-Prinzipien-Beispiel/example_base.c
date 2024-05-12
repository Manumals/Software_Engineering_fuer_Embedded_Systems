/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/

/***************************************************************************************************
 * @file example.c
 *
 * This file represents a example module to show the diffrent design principles.
 **************************************************************************************************/

/* Includes ***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Constants **************************************************************************************/

/* Macros *****************************************************************************************/

/* Typedefinitions ********************************************************************************/

/** Struct of a Point. */
typedef struct tag_Point
{
    int xPosition;  /**< x-Position of point. */
    int yPosition;  /**< y-Position of point. */
} Point;

/* Prototypes *************************************************************************************/

/**
 * Calculate eucledian distance between two points.
 * 
 * @param[in] firstPoint First point for distance calculation.
 * @param[in] secondPoint Second point for distance calculation.
 * 
 * @return Calculated distance.
*/
static double calculateEucledianDistance(Point firstPoint, Point secondPoint);

/* Variables **************************************************************************************/

/* Functions **************************************************************************************/

int main(void) 
{
    Point pointA = {0, 0};
    Point pointB = {3, 4};
    Point pointC = {4, 7};

    /* calculate euclidian distance */
    double distanceEucAB = calculateEucledianDistance(pointA, pointB);
    double distanceEucBC = sqrt(pow((pointB.xPosition - pointC.xPosition), 2) + pow((pointB.yPosition - pointC.yPosition), 2));
    double distanceEucAC = sqrt(pow((pointA.xPosition - pointC.xPosition), 2) + pow((pointA.yPosition - pointC.yPosition), 2));

    /* calculate manhattan distance */
    double distanceManAB = abs(pointA.xPosition - pointB.xPosition) + abs(pointA.yPosition - pointB.yPosition);
    double distanceManBC = abs(pointB.xPosition - pointC.xPosition) + abs(pointB.yPosition - pointC.yPosition);
    double distanceManAC = abs(pointA.xPosition - pointC.xPosition) + abs(pointA.yPosition - pointC.yPosition);

    return 0;
}

double calculateEucledianDistance(Point firstPoint, Point secondPoint)
{
    return sqrt(pow((firstPoint.xPosition - secondPoint.xPosition), 2) + pow((firstPoint.yPosition - secondPoint.yPosition), 2));
}