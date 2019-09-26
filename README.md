# MassFlowControlUnitPublic
Previously in our laboratory we used a very simple control unit for mass flow controllers. It basically consisted of potentiometers, power supply and digital voltmeters. The project goal was to construct and program more advanced control unit. The new one recalculates voltage values of the flow rate into the standard cubic centimeters, remembers the state after swithing off and shows both actual flow and setpoint values.

For powering the mass flow controllers 24 V a switched mode power supply was used. A step down DC-DC converter was used for powering buffer operational amplifiers (12 V) and further step down DC-DC was used for powering the Arduino Nano (5 V). All step-down DC-DC converters and power supply have overvoltage and overcurrent protections. The LCD display (16x4) with LED backlight was connected to Arduino using parallel connection

## Highlights
* Hardware soldering and programming

## Used frameworks and programming languages
Arduino C
