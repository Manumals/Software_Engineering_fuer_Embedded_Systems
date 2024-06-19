/***************************************************************************************************
  (c) NewTec GmbH System-Entwicklung und Beratung 2015   -   www.newtec.de
  $URL: https://svn.newtec.zz/NTCampus/SW-Entwicklung/trunk/system/50_Implementierung/Projekte/Linienfolger/20_Beistellung/Delivery/Beistellung_r300/Coding/lib/service_target/service/LineSensor.h $

  (c) Team 🏁~~ ō͡≡o\ (Maurice Ott, Simon Walderich, Thorben Päpke) 2024
***************************************************************************************************/
/**
@addtogroup Service
@{
@file       LineSensor.h

Functions to control the 5 Line sensors on the Pololu Zumo.


@version    %$Id: LineSensor.h 300 2024-02-01 12:58:40Z leglaz $
* @}
***************************************************************************************************/
#ifndef LINESENSOR_H
#define LINESENSOR_H

#ifdef __cplusplus
extern "C"
{
#endif

/* INCLUDES ***************************************************************************************/
#include "Types.h"

/* CONSTANTS **************************************************************************************/

/** Maximum sensor read delay. */
#define LINESENSOR_READ_TIMEOUT_US    (2000U)

/** Number of sensors. */
#define LINESENSOR_COUNT            (5U)

#define LINESENSOR_LEFT             (0U)   /**< leftmost sensor */
#define LINESENSOR_MIDDLE_LEFT      (1U)   /**< middle left sensor */
#define LINESENSOR_MIDDLE           (2U)   /**< middle sensor */
#define LINESENSOR_MIDDLE_RIGHT     (3U)   /**< middle right sensor */
#define LINESENSOR_RIGHT            (4U)   /**< rightmost sensor */

/** Normalized value range. */
#define LINESENSOR_NORMALIZED_RANGE   (255U)

/* MACROS *****************************************************************************************/

/* TYPES ******************************************************************************************/
/** Calibration data for a line sensor. */
typedef struct tag_LineSensorCalibrationData
{
    UInt8 minVal; /**< Minimum values obtained during calibration.     */
    UInt8 maxVal; /**< Maximum values obtained during calibration.     */
    UInt8 range; /**< Calibration range.                               */
} LineSensorCalibrationData;

/**
 * Type holding line sensor values.
 */
typedef struct tag_LineSensorValues
{
    UInt8 value[LINESENSOR_COUNT];      /*< Line detect confidence in percent.   */
} LineSensorValues;

/**
 * Type holding line sensor calibration values.
 */
typedef struct tag_LineSensorCalibration
{
    bool   calibrated[LINESENSOR_COUNT]; /*< indicate if line sensor is calibrated.    */
} LineSensorCalibration;

/* PROTOTYPES *************************************************************************************/
/**
 * Initialize module.
 */
extern void LineSensor_init(void);

/**
 * Deinitialize module.
 */
extern void LineSensor_deinit(void);

/** Start calibrating the LineSensors.
*/
extern void LineSensor_startCalibration(void);

/** Stop calibrating the LineSensors.
*/
extern void LineSensor_stopCalibration(void);

/** Get status of calibrations.
 * @return  bool status.
 * @retval  true
 *              All sensors calibrated succesfully.
 * @retval  false
 *              Not all sensors calibrated yet.
 */
extern bool LineSensor_getCalibrationState(void);

/** Get calibration data
 * @param[out] sensorNo The sensor no for which the calibration data should be returned
 * @return calibration data of the given sensor
 */
extern LineSensorCalibrationData LineSensor_getCalibrationData(UInt8 sensorNo);

/** Read the LineSensors.
*
* @param[out] sensorValues The sensor values from the LineSensor.
*/
extern void LineSensor_read(LineSensorValues * sensorValues, LineSensorCalibration *calib);

/** Read raw (uncalibrated) sensor values.
 *  Reading is done by charging the sensors for ~10Us, then measure the time until the drop
 *  to low level again. A darker ground means a slower voltage drop.
 * @param[out] data sensor data value container.
 */
extern void LineSensor_readRaw(LineSensorValues * data);

/** Turn Emitters on. */
extern void LineSensor_enableEmitter(void);

/** Turn Emitters off */
extern void LineSensor_disableEmitter(void);

#endif  /* LINESENSOR_H */
