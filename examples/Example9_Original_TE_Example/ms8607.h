#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

enum ms8607_humidity_i2c_master_mode { ms8607_i2c_hold, ms8607_i2c_no_hold };

enum ms8607_status {
  ms8607_status_ok,
  ms8607_status_no_i2c_acknowledge,
  ms8607_status_i2c_transfer_error,
  ms8607_status_crc_error,
  ms8607_status_heater_on_error
};

enum ms8607_humidity_resolution {
  ms8607_humidity_resolution_12b = 0,
  ms8607_humidity_resolution_8b,
  ms8607_humidity_resolution_10b,
  ms8607_humidity_resolution_11b
};

enum ms8607_battery_status { ms8607_battery_ok, ms8607_battery_low };

enum ms8607_heater_status { ms8607_heater_off, ms8607_heater_on };

enum ms8607_pressure_resolution {
  ms8607_pressure_resolution_osr_256 = 0,
  ms8607_pressure_resolution_osr_512,
  ms8607_pressure_resolution_osr_1024,
  ms8607_pressure_resolution_osr_2048,
  ms8607_pressure_resolution_osr_4096,
  ms8607_pressure_resolution_osr_8192
};

enum i2c_status_code {
  i2c_status_ok = 0x00,
  i2c_status_err_overflow = 0x01,
  i2c_status_err_timeout = 0x02,
};

class ms8607 {

public:
  ms8607();

  /**
   * \brief Perform initial configuration. Has to be called once.
   */
  void begin();

  /**
  * \brief Check whether MS8607 device is connected
  *
  * \return bool : status of MS8607
  *       - true : Device is present
  *       - false : Device is not acknowledging I2C address
  */
  bool is_connected(void);

  /**
   * \brief Reset the MS8607 device
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   */
  enum ms8607_status reset(void);

  /**
   * \brief Enable heater
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   */
  enum ms8607_status enable_heater(void);

  /**
   * \brief Disable heater
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   */
  enum ms8607_status disable_heater(void);

  /**
   * \brief Get heater status
   *
   * \param[in] ms8607_heater_status* : Return heater status (above or below
   *2.5V)
   *	                    - ms8607_heater_off,
   *                      - ms8607_heater_on
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   */
  enum ms8607_status get_heater_status(enum ms8607_heater_status *heater);

  /**
   * \brief Provide battery status
   *
   * \param[out] ms8607_battery_status* : Battery status
   *                      - ms8607_battery_ok,
   *                      - ms8607_battery_low
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   */
  enum ms8607_status get_battery_status(enum ms8607_battery_status *bat);

  /**
   * \brief Reads the temperature, pressure and relative humidity value.
   *
   * \param[out] float* : degC temperature value
   * \param[out] float* : mbar pressure value
   * \param[out] float* : %RH Relative Humidity value
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   *       - ms8607_status_crc_error : CRC check error
   */
  enum ms8607_status read_temperature_pressure_humidity(float *t, float *p,
                                                        float *h);

  /******************** Functions from humidity sensor ********************/

  /**
   * \brief Set humidity ADC resolution.
   *
   * \param[in] ms8607_humidity_resolution : Resolution requested
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   *       - ms8607_status_crc_error : CRC check error
   */
  enum ms8607_status
  set_humidity_resolution(enum ms8607_humidity_resolution res);

  /**
   * \brief Set Humidity sensor ADC resolution.
   *
   * \param[in] ms8607_i2c_master_mode : I2C mode
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok
   */
  void set_humidity_i2c_master_mode(enum ms8607_humidity_i2c_master_mode mode);

  /**
   * \brief Returns result of compensated humidity
   *        Note : This function shall only be used when the heater is OFF. It
   * will return an error otherwise.
   *
   * \param[in] float - Actual temperature measured (degC)
   * \param[in] float - Actual relative humidity measured (%RH)
   * \param[out] float *- Compensated humidity (%RH).
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_heater_on_error : Cannot compute compensated humidity
   * because heater is on
   */
  enum ms8607_status get_compensated_humidity(float temperature,
                                              float relative_humidity,
                                              float *compensated_humidity);

  /**
   * \brief Returns the computed dew point
   *        Note : This function shall only be used when the heater is OFF. It
   * will return an error otherwise.
   *
   * \param[in] float - Actual temperature measured (degC)
   * \param[in] float - Actual relative humidity measured (%RH)
   * \param[out] float *- Dew point temperature (DegC).
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_heater_on_error : Cannot compute compensated humidity
   * because heater is on
   */
  enum ms8607_status get_dew_point(float temperature, float relative_humidity,
                                   float *dew_point);

