#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

OneWire oneWire(D02);
DallasTemperature sensors(&oneWire);
DHT dht(D01, DHT22);

void setup(void)
{
  //sensors.setResolution(9);
  sensors.begin();
  dht.begin();
  
 int cnt = 0;
  while(!HologramCloud.isConnected()) {
    Dash.snooze(1000);
    cnt++;
  }

  //sync clock with network time
  rtc_datetime_t dt;
  if(HologramCloud.getNetworkTime(dt)) {
    Clock.setDateTime(dt);
  }
}

void loop(void) 
{

  int cnt = 0;
  // wait for modem to get good connection
  while (!HologramCloud.isConnected() && cnt++<15) {
    Dash.snooze(500);
  }

  sensors.requestTemperatures();

  float temp = dht.readTemperature(true);
  float humidity = dht.readHumidity();

  HologramCloud.print(sensors.getTempFByIndex(0));
  HologramCloud.print(":");
  HologramCloud.print(Charger.batteryMillivolts());
  HologramCloud.print(":");
  HologramCloud.print(cnt);
  HologramCloud.print(":");
  HologramCloud.print(temp);
  HologramCloud.print(":");
  HologramCloud.print(humidity);
  HologramCloud.attachTag("T");
  HologramCloud.sendMessage();
  HologramCloud.clear();
 
  HologramCloud.powerDown();
  int hour = Clock.currentTime().substring(0, 2).toInt();
  if (hour>20 || hour<5)
    Dash.deepSleepMin(30);
  else
    Dash.deepSleepMin(15);
  HologramCloud.powerUp();
}

