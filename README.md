# Apsara---Automatic-Perimeter-Security-and-contRol-Assembly
This is a security focused project programmed using AVR assembly in Microchip/Atmel Studio IDE and C++ in Arduino IDE

▪ A perimeter surveillance bot which follows the basic principles of IR line following and incorporates inside it various surveillance and IoT features such as a video camera, sound trigger and close range Lidar for physical detection. 

▪ It has two microcontrollers controlling all the components inside it, an AVR Atmega 328p (programmed through avr dude using icsp by AVR studio) and 3 Esp32 modules providing iot and camera services to the entire project.
 
▪AVR mcu (Atmega 328p) programmed entirely in assembly while utilizing and manipulating several different registers of the MCU like the TCCR1A and TCCR1B for PWM, Output compare registers (OCR1A), GPIO manipulation using load store techniques and creation of custom delay subroutines. (IoT features for Esp32 in ArduinoIDE using C++)

