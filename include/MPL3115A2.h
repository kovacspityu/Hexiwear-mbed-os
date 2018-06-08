#ifndef _MPL3115A2_DRIVER_
#define _MPL3115A2_DRIVER_
#include "MPL3115A2_enum.h"
#include "mbed.h"


/*! @class MPL3115A2 
Class that implements the MPL3115A2 sensor. */
class MPL3115A2
{
    public:
    /*! Constructs an istance with the specified paramenters
    @param mode The mode the sensor starts in, defaults to BAROMETER
    @param ratio The ratio the sensor starts in, defaults to OS_512_MS
    @param timeStep The time step the sensor starts in, defaults to TS_1S
    */
    MPL3115A2(MPL::Mode mode = MPL::BAROMETER, MPL::Os_Ratio ratio = MPL::OS_512_MS, MPL::Time_Step timeStep = MPL::TS_1S);

    /*! @struct mail_t Contains a single sensor measurement.
    */
    typedef struct {
        MPL::Data_Type type;
        float value;
    } mail_t;
    /*! The Mail container for the data measurements.
    This is where the class will store all measurements linked with an Interrupt.*/
    Mail<mail_t, 48> mailBox;
    
    
    /*! Activates the MPL3115A2 sensor*/
    void setActive();
    /*! Puts the MPL3115A2 sensor in standby*/
    void standby();
    /*! Puts the MPL3115A2 sensor through a whole standby to active cycle.*/
    void refresh();
    /*! Uses the MPL3115A2 sensor reset function,
        which resets all registers to default values*/
    void fullReset();
    /*! Checks if any new measurement has happened, relies on flags in EVENT_CONFIG
        been set.
        @returns true if new data is available.
        TODO Check why it doesn't work*/
    bool isDataAvailable();
    /*! Gets altimeter or barometer data, depending on the active mode of the MPL3115A2 sensor.
        @returns a float containing the value converted in SI units.*/
    float getData();
    /*! Gets the temperature data.
    @returns a float containing the temperature in Celsius unit*/
    float getTemperature();
    /*! Gets the difference between the last two measurement.
    @returns a float containing the difference value converted in SI units*/
    float getDataDelta();
    /*! Gets the difference between the last two temperature measurement.
    @returns a float containing the difference value in Celsius/Kelvin.*/
    float getTemperatureDelta();
    /*! Gets the maximum measurement by the MPL3115A2 sensor.
    @returns a float containing the maximum measurement converted in SI units.*/
    float getMaxData();
    /*! Gets the maximum temperature measurement.
    @returns a float containing the maximum temperature recorded in Celsius.*/
    float getMaxTemperature();
    /*! Gets the minimum measurement by the MPL3115A2 sensor.
    @returns a float containing the minimum measurement converted in SI units.*/
    float getMinData();
    /*! Gets the minimum temperature measurement.
    @returns a float containing the minimum temperature recorded in Celsius.*/
    float getMinTemperature();
    /*! Allows access to the static variable containing the active interrupts on PIN_ONE
    @returns activeInterruptsOne*/
    uint8_t getActiveInterruptsOne();
    /*! Allows access to the static variable containing the active interrupts on PIN_TWO
    @returns activeInterruptsTwo*/
    uint8_t getActiveInterruptsTwo();
    /*! Reads the DR_STATUS register, which contains info on new measurements and
        data having been overwritten without being read.
        @returns the register as a uint8_t*/
    uint8_t getStatus();
    /*! Gets the current Oversample
    @returns the Oversample as a MPL::Os_Ratio*/
    MPL::Os_Ratio getOsR();
    /*! Sets the Oversample
    @param ratio the Oversample desired.*/
    void setOsR(MPL::Os_Ratio ratio);
    /*! Gets the current time step between measurements.
    @returns the measurement as a MPL::Time_Step*/
    MPL::Time_Step getTimeStep();
    /*! Sets the time step between measurements.
    @param timeStep the time step desired.*/
    void setTimeStep(MPL::Time_Step timeStep);
    /*! Activates an interrupt and attaches a function to it.
        The function will need to get the measurements themselves from mailBox
    @param pin the pin that will emit the signal for the interrupt
    @param name the interrupt type
    @param function the function to call when the data has been saved in the mailBox
    @param target optional value for specific interrupts that have a threshold or window value to check against, defaults to 0.*/
    void setInterrupt(MPL::Interrupt_Pin pin, MPL::Interrupt name, void (*function)(), float target = 0);   
    /*! Activates the FIFO interrupt and attaches a function to it.
        It's an overloads the other method in case of FIFO interrupt, as it uses different parameters.
    @param pin the pin that will emit the signal for the interrupt
    @param name the interrupt type
    @param function the function to call when the data has been saved in the mailBox
    @param overflow allows the FIFO buffer to be overflowed if true
    @param watermark sends an interrupts signal after watermark number of measurements
                     have been recorded, defaults to inactive */ 
    void setInterrupt(MPL::Interrupt_Pin pin, MPL::Interrupt name, void (*function)(), bool overflow, uint8_t watermark = 0);
    /*! Deactivates an interrupt.
    @param name the interrupt name to deactivate.*/
    void removeInterrupt(MPL::Interrupt name);
    /*! Gets the internal variable storing the MPL3115A2 sensor current mode.
    @returns true if the MPL3115A2 sensor is in ALTIMETER mode.*/
    bool isAltimeter();
    /*! Gets the internal variable storing whether 
        the MPL3115A2 sensor is currently in FIFO mode.
    @returns true if the MPL3115A2 sensor is in FIFO mode.*/
    bool isFIFO();
    /*! Sets the MPL3115A2 sensor mode.
    @param mode the desired mode.*/
    void setMode(MPL::Mode mode);
    /*! Sets the reference sea level pressure used
        in internal calculations to determine altitude.
        @param pressure the sea level pressure at the user's coordinates.
        Calling the function with no parameter will prompts 
        a measurement of the current pressure and use that as a reference.
        TODO Check if it's necessary to use raw data and make the sensor wait until it has a new measurement.*/
    void setSeaLevelPressure(float pressure = 0);
    /*! Sets a pressure offset.
    @param pressure the offset desired, expressed in whole Pascals. Range is -512 Pa to +508 Pa */
    void setPressureOffset(int8_t pressure);
    /*! Sets a temperature offset.
    @param temperature the offset desired. Range is -9°C to +7.9375°C*/
    void setTemperatureOffset(float temperature);
    /*! Sets the Altitude offset.
    @param altitude the desired offset in meters. Range is -128 m to 127 m*/
    void setAltitudeOffset(int8_t altitude);

