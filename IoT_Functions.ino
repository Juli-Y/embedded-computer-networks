/**
  IoT_Functions.ino

  This file contrains the MQTT functions and application functions
  for the IoT section of the program. These functions controll all aspects
  to do with sending and receiving data from the MQTT broker.

  Author: Julian Yuste
  Date:   11/03/2023
*/


// MQTT FUNCTIONS
// Reconnect to adafruit io MQTT server
void reconnect()
{
  // loop until we're reconnected
  while (!client.connected())
  {
    Serial.println("attempting mqtt connection...");

    // Try to connect to adafruit.io
    if (client.connect(CLIENT_ID, USER_ID, API_KEY))
    {
      Serial.println("... connected");

      // Once connected, subscribe to all feeds.
      // "throttle" and "errors" topics will let us know if we are
      // bumping up against the adafruit.io rate limit.
      client.subscribe((USER_ID "/feeds/status-messages"));
      client.subscribe((USER_ID "/feeds/outdoor-light-level"));
      client.subscribe((USER_ID "/feeds/room-temperature"));
      client.subscribe((USER_ID "/feeds/room-humidity"));
      client.subscribe((USER_ID "/feeds/light-monitor"));
      client.subscribe((USER_ID "/feeds/heating-monitor"));
      client.subscribe((USER_ID "/feeds/heater-system"));
      client.subscribe((USER_ID "/feeds/alarm-system"));
      client.subscribe((USER_ID "/feeds/temp-plus"));
      client.subscribe((USER_ID "/feeds/temp-minus"));
      client.subscribe((USER_ID "/throttle"));
      client.subscribe((USER_ID "/errors"));

      // Publish an announcement on the MQTT dashboard
      client.publish((USER_ID "/feeds/status-messages"), "CONNECTED!");
    }
    else
    {
      // Print some error status
      Serial.print("connection failed, rc = ");
      Serial.print(client.state());
      Serial.println();
      Serial.println("We will try again in 5 seconds");

      // wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// MQTT message received callback (triggered every time we get a
// message)
void callback(char* topic, byte* payload, unsigned int length)
{
  // Get the topic
  String t = String(topic);

  // Get the value of the message
  char data[length + 1];
  for (int i = 0; i < length; i++)
  {
    data[i] = payload[i];
  }
  data[length] = '\0';

  // Print the message to the serial window
  Serial.print("message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(data);

  // Parse the lighting monitoring topic
  if (t.indexOf("light-monitor") > 0)
  {
    // If the light smart monitoring has been activated
    if (strcmp(data, "ON") == 0)
    {
      // Set the state to HIGH so that the PIR sensor is read
      // Publish an announcement on the MQTT dashboard
      lighting_pir_led_state = HIGH;
      lighting_pir_led_new_state = lighting_pir_led_state;
      client.publish((USER_ID "/feeds/status-messages"), "Smart Monitoring Activated (Lighting)");
    }
    else if (strcmp(data, "OFF") == 0)
    {
      // If the light smart monitoring has been disabled
      // Set the state to LOW so that the PIR sensor and code is stopped
      // Publish an announcement on the MQTT dashboard
      lighting_pir_led_state = LOW;
      lighting_pir_led_new_state = lighting_pir_led_state;
      client.publish((USER_ID "/feeds/status-messages"), "Smart Monitoring Disabled (Lighting)");
    }
  }

  // Parse the heating monitoring topic
  if (t.indexOf("heating-monitor") > 0)
  {
    // If the heating smart monitoring has been activated
    if (strcmp(data, "ON") == 0)
    {
      // Set the state to HIGH so that the PIR sensor is read
      // Publish an announcement on the MQTT dashboard
      heating_pir_led_state = HIGH;
      heating_pir_led_new_state = heating_pir_led_state;
      client.publish((USER_ID "/feeds/status-messages"), "Smart Monitoring Activated (Heating)");
    }
    else if (strcmp(data, "OFF") == 0)
    {
      // If the heating smart monitoring has been disabled
      // Set the state to LOW so that the PIR sensor and code is stopped
      // Publish an announcement on the MQTT dashboard
      heating_pir_led_state = LOW;
      heating_pir_led_new_state = heating_pir_led_state;
      client.publish((USER_ID "/feeds/status-messages"), "Smart Monitoring Disabled (Heating)");
    }
  }

  // Parse the heating system topic
  if (t.indexOf("heater-system") > 0)
  {
    // If the heater system has been turned ON
    if (strcmp(data, "ON") == 0)
    {
      // Set the state to HIGH so that the heater system is turned ON
      // Publish an announcement on the MQTT dashboard
      heater_led_state = HIGH;
      heater_led_new_state = heater_led_state;
      client.publish((USER_ID "/feeds/status-messages"), "Heater System Activated");
    }
    else if (strcmp(data, "OFF") == 0)
    {
      // If the heater system has been turned OFF
      // Set the state to LOW so that the heater system is turned OFF
      // Publish an announcement on the MQTT dashboard
      heater_led_state = LOW;
      heater_led_new_state = heater_led_state;
      client.publish((USER_ID "/feeds/status-messages"), "Heater System Disabled");
    }
  }

  // Parse the alarm system topic
  if (t.indexOf("alarm-system") > 0)
  {
    // If the alarm system has been turned ON
    if (strcmp(data, "ON") == 0)
    {
      // Set the state to HIGH so that the alarm system is turned ON
      // Publish an announcement on the MQTT dashboard
      alarm_led_state = HIGH;
      alarm_led_new_state = alarm_led_state;
      client.publish((USER_ID "/feeds/status-messages"), "Alarm System Activated");
    }
    else if (strcmp(data, "OFF") == 0)
    {
      // If the alarm system has been turned OFF
      // Set the state to LOW so that the alarm system is turned OFF
      // Publish an announcement on the MQTT dashboard
      alarm_led_state = LOW;
      alarm_led_new_state = alarm_led_state;
      client.publish((USER_ID "/feeds/status-messages"), "Alarm System Disabled");
    }
  }

  // Parse the temp plus topic
  if (t.indexOf("temp-plus") > 0)
  {
    // If the temp plus (+) button is pressed
    if (strcmp(data, "1") == 0)
    {
      // Increase the temperature setpoint by 2
      // Convert the value into a string and
      // Publish an announcement on the MQTT dashboard
      set_temp = set_temp + 2;
      char set_temp_info[40];
      sprintf(set_temp_info, "Temperature is Set to %lu C", set_temp);
      client.publish((USER_ID "/feeds/status-messages"), set_temp_info);
    }
  }

  // Parse the temp minus topic
  if (t.indexOf("temp-minus") > 0)
  {
    // If the temp minus (-) button is pressed
    if (strcmp(data, "1") == 0)
    {
      // Decrease the temperature setpoint by 2
      // Convert the value into a string and
      // Publish an announcement on the MQTT dashboard
      set_temp = set_temp - 2;
      char set_temp_info[40];
      sprintf(set_temp_info, "Temperature is Set to %lu C", set_temp);
      client.publish((USER_ID "/feeds/status-messages"), set_temp_info);
    }
  }
}


// APPLICATION FUNCTIONS
// Sends the outdoor light level to the MQTT dashboard
void send_lighting()
{
  // If the client is already connected then send data
  if (client.connected())
  {
    // Reads the light sensor and maps it to be 0 to 5
    // Also maps from 0 to 100% to send to MQTT
    int ldr_value = analogRead(ldr_pin);
    int light_percent = map(ldr_value, 140, 980, 0, 100);

    // Convert the light level % to a string of characters for outdoor lighting
    // Publish an announcement on the MQTT dashboard
    char outdoor_light_level[5];
    sprintf(outdoor_light_level, "%i", light_percent);
    client.publish((USER_ID "/feeds/outdoor-light-level"), outdoor_light_level);
  }
}

// Sends the temperature & humidity readings to the MQTT dashboard
void send_temp_humid()
{
  // If the client is already connected then send data
  if (client.connected())
  {
    // Initialise structures to contain the sensor reading
    sensors_event_t humidity;
    sensors_event_t temp;
    sht40.getEvent(&humidity, &temp);
    int temp_val = temp.temperature;
    int humid_val = humidity.relative_humidity;

    // Convert the room temperature to a string of characters
    // Publish an announcement on the MQTT dashboard
    char temperature_info[2];
    sprintf(temperature_info, "%i", temp_val);
    client.publish((USER_ID "/feeds/room-temperature"), temperature_info);

    // Convert the room humidity to a string of characters
    // Publish an announcement on the MQTT dashboard
    char humidity_info[2];
    sprintf(humidity_info, "%i", humid_val);
    client.publish((USER_ID "/feeds/room-humidity"), humidity_info);
  }
}
