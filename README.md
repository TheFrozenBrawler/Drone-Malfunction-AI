# Drone-Malfunction-AI
This project was created by a group of three students of Poznań University of Technology, Poland as a preparation work for final engineering bachelor thesis. It was made to detect a malfunction of a drone propeller with AI tools.

# Hardware Components
![connection_schematic](https://github.com/user-attachments/assets/25afd8dc-e7a5-498d-88f8-de4a50ade5d8)

# Software Components
- Drone_Malfunction_AI_RTOS - main program to control BLDC motor, read data and send it via uart. It is written is C language for STM32 board and uses CMSIS and RTOS libraries - my part
- python_data_receiver - reading data transfered via UART - my part
- AI_Python - data postprocessig and comparsion to known set of data - created by my coworker

# Description
Main goal of the project was to detect if the propeller is damaged by readings of three AC current and one vibration sensors. My part of the project was hardware connection and software to acquire data. To do this I've used Nucleo H723ZG board due to the easy acces to ADC converters, I2C interface, UART and PWM timer.

Drone_Malfunction_AI_RTOS program cycle:
1. motor start with PWM reading
2. at the same moment ADC converters start reading data
3. data is reading for a predefined time
4. data is trasnfered by UART and received with python program

Nucleo project Drone_Malfunction_AI_RTOS consists of these main components:
- globals.h - configuration header file
- motor_handler.c / .h - motor control component
- measure_handler.c / .h - data reading component using ADC
- mpu6050.c / .h - external library used for vibration sensor support
- FreeRTOS.c - component using CMSIS FreeRTOS v2
- control_task - thread controlling the order of task execution
- measure_task - thread controlling data readings
- uart_task - thread used for sending data


# Examples of results
| Image of propeller | plot of AC | plot of vibration |
|--|--|--|
| <img src="https://github.com/user-attachments/assets/d5396145-5114-4f1d-a835-57de9c12f3dd" alt="fine_propeller" width="500"/> | <img src="https://github.com/user-attachments/assets/9b50027a-530d-49c5-ac79-a4c140c2dae4" alt="fine_propeller_AC" width="500"/> |<img src="https://github.com/user-attachments/assets/70874769-ae3c-429f-bbd7-8117309c5097" alt="fine_propeller_vibr" width="500"/>|
| <img src="https://github.com/user-attachments/assets/32189617-a860-4379-9978-1f9cbcd76b95" alt="half_wing_broke" width="500"/> | <img src="https://github.com/user-attachments/assets/b3f53c6b-f342-4379-8eba-03ebaee5b1ab" alt="half_wing_broke_AC" width="500"/> |<img src="https://github.com/user-attachments/assets/9ba93aad-0f67-4d59-b219-7579a65be488" alt="half_wing_broke_vibr" width="500"/>|
| <img src="https://github.com/user-attachments/assets/be9c1526-1df4-48a0-a8b7-2e9cdddaae8c" alt="one_wing" width="500"/> | <img src="https://github.com/user-attachments/assets/04ff28d8-6317-44dc-bbd5-d3244b54f45a" alt="one_wing_AC" width="500"/> |<img src="https://github.com/user-attachments/assets/4f04c4ee-680e-4efc-88e7-928d438b20b7" alt="one_wing_vibr" width="500"/>|
