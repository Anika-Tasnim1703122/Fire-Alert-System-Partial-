#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#define flame D0
#define buzzer D2
#define led1 D5
#define led2 D8

#define FIREBASE_HOST "dht11andflamesensor-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "tVuULdk6xPwvJCyyKCuik9QVEANZfFkpxv8fH3f0"

#include "DHT.h"
#define DHTTYPE DHT11
#define sensor D3

DHT dht(sensor, DHTTYPE);



char ssid[] = "Anika";
char pass[] = "nameera5589";
String IP;

FirebaseData firebaseData;
FirebaseData ledData;
FirebaseJson json;

//FirebaseJson json;



void Connection()
{
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  IP = WiFi.macAddress();
}

void setup() {
  // put your setup code here, to run once:
  dht.begin();
  Serial.begin(9600);
  pinMode(flame, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  Connection();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  delay(700);


}

void loop() {
  // put your main code here, to run repeatedly:


  int fire = digitalRead(flame);
  int flag = 0;

  //Serial.println(fire);

  if (fire == 0)
  {
    flag = 1;
    digitalWrite(buzzer, HIGH);
    //digitalWrite(led1, HIGH);
    //digitalWrite(led2, HIGH);
    Serial.println("Alert!!! Fire detected");
    //SendEmail();
    if (Firebase.setBool(firebaseData, "/FirebaseIOT/FireStatus", 1))
    {
      Serial.println("PASSED");
    }
    else
    {
      Serial.println("FAILED");

    }
  }
  else
  {
    digitalWrite(buzzer, LOW);
    //digitalWrite(led1, LOW);
    //digitalWrite(led2, LOW);
    if (Firebase.setBool(firebaseData, "/FirebaseIOT/FireStatus", 0))
    {
      Serial.println("PASSED");
    }
    else
    {
      Serial.println("FAILED");

    }
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Current humidity = ");
  Serial.print(h);
  Serial.print("%  ");
  String fireH = String(h) + String("%");
  Serial.print("temperature = ");
  Serial.print(t);
  String tempH = String(t) + String("C");
  Serial.println("C  ");
  Serial.println(IP);

  //Firebase.setFloat("Humidity ",h);
  //Firebase.setFloat("Temperature ",t);

  /*if(Firebase.failed())
    {
    Serial.println("Firebase data setting failed");
    Serial.println(Firebase.error());
    delay(500);
    return;
    }*/
  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/temperature", t))
  {
    Serial.println("PASSED");
  }
  else
  {
    Serial.println("FAILED");

  }

  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/humidity", h))
  {
    Serial.println("PASSED");
  }
  else
  {
    Serial.println("FAILED");

  }
  if (Firebase.setString(firebaseData, "/FirebaseIOT/LocalIP",IP))
    {
      Serial.println("PASSED");
    }
    else
    {
      Serial.println("FAILED");

    }



  delay(1000);


}
