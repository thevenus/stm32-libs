
#ifndef BME280_LIBRARY 
#define BME280_LIBRARY


// *** Includes ***
#include "bme280.h"
#include "main.h"


/**
 * ************************************************************
 *                  Macros                                    *
 * ************************************************************
*/
#define BME280DeviceDef(name, address, \
                        temp_oversampling, \
                        pres_oversampling, \
                        humid_oversampling, \
                        filter_coef, resttime) \
struct bme280_dev bme280_ ## name = {\
  .dev_id = address, \
  .intf = BME280_I2C_INTF, \
}; \
struct bme280_settings bme280_conf_ ## name = {\
  .osr_t = temp_oversampling, \
  .osr_p = pres_oversampling, \
  .osr_h = humid_oversampling, \
  .standby_time = resttime, \
  .filter = filter_coef, \
}

#define BME280(name) &bme280_ ## name, &bme280_conf_ ## name
#define BME280Sensor(name) &bme280_ ## name


/**
 * ************************************************************
 *                  Function prototypes                       *
 * ************************************************************
*/
int8_t BME280_Start(struct bme280_dev * sensor, struct bme280_settings * cfg ,I2C_HandleTypeDef * i2c);
int8_t BME280_GetPressure(double * pressure, struct bme280_dev * sensor);

#endif