    /*! Utility function to take the data as stored in the MPL3115A2 register and convert it to meters.
    @param altitude pointer to a 3-uint8_t array with the altitude data.*/
    float convertAltitudeI2D(uint8_t* altitude);
    /*! Utility function to take the data as stored in the MPL3115A2 register and convert it to Pascals.
    @param altitude pointer to a 3-uint8_t array with the pressure data.*/
    float convertPressureI2D(uint8_t* pressure);

    private:

    I2C mI2C;
    uint8_t mAddress;
    Thread threadOne;
    Thread threadTwo;
    InterruptIn mInterruptOne, mInterruptTwo;
    /*! @var    visAltemeter Stores the mode of the MPL3115A2 sensor, shared among all class instances */
    static bool visAltimeter;
    /*! @var    visFifo Stores whether the MPL3115A2 sensor is in FIFO mode, shared among all class instances*/
    static bool visFIFO;
    /*! @var    activeInterruptsOne, activeInterruptsTwo store the interrupts 
                that have been configured on each pin of the MPL3115A2 sensor,
                shared among all class instances.*/
    static uint8_t activeInterruptsOne, activeInterruptsTwo;
    void interruptWrapper(MPL::Interrupt_Pin pin);
    void interruptWrapperOne();
    void interruptWrapperTwo();
    void (*MPL3115A2InterruptOne)();
    void (*MPL3115A2InterruptTwo)();
    
    void read(MPL::Address address, uint8_t *data, int length = 1);
    int  write(MPL::Address address, uint8_t *data, int length = 1);

    void dispatchInterruptData(MPL::Interrupt_Pin pin);
    MPL::Interrupt identifyInterrupt(MPL::Interrupt_Pin pin);
    void setInterruptFunction(void (*function)(), MPL::Interrupt_Pin pin);
    void dispatchInterruptDataOne();
    void dispatchInterruptDataTwo();

    void convertPressureD2I(float pressure, uint8_t* out);
    void convertAltitudeD2I(float altitude, uint8_t* out);
    uint8_t convertTemperatureD2I(float temperature);
    float convertTemperatureI2D(uint8_t* temperature);
    
    
};

#endif
