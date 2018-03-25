#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

OneWire oneWire(D02);
DallasTemperature sensors(&oneWire);
DHT dht(D01, DHT22);

void setup(void)
{
  sensors.begin();
  dht.begin();
  
 int cnt = 0;
  while(!HologramCloud.isConnected()) {
    Dash.snooze(1000);
    cnt++;
  }
  
  HologramCloud.print("Sensor On: ");
  HologramCloud.println(HologramCloud.getSignalStrength());
  HologramCloud.println(cnt);
  HologramCloud.sendMessage();
  
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
  while (!HologramCloud.isConnected() && cnt++<30) {
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

  rtc_datetime_t dt;
  if(HologramCloud.getNetworkTime(dt)) {
    Clock.setDateTime(dt);
  }

  if (Charger.batteryMillivolts()<3000) { 
    HologramCloud.print("battery low");
    HologramCloud.attachTag("M");
    HologramCloud.sendMessage();
    HologramCloud.clear();
    HologramCloud.powerDown();
    Dash.deepSleep();
  }
  
  HologramCloud.powerDown();
  int hour = Clock.currentTime().substring(0, 2).toInt();
  if (hour>20 || hour<5)
    Dash.deepSleepMin(30);
  else
    Dash.deepSleepMin(15);
  HologramCloud.powerUp();
}


