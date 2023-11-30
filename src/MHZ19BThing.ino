#include <Thing.h>
#include <BlinkPattern.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <MHZ19.h>

using namespace ootb;

#define PIN_TX D1
#define PIN_RX D2

Thing thing;
BlinkPattern led(BUILTIN_LED);

SoftwareSerial ss(PIN_RX, PIN_TX);
MHZ19 mhz(&ss);

BlinkPattern::Pattern<2> panic{{1, 1}, 25};
BlinkPattern::Pattern<2> start{{1, 9}, 25};
BlinkPattern::Pattern<2> normal{{1, 39}, 25};
BlinkPattern::Pattern<2> on{{1,0}, 1000};
BlinkPattern::Pattern<2> off{{0,1}, 1000};

void setup()
{
    Serial.begin(230400);
    Serial.println();
    Serial.println("Client:" + thing.clientId());

    led.begin();
    led.setPattern(start);

    ss.begin(9600);
    //mhz.useLimit(5000);
    mhz.setAutoCalibration(false);

    thing.onStateChange([](const String &msg)
    {
        Serial.println(msg);
    });

    thing.begin();

    String topic = "things/" + thing.clientId() + "/mhz19b/values";
    thing.addSensor(topic, 1000, [](Value &value)
    {
        MHZ19_RESULT response = mhz.retrieveData();
        if (response == MHZ19_RESULT_OK)
        {
            int co2 = mhz.getCO2();
            int minCO2 = mhz.getMinCO2();
            int temperature = mhz.getTemperature();
            int accuracy = mhz.getAccuracy();
            Serial.print(F("CO2: "));
            Serial.println(co2);
            Serial.print(F("Min CO2: "));
            Serial.println(minCO2);
            Serial.print(F("Temperature: "));
            Serial.println(temperature);
            Serial.print(F("Accuracy: "));
            Serial.println(accuracy);
            const int capacity = JSON_OBJECT_SIZE(5);
            StaticJsonBuffer<capacity> jb;
            JsonObject& doc = jb.createObject();
            //DynamicJsonDocument doc(1024);
            doc["co2"] = co2;
            doc["CO2"] = co2;
            doc["minCO2"] = minCO2;
            doc["temperature"] = temperature;
            doc["accuracy"] = accuracy;
            String output;
            doc.printTo(output);
            value = output;
            Serial.println(output);
            led.setPattern(off);
        }
        else
        {
            Serial.print(F("Error, code: "));
            Serial.println(response);
        }
    });

    String topicDisplay = "things/" + thing.clientId() + "/display";
    thing.addActuator(topicDisplay, [](Value &value)
    {
        Serial.println("Got " + String(value));
    });

    thing.addActuator("things/" + thing.clientId() + "/mhz19b/configure", [](Value &value)
    {
        StaticJsonBuffer<1024> jsonBuffer;
        String msg = value;
        JsonObject& root = jsonBuffer.parseObject(msg);

        if (!root.success()) {
            Serial.println("parseObject() failed");
            return false;
        }
        bool autoCalibrate = root["autoCalibrate"];
        mhz.setAutoCalibration(autoCalibrate);
        uint16_t range = root["range"];
        MHZ19_RANGE mhz19_range = parseRange(range);
        mhz.setRange(mhz19_range);
        return true;
    });

    led.setPattern(normal);
}

MHZ19_RANGE parseRange(uint16_t range)
{
    switch(range)
    {
        case 1000: return MHZ19_RANGE_1000;
        case 2000: return MHZ19_RANGE_2000;
        case 3000: return MHZ19_RANGE_3000;
        case 5000: return MHZ19_RANGE_5000;
        case 10000: return MHZ19_RANGE_10000;
        default: return MHZ19_RANGE_2000;
    }
    return MHZ19_RANGE_2000;
}

void loop()
{
    thing.handle();
    led.handle();
}
