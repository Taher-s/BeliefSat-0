# BeliefSat-0

BeliefSat-0 is developed by students of K.J. Somaiya Institute of Technology to widen the reach of Amateur Radio-operators around the globe. The payload is a part of tribute to 100 years of Amateur Radio in India and meant to be in service to the Amateur radio community worldwide. The payload will perform UHF to VHF FM Voice Repeating and APRS Digipeating. It will have the following amateur radio payloads onboard:

1. Amateur Band UHF to VHF FM voice repeater with 2.5 KHz max bandwidth and 12.5 KHz channel spacing. 67.0 Hz CTCSS tone for uplink. 
1. VHF APRS Digipeater with 71.9 Hz CTCSS tone on the uplink. 

The ongoing project is in an advanced stage of development by the dedicated student team of our Institute's Student Satellite Program “New Leap Initiative”, led by the expertise of Principal Investigator Dr. Umesh Shinde. 

This project aims to develop and demonstrate the technology utilizing nano-satellite technology to make space tech more accessible to educational and low budget commercial missions, by utilizing inexpensive, off the shelf technologies and easily available materials. 

Working of BeliefSat-0:

1. The satellite will be launched through PSLV, this opportunity was presented to us by IN-SPACe (ISRO) as part of their POEM (PSLV Orbital Experimental Module)
1. The satellite will be positioned in a 650km orbit with a 6-degree inclination.
1. Its major function is voice repeating and APRS digipeating for the HAM radio community as our project main aim is to widen the reach of the HAM radio operators.
1. In the final stage of the PSLV, the satellite will have a continuous power supply which ensures the satellite's functionality.

### What is it made of? What does it look like? What are its dimensions and mass?

Structure: 

The external structure is constructed using Aluminum 8011.


![image](https://github.com/NewLeapKjsieit/BeliefSat-0/assets/111870945/7a989546-70be-4412-b3b2-f4791447a699)


`             `]


Mechanical Interface:


|**ID**|**Parameter**|**Unit**|**Specification**|
| :-: | :-: | :-: | :-: |
|Mechanical||||
|1\.|Maximum Estimate Mass of the payload|g|500  |
|2\.|Payload Volume(Excluding Antenna)|mm <sup>3</sup>|` `150x118x34 |
|3\. |Payload Volume (Including Antenna)|mm <sup>3</sup>|` `150x118x512|
|4\.|Antenna Length (Nagoya NL-770S VHF/UHF High Gain Mobile Antenna)|mm|480|
|5\.|Payload mechanical interface||6x M5 HEX steel bolt|

# How is it Powered?

**Electrical interface:**

The payload will be supplied power from the launch vehicle to which it will be connected by a DB9 connector.

The interface connector details are provided in the table given below:

|**CONNECTOR TYPE**|D-sub 9 Pin Connector|
| :- | :- |
|**On payload**|RECEPTACLE-TYPE (female) (Refer **Appendix B** for pictorial information)|
|**On Launch Vehicle**|PIN-TYPE (male)|
|**Location**|On the side plate, which is situated in the XZ plane (along negative Y axis)|

Only the VCC and the ground pin of the DB9 connector will be connected to the vehicle.

Power requirements:

Our payload requires a nominal power supply of 2W and peak power supply of upto 9W

# How does it communicate?

Transciever Specs 

VHF

