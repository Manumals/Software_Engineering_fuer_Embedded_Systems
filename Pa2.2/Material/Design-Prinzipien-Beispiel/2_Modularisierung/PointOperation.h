/***************************************************************************************************
  (c) NewTec GmbH 2024   -   www.newtec.de
***************************************************************************************************/
/**
 * @file       PointOperation.h
 *
 * This module contains existing points and provide operations to calculate distances.
 */
/**************************************************************************************************/
#ifndef POINTOPERATION_H
#define POINTOPERATION_H

/* INCLUDES ***************************************************************************************/

/* CONSTANTS **************************************************************************************/

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/

/** Struct of a Point. */
typedef struct tag_Point
{
    int xPosition;  /**< x-Position of point. */
    int yPosition;  /**< y-Position of point. */
} Point;

/** Enum of different existing points. */
typedef enum tag_PointId
{
    POINT_A, /**< Point A. */
    POINT_B, /**< Point B. */
    POINT_C  /**< Point C. */
} PointId;

/* PROTOTYPES *************************************************************************************/

/** Init PointOperations */
extern void PointOperations_init(void);

/** Calculate the euclidian distance between two points.
 * 
 * @param[in] firstPoint First point used for distance calculation.
 * @param[in] secondPoint Second point used for distance calculation.
 * 
 * @return Calculated euclidian distance between two points.
*/
extern double PointOperations_calculateEuclidianDistance(Point firstPoint, Point secondPoint);

/** Calculate the manhattan distance between two points.
 * 
 * @param[in] firstPoint First point used for distance calculation.
 * @param[in] secondPoint Second point used for distance calculation.
 * 
 * @return Calculated manhattan distance between two points.
*/
extern double PointOperations_calculateManhattanDistance(Point firstPoint, Point secondPoint);

extern Point *PointOperations_getPointPointer(PointId pointId);

#endif /* POINTOPERATION_H */