  /******************** Functions from Pressure sensor ********************/
  /**
   * \brief Set pressure ADC resolution.
   *
   * \param[in] ms8607_pressure_resolution : Resolution requested
   *
   */
  void set_pressure_resolution(enum ms8607_pressure_resolution res);

private:
  /******************** Functions from humidity sensor ********************/

  /**
   * \brief Check whether humidity sensor is connected
   *
   * \return bool : status of humidity sensor
   *       - true : Device is present
   *       - false : Device is not acknowledging I2C address
   */
  bool hsensor_is_connected(void);

  /**
   * \brief Reset the humidity sensor part
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   */
  enum ms8607_status hsensor_reset(void);

  /**
   * \brief Check CRC
   *
   * \param[in] uint16_t : variable on which to check CRC
   * \param[in] uint8_t : CRC value
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : CRC check is OK
   *       - ms8607_status_crc_error : CRC check error
   */
  enum ms8607_status hsensor_crc_check(uint16_t value, uint8_t crc);

  /**
   * \brief Reads the MS8607 humidity user register.
   *
   * \param[out] uint8_t* : Storage of user register value
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   */
  enum ms8607_status hsensor_read_user_register(uint8_t *value);

  /**
   * \brief Writes the MS8607 humidity user register with value
   *        Will read and keep the unreserved bits of the register
   *
   * \param[in] uint8_t : Register value to be set.
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   */
  enum ms8607_status hsensor_write_user_register(uint8_t value);

  /**
   * \brief Set Humidity sensor ADC resolution.
   *
   * \param[in] ms8607_i2c_master_mode : I2C mode
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok
   */
  enum ms8607_humidity_i2c_master_mode hsensor_i2c_master_mode;

  /**
   * \brief Reads the relative humidity ADC value
   *
   * \param[out] uint16_t* : Relative humidity ADC value.
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   *       - ms8607_status_crc_error : CRC check error
   */
  enum ms8607_status hsensor_humidity_conversion_and_read_adc(uint16_t *adc);

  /**
   * \brief Reads the relative humidity value.
   *
   * \param[out] float* : %RH Relative Humidity value
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   *       - ms8607_status_crc_error : CRC check error
   */
  enum ms8607_status hsensor_read_relative_humidity(float *humidity);

  /******************** Functions from Pressure sensor ********************/

  /**
   * \brief Check whether MS8607 pressure sensor is connected
   *
   * \return bool : status of MS8607
   *       - true : Device is present
   *       - false : Device is not acknowledging I2C address
   */
  bool psensor_is_connected(void);

  /**
   * \brief Reset the Pressure sensor part
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   */
  enum ms8607_status psensor_reset(void);

  /**
   * \brief Reads the psensor EEPROM coefficient stored at address provided.
   *
   * \param[in] uint8_t : Address of coefficient in EEPROM
   * \param[out] uint16_t* : Value read in EEPROM
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   *       - ms8607_status_crc_error : CRC check error on the coefficients
   */
  enum ms8607_status psensor_read_eeprom_coeff(uint8_t command,
                                               uint16_t *coeff);

  /**
   * \brief Reads the ms8607 EEPROM coefficients to store them for computation.
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   *       - ms8607_status_crc_error : CRC check error on the coefficients
   */
  enum ms8607_status psensor_read_eeprom(void);

  /**
   * \brief CRC check
   *
   * \param[in] uint16_t *: List of EEPROM coefficients
   * \param[in] uint8_t : crc to compare
   *
   * \return bool : TRUE if CRC is OK, FALSE if KO
   */
  bool psensor_crc_check(uint16_t *n_prom, uint8_t crc);

  /**
   * \brief Compute temperature and pressure
   *
   * \param[out] float* : Celsius Degree temperature value
   * \param[out] float* : mbar pressure value
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   *       - ms8607_status_crc_error : CRC check error on the coefficients
   */
  enum ms8607_status psensor_read_pressure_and_temperature(float *temperature,
                                                           float *pressure);

  /**
   * \brief Triggers conversion and read ADC value
   *
   * \param[in] uint8_t : Command used for conversion (will determine
   * Temperature vs Pressure and osr)
   * \param[out] uint32_t* : ADC value.
   *
   * \return ms8607_status : status of MS8607
   *       - ms8607_status_ok : I2C transfer completed successfully
   *       - ms8607_status_i2c_transfer_error : Problem with i2c transfer
   *       - ms8607_status_no_i2c_acknowledge : I2C did not acknowledge
   */
  enum ms8607_status psensor_conversion_and_read_adc(uint8_t cmd,
                                                     uint32_t *adc);

  uint32_t hsensor_conversion_time;
  bool hsensor_heater_on;
  bool psensor_coeff_read;
  uint32_t psensor_conversion_time[6];
};
