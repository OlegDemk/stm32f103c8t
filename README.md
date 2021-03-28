# stm32f103c8t
Hardwere:

Scheme and main algorithm all project is in 'Hardware diagram'.
'Hardware diagram' file can be opened in https://app.diagrams.net/

  MODULES:                                        Bus         Functions
1. STM32F103C8T.                                             
2. GSM module (SIM800L).                           UART        Make/receive calls (no sms)
3. Microphone for GSM module.                                  For Call            
4. Phone speaker GSM module.                                   For Call
5. UART/USB module for GSM module.                             For debug
6. GPS module (GY-GPS6MV2).                        UART        Only receive data
7. GPS antenna.                                                USE
8. Finger print module (GT511C3).                  UART        Saving fingers/identify/delete fingers 
9. OLED module  (SSD1306).                         I2C         Print all information
10. Flash memory module W25Q128.                    SPI        Log data from SI7021 
11. Flas memory Micro SD module.                    SPI        Doesn't used
12. Sensor Temperature and humidity (SI7021).       I2C        Temperature and humidity     
13. Secsor Temperature and humidity (AM2302).      1-wire bus  Doesn't used
14. Motion detector sensor.                        PIN         Show on OLED  
15. Motion sensor (APDS-9960).                     I2C         Doesn't used
16. Step down DC/DC module.                                    Main power
17. Serwo motor (SC90).                            PWM         Door lock imitation. Using with fingerprint sensor
18. Buzzer.                                        PWM         Doesn't used
19. Green LED.                                     PWM         PWM
20. Keyboard.                                                  Print number/action/navigate menu
21. Power on/off switch.                               
22. UART/USB module finger print module.                       For bebug 
23. GSM antenna.       
24. Vibro engine.                                              For incomming call
25. Batteries (2X18650).                                       


![alt text](https://github.com/OlegDemk/stm32f103c8t/blob/master/1.png)


![alt text](https://github.com/OlegDemk/stm32f103c8t/blob/master/2.png)

Software:
For create project has been used STM32IDE.
![alt text](https://github.com/OlegDemk/stm32f103c8t/blob/master/screenshot_4.png)

Project was stopped, because STM32f103 doesn't have enough FLASH memory for growing project.

