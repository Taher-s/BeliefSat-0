/*
This firmware file is completed with credits to GitHUb user markqvist for their reposatory titled LibAPRS: https://github.com/markqvist/LibAPRS.git
It is recommended to look at their example code before trying to understand this firmware.

Date of updation: 04/03/2024
*/


#include <EEPROM.h>
#include "LibAPRS.h"
#include <SoftwareSerial.h>
#include <avr/wdt.h>

#define ADC_REFERENCE REF_3V3
#define OPEN_SQUELCH false
#define debug

#define SATCALL_SIGN "VU3CDI"   
#define GSCALL_SIGN "VU3OIR"
#define SATTELEM_SSID 11
#define SATDIGI_SSID 6
#define GS_SSID 1             


#define APRS_TOCALL "APUVU1"

#define PTT_VHF (3)           
#define UHF_POWER_DOWN_PIN (2)
#define UHF_RX (10)
#define UHF_TX (11)
#define VHF_RX (12)
#define VHF_TX (13)
#define THERM_1 (24)
#define THERM_2 (25)

//String VHF_SETUP_STRING = "AT+DMOSETGROUP=0,145.8250,145.8250,0000,0,0000\r\n";    //default values
#define UHF_SETUP_STRING "AT+DMOSETGROUP=0,437.0000,437.0000,0001,2,0001\r\n"        //default
#define TURN_OFF_VOICE_REPEATER digitalWrite(UHF_POWER_DOWN_PIN, LOW)   
#define TURN_ON_VOICE_REPEATER                                                 \
  do {                                                                         \
    digitalWrite(UHF_POWER_DOWN_PIN, HIGH);                                    \
    uhf_port.write(F(UHF_SETUP_STRING));                                       \                                 
  } while (0)

#define TOTAL_RESETS_EEPROM_ADDR 0x00
#define WD_INDUCED_RESETS_EEPROM_ADDR 0x02
#define TX_STATUS 0x04
#define Rx_Frequency 0x06 //need to check
#define Tx_Frequency 0x15 //need to check
char Rx_freq[8]="145.9800";
char Tx_freq[8]="145.9800";

#define COMMAND_INDICATOR '!'

SoftwareSerial vhf_port(VHF_RX, VHF_TX);
SoftwareSerial uhf_port(UHF_RX, UHF_TX);

int TELEMETRY_INTERVAL= 30000; // milliseconds
boolean gotPacket = false;
AX25Msg incomingPacket;
uint8_t *packetData;

boolean digipeater_is_on = false;
boolean voice_repeater_is_on = false;
boolean transmission_is_on;
uint16_t total_resets;
uint16_t wd_induced_resets ;
long last_telemetry_time;


void watchdogSetup(void) {
  cli();
  wdt_reset();
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDIE) | (1 << WDE) | (1 << WDP3) | (0 << WDP2) | (0 << WDP1) |
           (1 << WDP0);
  sei();
}

// This function is to copy the received and decoded packet into the controller's memory.
void aprs_msg_callback(struct AX25Msg *msg) {
  if (!gotPacket) {
    gotPacket = true;
    memcpy(&incomingPacket, msg, sizeof(AX25Msg));
    if (freeMemory() > msg->len) {
      packetData = (uint8_t *)malloc(msg->len);
      memcpy(packetData, msg->info, msg->len);
      incomingPacket.info = packetData;
    } else {
      gotPacket = false;
    }
  }
}


ISR(WDT_vect) {
  wd_induced_resets++;  // Increment the watchdog-induced resets variable
  EEPROM.write(WD_INDUCED_RESETS_EEPROM_ADDR, wd_induced_resets);   // Store the updated value in the permanent memory of the controller
}

