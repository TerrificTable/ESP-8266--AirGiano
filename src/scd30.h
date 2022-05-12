#include <Arduino.h>

#include "SparkFun_SCD30_Arduino_Library.h"



typedef struct {
    int             Co2             = 0;
    float           Temperature     = 0;
    int             Humidity        = 0;

    String          SerialNumber    = "n/a";
    String          Firmware        = "n/a";
    unsigned long   TimeOut         = millis() + 10000;

    boolean         Connected       = false;

} SDC30Data;

SDC30Data ConfigSDC30;
SCD30 airSensor;


void SDC30() {

    if (!ConfigSDC30.Connected) {
        // DisplayText("Wait 2 Seconds");

        if (airSensor.begin() == false) {
            Serial.println("Air sensor not detected. Please check wiring. Restarting in 5sec...");
            delay(5000);
            ESP.restart();
        }

        airSensor.setTemperatureOffset(0);

        ConfigSDC30.Connected = true;
    }




    static boolean SDC30Connected = false;
    static int RingBufferSize = 126;
    static int RingBufferPos = -1;
    static float Data[126][3]; // 0=CO2 1=Temp 3=Hum

    if (!SDC30Connected) {
        Serial.println("SDC30:Init");

        for (int a = 0; a < RingBufferSize; a++) {
            Data[a][0] = -99.9;
            Data[a][1] = -99.9;
            Data[a][2] = -99.9;
        }
        RingBufferPos = 0;

        if (airSensor.begin(Wire) == false) {
            Serial.println("SDC30:Sensor not detected. Please check wiring. Now disabled!");
            delay(5000);
            ESP.restart();
        }

        airSensor.setAutoSelfCalibration(false); // Sensirion no auto calibration
        airSensor.setMeasurementInterval(2);
        airSensor.setTemperatureOffset(5);
        SDC30Connected = true;
    }

    if (!SDC30Connected)
        return;


    int CurrCo2 = ConfigSDC30.Co2;
    float CurrTemperature = ConfigSDC30.Temperature;
    int CurrHumidity = ConfigSDC30.Humidity;


    if (airSensor.dataAvailable()) {
        if (airSensor.getCO2() != 0) {
            ConfigSDC30.TimeOut = millis() + 10000;
            ConfigSDC30.Co2 = airSensor.getCO2();
            ConfigSDC30.Temperature = airSensor.getTemperature();
            ConfigSDC30.Humidity = airSensor.getHumidity();
        }
    }


    RingBufferPos++;

    if (RingBufferPos == RingBufferSize)
    {
        RingBufferPos = 0;
    }
    Data[RingBufferPos][0] = ConfigSDC30.Co2;
    Data[RingBufferPos][1] = ConfigSDC30.Temperature;
    Data[RingBufferPos][2] = ConfigSDC30.Humidity;


    if (millis() >= ConfigSDC30.TimeOut) {
        Serial.println("001 Sensort, SDC30 ERROR");

        #if DEBUG
            Msg(F("SDC30"), F("TimeOUT!"));
        #endif
    }



    if (CurrCo2 != ConfigSDC30.Co2 or CurrTemperature != ConfigSDC30.Temperature or CurrHumidity != ConfigSDC30.Humidity)
    {
        Serial.printf("\nCO2: %i\n TEMPERATURE: %.1f\n HUMIDITY: %i%\n \n", ConfigSDC30.Co2, ConfigSDC30.Temperature, airSensor.getHumidity());

        // DisplayText(currentTime()  +  \
        //             "\n" + "CO2: " + String(ConfigSDC30.Co2) +
        //             "\n" + "C:   " + String(ConfigSDC30.Temperature) +
        //             "\n" + "lf%: " + String(airSensor.getHumidity()));

        sendMQTT("{ \"CO2\": " + String(ConfigSDC30.Co2) + ", \"Temp\": " + String(ConfigSDC30.Temperature) + ", \"Hum\": " + String(airSensor.getHumidity()) + " }");
        
    }
}
