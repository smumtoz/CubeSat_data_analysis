#include <MS5611.h>
#include <LM75A.h>
#include <LIS3MDL.h>
#include <STM32SD.h>
#include <LSM6DS3.h>
#include <OneWire.h>
#include <DallasTemperature.h>

using namespace IntroStratLib;

#ifndef SD_DETECT_PIN
#define SD_DETECT_PIN PB8
#endif

HardwareSerial Serial1(PA10, PA9);  // LoRa serial
HardwareSerial Serial2(PA3, PA2);   // USB serial

TwoWire Wire1(PB7, PB6);

MS5611 bar(Wire1, 0x77);
LM75A tempboard(Wire1, 0x4A);
LIS3MDL magn(Wire1, 0x1C);
LSM6DS3 gyro(Wire1, 0x6A);
LSM6DS3 acc(Wire1, 0x6A);

// DS18B20 on PA5
const int SENSOR_PIN = PA5;
OneWire oneWire(SENSOR_PIN);
DallasTemperature ds18b20(&oneWire);

File dataFile;
int dataIndex = 1;

unsigned long lastPrint = 0;

// Sensor values
float b;         // pressure
float tboard;    // LM75A board temp
float tds18b20;  // DS18B20 external temp
float mx, my, mz;
float gx, gy, gz;
float ax, ay, az;

void setup() {
  Serial2.begin(9600);  // USB
  Serial1.begin(9600); // LoRa

  SD.setDx(PC8, PC9, PC10, PC11);
  SD.setCMD(PD2);
  SD.setCK(PC12);

  Wire1.begin();
  bar.Init();
  delay(10);
  tempboard.Init();
  delay(10);
  magn.Init();
  delay(10);
  gyro.InitGyro();
  delay(10);
  acc.InitAccel();
  delay(10);

  ds18b20.begin();  // init DS18B20

  if (SD.begin()) {
    char filename[15];
    while (true) {
      sprintf(filename, "DATA%03d.TXT", dataIndex);
      if (!SD.exists(filename)) {
        dataFile = SD.open(filename, FILE_WRITE);
        break;
      }
      dataIndex++;
    }
  }
}

void loop() {
  // Read 9DOF sensors
  mx = magn.MX();
  my = magn.MY();
  mz = magn.MZ();
  gx = gyro.GX();
  gy = gyro.GY();
  gz = gyro.GZ();
  ax = acc.AX();
  ay = acc.AY();
  az = acc.AZ();

  // Read pressure & temps
  b = bar.GetPressure();
  tboard = tempboard.GetTemperature();

  // Read DS18B20 on PA5
  ds18b20.requestTemperatures();
  tds18b20 = ds18b20.getTempCByIndex(0);

  if (millis() - lastPrint > 3000) {
    String datalog = "";
    datalog += "Pressure=" + String(b);
    datalog += " TempIn=" + String(tboard);      // LM75A
    datalog += " TempOut=" + String(tds18b20);   // DS18B20
    datalog += " GX=" + String(gx);
    datalog += " GY=" + String(gy);
    datalog += " GZ=" + String(gz);
    datalog += " AX=" + String(ax);
    datalog += " AY=" + String(ay);
    datalog += " AZ=" + String(az);
    datalog += " MX=" + String(mx);
    datalog += " MY=" + String(my);
    datalog += " MZ=" + String(mz);

    if (dataFile) {
      datalog += " SD=1";
      dataFile.println(datalog);
      dataFile.flush();
    } else {
      datalog += " SD=0";
    }

    Serial1.println(datalog); // LoRa
    Serial2.println(datalog); // USB

    lastPrint = millis();
  }
}
