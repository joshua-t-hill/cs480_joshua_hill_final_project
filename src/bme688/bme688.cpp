#include "Adafruit_BME680.h"
#include "Ubidots.h"

// Globals
const String LOG_INITIALIZATION_FAILED = "BME688 could not be initialiased, check sensor.";
const String LOG_READING_FAILED = "BME0688 reading failed";
const String LOG_READINGS = "BME688 readings:\n\tTemp Celsius: %f\n\tHumidity %%: %f\n\tPressure hPa: %f\n\tGas res. KOhms: %f";

extern Ubidots ubidots; // Defined in main src file
Adafruit_BME680 bme; // I2C
double temperatureInC = 0;
double relativeHumidity = 0;
double pressureHpa = 0;
double gasResistanceKOhms = 0;

/// @brief The BME loop method reports the current readings from the BME688; temp, rel. humidity, air pressure, and gas resistance (WIP).
void bmeLoop()
{
    if (! bme.performReading()) 
    {
        Log.info(LOG_READING_FAILED);
    }
    else
    {
        temperatureInC = bme.temperature;
        relativeHumidity = bme.humidity;
        pressureHpa = bme.pressure / 100.0;
        gasResistanceKOhms = bme.gas_resistance / 1000.0; // Need a better understanding of what the resistance correlates to in terms of air composition; come back to later if time allows.

        Log.info(LOG_READINGS,
                 temperatureInC,
                 relativeHumidity,
                 pressureHpa,
                 gasResistanceKOhms);

        ubidots.add("bme688_temperature_celsius"      , temperatureInC);
        ubidots.add("bme688_humidity_percentage"      , relativeHumidity);
        ubidots.add("bme688_air_pressure_hectopascals", pressureHpa);
    }
}

/// @brief Setup the BME688 with the desired sampling settings.
void bmeSetup()
{
    if(!bme.begin()) // Default i2c address (when not specified) is 0x77
    {
        Log.info(LOG_INITIALIZATION_FAILED);
    }
    else
    {
        // Set up oversampling and filter initialization
        bme.setTemperatureOversampling(BME680_OS_8X);
        bme.setHumidityOversampling(BME680_OS_2X);
        bme.setPressureOversampling(BME680_OS_4X);
        bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
        bme.setGasHeater(320, 150); // 320*C for 150 ms
    }
}