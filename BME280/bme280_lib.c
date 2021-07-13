
#include "bme280_lib.h"
#include <stdio.h>

int8_t i2c_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t * data, uint16_t len);
int8_t i2c_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t * data, uint16_t len);
void print_rslt(const char api_name[], int8_t rslt);

I2C_HandleTypeDef * i2c_conf;
/**
 * BME280_Start: This function establishes connection to the BME sensor and
 *                  initializes it with given parameters;
 * Arguments:
 *    [0] bme280_dev * sensor: pointer to the sensor main struct
 *    [1] bme280_settings * cfg: pointer to the sensor configuration struct
 *    [2] I2C_HandleTypeDef * i2c: pointer to i2c configuration 
 * Note:
 *    BME280DeviceDef(...) macro creates main and settings structs with the given name
 *    BME280(name) macro returns main and settings structs with given name (thus can be used to enter values)
 * 
*/
int8_t BME280_Start(struct bme280_dev * sensor, struct bme280_settings * cfg, I2C_HandleTypeDef * i2c){
  // variables 
  uint8_t result;

  // setting i2c for I/O
  i2c_conf = i2c;

  if ((cfg != NULL) && (sensor != NULL)){
    // mapping API functions
    sensor->delay_ms = HAL_Delay;
    sensor->read = i2c_read;
    sensor->write = i2c_write;

    // reset and read chip-id and calib-data from sensor 
    result = bme280_init(sensor);

    if (result == BME280_OK){
      // get current settings from the sensor
      result = bme280_get_sensor_settings(sensor);

      if (result == BME280_OK){
        // setting desired configurations of the sensor 
        sensor->settings.osr_t = cfg->osr_t;
        sensor->settings.osr_p = cfg->osr_p;
        sensor->settings.osr_h = cfg->osr_h;
        sensor->settings.filter = cfg->filter;
        sensor->settings.standby_time = cfg->standby_time;

        result = bme280_set_sensor_settings(BME280_ALL_SETTINGS_SEL, sensor);

        if (result == BME280_OK){        
          // setting the power mode 
          result = bme280_set_sensor_mode(BME280_NORMAL_MODE, sensor);
        }
        else {
          print_rslt(" Set config status", result);
        }

      }
      else {
        print_rslt(" Get config status", result);
      }
      
    }
    else {
      print_rslt(" Init status", result);
    }

  }
  else {
    result = BME280_E_NULL_PTR;
  }

  return result;

}

int8_t BME280_GetPressure(double * pressure, struct bme280_dev * sensor){
  int8_t result;
  struct bme280_data temporary;

  if (sensor != NULL){
    result = bme280_get_sensor_data(BME280_PRESS, &temporary, sensor);
    if (result == BME280_OK){
      *pressure = (double)temporary.pressure / 100;
    }
  
  }
  else {
    result = BME280_E_NULL_PTR;
  }

  return result;

}


int8_t i2c_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t * data, uint16_t len) {
  
  i2c_addr <<= 1;

  int8_t result = HAL_I2C_Mem_Write(i2c_conf, i2c_addr, (uint16_t)reg_addr,\
                            I2C_MEMADD_SIZE_8BIT, data, len, HAL_MAX_DELAY );

  if (result == HAL_OK)
    result = BME280_OK;
  else 
    result = BME280_E_COMM_FAIL;

  return result;

}

int8_t i2c_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t * data, uint16_t len){

  i2c_addr <<= 1;

  int8_t result = HAL_I2C_Mem_Read(i2c_conf, i2c_addr, (uint16_t)reg_addr, \
                                   I2C_MEMADD_SIZE_8BIT, data, len, HAL_MAX_DELAY);

  if (result == HAL_OK){
    result = BME280_OK;
  }
  else 
    result = BME280_E_COMM_FAIL;

  return result;

}

void print_rslt(const char api_name[], int8_t rslt)
{
  if (rslt != BME280_OK)
  {
    printf("%s\t", api_name);
    if (rslt == BME280_E_NULL_PTR)
    {
      printf("Error [%d] : Null pointer error\r\n", rslt);
    }
    else if (rslt == BME280_E_COMM_FAIL)
    {
      printf("Error [%d] : Bus communication failed\r\n", rslt);
    }
    else if (rslt == BME280_E_DEV_NOT_FOUND)
    {
      printf("Error [%d] : Device not found\r\n", rslt);
    }
    else
    {
      /* For more error codes refer "*_defs.h" */
      printf("Error [%d] : Unknown error code\r\n", rslt);
    }
  }
}