- Name : [DRA818V](http://www.dorji.com/products.php?Keyword=dra818v)
- Frequency Range : 134~174 Mhz
- Channel Space : 12.5/25 KHz
- TX Current : 450/750 mA
- Temperature : -20°C ~ +70°C
- Output Power : +27/30 dbm
- Reciever Sensitivity : -122 dbm (As per datasheet, observed sensititvity yet to be calculated)
  UHF
- Name : [DRA818U](http://www.dorji.com/products.php?Keyword=dra818u)
- Frequency Range : 400~470 Mhz
- Channel Space : 25 KHz
- TX Current : 450/750 mA
- Temperature : -20°C ~ +70°C
- Output Power : +27/30 dbm
- Reciever Sensitivity : -122 dbm (As per datasheet, observed sensititvity yet to be calculated) (UHF will only be used for voice repeater uplink)

` `Antenna:

![image](https://github.com/NewLeapKjsieit/BeliefSat-0/blob/main/Nagoya-NL-770-S-Car-Antenna-500x500.jpg)
Nagoya NL-770S VHF/UHF High Gain Mobile Antenna:

This antenna boasts a unique feature where the satellite's outer cover serves as the grounding mechanism. The carefully engineered radiation pattern, along with impressive gain parameters, ensures strong signal reception and transmission.

What sets the NL-770S apart is its robust construction, making it a reliable choice for various conditions. Its dual-band capabilities cover both VHF and UHF frequencies, enhancing its versatility. The antenna's strategic placement on the satellite, connected via a UHF connector at the top, optimizes signal performance.

`  `Link Budget:

- UHF Uplink Budget: 

i. Calculation of FSPL() = 20x log(D) + 20x log(F) + 20x log (4x pi/c) - G<sub>TX</sub>  - G<sub>RX</sub>               

`                          `= 149.9724 dBm

ii. Calculation of Power Received (P<sub>RX</sub>) =P<sub>TX</sub>+G<sub>TX</sub>-L<sub>TX</sub>-L<sub>FSPL</sub> -L<sub>RX</sub> +G<sub>RX</sub> -</sub> L<sub>M</sub>

`                                                    `=46.99 + 0 - 0 - 149.9724 - 0 + 0 - 0

`                                                    `= -102.9824 dBm

iii. Link Margin = P<sub>RX</sub> - RS (Receive Sensitivity)

`                         `= (-102.9824 ) - (-122 )

`                         `= 19.0176 dBm

- VHF Uplink Budget: 

i. Calculation of FSPL = 20x log(D) + 20x log(F) + 20x log (4x pi/c) - G<sub>TX</sub>  - G<sub>RX</sub>               

`                          `= 140.4300 dBm

ii. Calculation of Power Received (P<sub>RX</sub>) =P<sub>TX</sub>+G<sub>TX</sub>-L<sub>TX</sub>-L<sub>FSPL</sub> -L<sub>RX</sub> +G<sub>RX</sub> -</sub> L<sub>M</sub>

`                                                    `=46.99 + 0 - 0 - 140.4300 - 0 + 0 - 0

`                                                    `= -93.44 dBm

iii. Link Margin = P<sub>RX</sub> - RS (Receive Sensitivity)

`                         `= (-93.44 ) - (-122 )

`                         `= 28.56 dBm


- ` `VHF Downlink Budget: 

i. Calculation of FSPL = 20x log(D) + 20x log(F) + 20x log (4x pi/c) - G<sub>TX</sub>  - G<sub>RX</sub>               

`                          `= 140.4300 dBm

ii. Calculation of Power Received (P<sub>RX</sub>) = P<sub>TX</sub>+G<sub>TX</sub>-L<sub>TX</sub>-L<sub>FSPL</sub> -L<sub>RX</sub> +G<sub>RX</sub> -</sub> L<sub>M</sub>

`                                                    `=30 + 0 - 0 - 140.4300 - 0 + 0 - 0

`                                                    `= -110.43 dBm

iii. Link Margin = P<sub>RX</sub> - RS (Receive Sensitivity)

`                         `= (-110.43) - (-121.88 )

`                         `= 11.45 dBm

Operational Modes:

The operational modes of the payload are:

1. Safe mode (initial mode) : Telemetry once every 30 seconds
1. Digipeater only mode :Telemetry once every 60 seconds and digipeater functionality active
1. Full function mode : Telemetry once every 60 seconds, both digipeater and voice repeater active

In the following table, we have listed three different commands along with their descriptions:

1\. Command:!MODE\_CHANGER@<ModeNumber>

Description: This command is used to change the operating mode, and it accepts a ModeNumber parameter that can be set to 0, 1, or 2.

2\. Command:!VHF\_RX\_FREQ\_CHANGER #<FREQUENCY>

Description: This command is employed for altering the receiving (RX) frequency of a VHF (Very High Frequency) device. You need to specify the desired frequency using the FREQUENCY parameter.

3\.Command: !VHF\_TX\_FREQ\_CHANGER %<FREQUENCY>

Description: This command allows you to modify the transmitting (TX) frequency of a VHF (Very High Frequency) device. To do this, provide the new frequency as the FREQUENCY parameter.

What are the different packet types and packet formats?

Telemetry Packet

APRS TELEMETRY FORMAT : The on-air packet telemetry format is as follows:

T#sss,111,222,333,444,555,xxxxxxxx where,

- sss : LSB of total number of system resets
- 111 : MSB of total number of system resets
- 222 : LSB of total number of resets induced by watch dog
- 333 : MSB of total number of resets induced by watch dog
- 444 : Temperature sensor 1 reading
- 555 : Temperature sensor 2 reading
- xxxxxxxx : If the last two bits are 10 then digipeater is on and if they are 01 then voice repeater is on.

References for converting Sensor data into readable format

1. Thermistor:
1. DC/DC Convertor: https://drive.google.com/drive/folders/0AMwc4gtF42UDUk9PVA

Reference for transreceivers

1. DRA818V :-  [Datasheet](http://www.dorji.com/docs/data/DRA818V.pdf)
1. DRA818U :-  [Datasheet](http://www.dorji.com/docs/data/DRA818U.pdf)