void setup() {
#ifdef debug
  Serial.begin(9600);
#endif

  watchdogSetup();
  EEPROM.get(TOTAL_RESETS_EEPROM_ADDR, total_resets);

  // Increment the value of 'total_resets' and store it at memory address 0x00
  total_resets++;   
  EEPROM.put(TOTAL_RESETS_EEPROM_ADDR, total_resets);
  EEPROM.get(WD_INDUCED_RESETS_EEPROM_ADDR, wd_induced_resets);
  EEPROM.get(TX_STATUS, transmission_is_on);

  //pinMode(UHF_POWER_DOWN_PIN, OUTPUT);
  //digitalWrite(UHF_POWER_DOWN_PIN,LOW);        //turn DRA818U off

  vhf_port.begin(9600);
  uhf_port.begin(9600);

  EEPROM.get(Rx_Frequency,Rx_freq);
  EEPROM.get(Tx_Frequency,Tx_freq);
  
  DRA_V_config(Rx_freq,Tx_freq);

  wdt_reset();

  APRS_init(ADC_REFERENCE, OPEN_SQUELCH);
  APRS_setCallsign(SATCALL_SIGN, SATTELEM_SSID);
  APRS_setDestination(APRS_TOCALL, 0);

  APRS_setPreamble(350);
  APRS_setTail(100);

#ifdef debug
  APRS_printSettings();
  Serial.print(F("Free RAM:     "));
  Serial.println(freeMemory());
#endif

  last_telemetry_time = millis();
}

void loop() {
  processPacket();
  #ifdef debug
  Serial.println(wd_induced_resets);
  #endif
  
  if (millis() - last_telemetry_time >= TELEMETRY_INTERVAL && transmission_is_on){
    send_telemetry();
  }
  wdt_reset();
}


void processPacket() {
  if (gotPacket) {
    gotPacket = false;

    #ifdef debug
      Serial.print(F("Received APRS packet. SRC: "));
      Serial.print(incomingPacket.src.call);
      Serial.print(F("-"));
      Serial.print(incomingPacket.src.ssid);
      Serial.print(F(". DST: "));
      Serial.print(incomingPacket.dst.call);
      Serial.print(F("-"));
      Serial.print(incomingPacket.dst.ssid);
      Serial.print(F(". Data: "));

      for (int i = 0; i < incomingPacket.len; i++) {
        Serial.write(incomingPacket.info[i]);
      }
      Serial.println("");
    #endif

   if(recieve_command()){
    switch (incomingPacket.info[1]){              //@ for changing modes, # for changing RX freq, % for changing TX freq
      case '@':{
            switch(incomingPacket.info[2]){
                case '0': {
                  TURN_OFF_VOICE_REPEATER;
                  voice_repeater_is_on=false;
                  TELEMETRY_INTERVAL=30000;
                  break;
                }
                case '1':{
                  TURN_OFF_VOICE_REPEATER;
                  voice_repeater_is_on=false;
                  TELEMETRY_INTERVAL=60000;
                  break;
                }
                case '2':{
                  TURN_ON_VOICE_REPEATER;
                  TELEMETRY_INTERVAL=60000;
                  voice_repeater_is_on=true;
                  break;
                }
                default:{                            //default being go back to safe mode
                  TURN_OFF_VOICE_REPEATER;
                  voice_repeater_is_on=false;
                  TELEMETRY_INTERVAL=30000;
                  break;
                }
        }
        break;
      }
      
      case '#':{
        for(int i=0;i<8;i++){
          Rx_freq[i]=incomingPacket.info[(3+i)];
        }
        EEPROM.put(Rx_Frequency,Rx_freq);
        DRA_V_config(Rx_freq,Tx_freq);
        break;
      }
      
      case '%':{
        for(int i=0;i<8;i++){
          Tx_freq[i]=incomingPacket.info[(3+i)];
        }
        EEPROM.put(Tx_Frequency,Tx_freq);
        DRA_V_config(Rx_freq,Tx_freq);
        break;
      }
    }
    
   // If not a command: Check digipeater/transmission, set 'do_repeat' if needed.
   } else {
        boolean do_repeat = false;
        for (int i = 0; i < incomingPacket.rpt_count; i++) {
          if (!strcmp(incomingPacket.rpt_list[i].call, "ARISS") ||
              !strcmp(incomingPacket.rpt_list[i].call, SATCALL_SIGN)) {
            do_repeat = true;
          }
        }
        if (do_repeat) {
          APRS_setCallsign(incomingPacket.src.call, incomingPacket.src.ssid);
          APRS_setPath1(SATCALL_SIGN, (SATDIGI_SSID | 0x40));
          APRS_sendPkt(&incomingPacket.info[0], incomingPacket.len);
          APRS_setCallsign(SATCALL_SIGN, SATTELEM_SSID);  // Reset Callsign and Path1 values after sending the packet.
          APRS_setPath1("WIDE1", 1);
        }
      
      free(packetData); // Free the packetdata to make space in memory for next packet
    }
   
  }

}

