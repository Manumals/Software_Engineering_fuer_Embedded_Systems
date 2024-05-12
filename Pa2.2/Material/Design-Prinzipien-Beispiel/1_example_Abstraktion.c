/***************************************************************************************************

  (c) NewTec GmbH System-Entwicklung und Beratung 2024   -   www.newtec.de

***************************************************************************************************/

/***************************************************************************************************
 * @file example.c
 *
 * This file represents a example module to show the diffrent design principles.
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/

#include <stdio.h>

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/* PROTOTYPES *************************************************************************************/

/** Struct of a Point. */
typedef struct tag_Point
{
    int xPosition;  /**< x-Position of point. */
    int yPosition;  /**< y-Position of point. */
} Point;

/* VARIABLES **************************************************************************************/

/** Calculate the euclidian distance between two points.
 * 
 * @param[in] firstPoint First point used for distance calculation.
 * @param[in] secondPoint Second point used for distance calculation.
 * 
 * @return Calculated euclidian distance between two points.
*/
static double calculateEuclidianDistance(Point firstPoint, Point secondPoint);

/** Calculate the manhattan distance between two points.
 * 
 * @param[in] firstPoint First point used for distance calculation.
 * @param[in] secondPoint Second point used for distance calculation.
 * 
 * @return Calculated manhattan distance between two points.
*/
static double calculateManhattanDistance(Point firstPoint, Point secondPoint);

/* EXTERNAL FUNCTIONS *****************************************************************************/

int main(void) 
{
    Point pointA = {0, 0};
    Point pointB = {3, 4};
    Point pointC = {4, 7};

    /* calculate euclidian distance */
    double distanceAB = calculateEuclidian(pointA, pointB);
    double distanceBC = calculateEuclidian(pointB, pointC);
    double distanceAC = calculateEuclidian(pointA, pointC);

    /* calculate manhattan distance */
    double distanceAB = calculateManhattanDistance(pointA, pointB);
    double distanceBC = calculateManhattanDistance(pointB, pointC);
    double distanceAC = calculateManhattanDistance(pointA, pointC);

    return 0;
}

/* INTERNAL FUNCTIONS *****************************************************************************/

static double calculateEuclidian(Point firstPoint, Point secondPoint)
{
    return sqrt(pow((firstPoint.xPosition - secondPoint.xPosition), 2) + pow((firstPoint.yPosition - secondPoint.yPosition), 2));
}

static double calculateManhattanDistance(Point firstPoint, Point secondPoint)
{
    return abs(firstPoint.xPosition - secondPoint.xPosition) + abs(firstPoint.yPosition - secondPoint.yPosition);
}