bool recieve_command(){
  // This if statement verifies if the received packet is a command for the satellite.
  if( incomingPacket.src.call == SATCALL_SIGN &&
      incomingPacket.src.ssid == GS_SSID &&
      incomingPacket.info[0] == COMMAND_INDICATOR &&
      incomingPacket.dst.call == SATCALL_SIGN &&
      incomingPacket.dst.ssid == SATTELEM_SSID){
          return true;
  }
  else{
    return false;
  }
}


float get_Temperature(int thermistor_output){
  int thermistor_adc_val;
  float output_voltage, thermistor_resistance, therm_res_ln, temperature; 
  thermistor_adc_val = analogRead(thermistor_output);

  ADMUX = 0;
  output_voltage = ( (thermistor_adc_val * 3.3) / 1023.0 );
  thermistor_resistance = ( ( 3.3 * ( 10.0 / output_voltage ) ) - 10 ); /* Resistance in kilo ohms */
  thermistor_resistance = thermistor_resistance * 1000 ; /* Resistance in ohms   */
  therm_res_ln = log(thermistor_resistance);
  temperature = ( 1 / ( 0.001129148 + ( 0.000234125 * therm_res_ln ) + ( 0.0000000876741 * therm_res_ln * therm_res_ln * therm_res_ln ) ) ); /* Temperature in Kelvin */
  temperature = temperature - 273.15;
  #ifdef debug
    Serial.print("Temperature in degree Celsius = ");
    Serial.print(temperature);
    Serial.print("\t\t");
    Serial.print("Resistance in ohms = ");
    Serial.print(thermistor_resistance);
    Serial.print("\n\n");
  #endif
  return temperature ;
}

void send_telemetry(){
  //APRS_setMessageDestination("NOCALL", 0);          //for testing
  wdt_reset();
  float float_temp1=  get_Temperature(THERM_1);       
  float float_temp2=  get_Temperature(THERM_2);

  String temp1= String(float_temp1);
  String temp2= String(float_temp2);
  String watchdog_resets_no= String(wd_induced_resets);

  EEPROM.get(TOTAL_RESETS_EEPROM_ADDR, total_resets);
  uint8_t total_resets_lsb = (total_resets & 0x00FF);
  uint8_t total_resets_msb = ((total_resets & 0xFF00) >>8);
  String sss = String(total_resets_lsb);
  String resets_msb = String(total_resets_msb);
  EEPROM.get(WD_INDUCED_RESETS_EEPROM_ADDR, wd_induced_resets);  // Get total number of resets from memory
  uint8_t wd_induced_resets_lsb = (wd_induced_resets & 0x00FF);  // Separate 16-bit integer into two 8-bit integers (LSB and MSB)
  uint8_t wd_induced_resets_msb = ((wd_induced_resets & 0xFF00) >>8);
  String wd_lsb = String(wd_induced_resets_lsb);  // Convert total resets to strings
  String wd_msb = String(wd_induced_resets_msb);

  // Concat all the statuses and sensor values into one long string
  String telemetry = "t#" + sss + "," + resets_msb + "," + wd_lsb + "," + wd_msb + "," + temp1 + "," + temp2 + ","+ "xxxxxx" +  "BeliefSat-0";
  // Convert the string into a character array
  char message[telemetry.length()] = "";     
  telemetry.toCharArray(message, telemetry.length());
  
  APRS_sendMsg(message, strlen(message));   //Send telemetry as a mesage.
  
  last_telemetry_time = millis();
  
}


//Function for setting DRA818V frequency
void DRA_V_config(String Rx_freq, String Tx_freq) {
  vhf_port.begin(9600);
  // Set DRA818V frequency
  String cmd=("AT+DMOSETGROUP=0," + Rx_freq + "," + Tx_freq + ",0002,1,0002\r\n");
  //Serial.println(cmd);    //For debugging
  vhf_port.write(cmd.c_str());
  delay(100); // Give some time to receive the acknowledgment
  #ifdef debug
  while (vhf_port.available()) {
    char response = vhf_port.read();
    Serial.write(response); // Print the acknowledgment response on the Serial monitor
  #endif
}
}
