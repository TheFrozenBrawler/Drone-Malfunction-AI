# Drone-Malfunction-AI
This project was created by a group of three students from Poznań University of Technology, Poland, as preparatory work for their final engineering bachelor thesis. It was developed to detect malfunctions in a drone's propeller using AI tools.

# Hardware Components
![connection_schematic](https://github.com/user-attachments/assets/25afd8dc-e7a5-498d-88f8-de4a50ade5d8)

# Software Components
- **Drone_Malfunction_AI_RTOS** - main program to control the BLDC motor, read data, and send it via UART. It is written in C for the STM32 board and uses CMSIS and RTOS libraries - my part.
- **python_data_receiver** - reads data transferred via UART - my part.
- **AI_Python** - post-processing data and comparing it to a known data set - created by my coworker.

# Description
The main goal of the project was to detect if the propeller is damaged based on readings from three AC current sensors and one vibration sensor. My part of the project was hardware connection and software development to acquire data. To achieve this, I used the Nucleo H723ZG board due to its easy access to ADC converters, I2C interface, UART, and PWM timer.

The **Drone_Malfunction_AI_RTOS** program cycle:
1. Motor starts with PWM control.
2. At the same time, ADC converters start reading data.
3. Data is read for a predefined time.
4. Motor and converter stops
5. Data is transferred via UART and received by the Python program.

The Nucleo project **Drone_Malfunction_AI_RTOS** consists of the following main components:
- **globals.h** - configuration header file.
- **motor_handler.c / .h** - motor control component.
- **measure_handler.c / .h** - data reading component using ADC.
- **mpu6050.c / .h** - external library used for vibration sensor support.
- **FreeRTOS.c** - component using CMSIS FreeRTOS v2.
- **control_task** - thread controlling the task execution order.
- **measure_task** - thread controlling data readings.
- **uart_task** - thread used for sending data.

# Examples of results
| Image of propeller | Plot of AC | Plot of vibration |
|--|--|--|
| <img src="https://github.com/user-attachments/assets/d5396145-5114-4f1d-a835-57de9c12f3dd" alt="fine_propeller" width="500"/> | <img src="https://github.com/user-attachments/assets/9b50027a-530d-49c5-ac79-a4c140c2dae4" alt="fine_propeller_AC" width="500"/> |<img src="https://github.com/user-attachments/assets/70874769-ae3c-429f-bbd7-8117309c5097" alt="fine_propeller_vibr" width="500"/>|
| <img src="https://github.com/user-attachments/assets/32189617-a860-4379-9978-1f9cbcd76b95" alt="half_wing_broke" width="500"/> | <img src="https://github.com/user-attachments/assets/b3f53c6b-f342-4379-8eba-03ebaee5b1ab" alt="half_wing_broke_AC" width="500"/> |<img src="https://github.com/user-attachments/assets/9ba93aad-0f67-4d59-b219-7579a65be488" alt="half_wing_broke_vibr" width="500"/>|
| <img src="https://github.com/user-attachments/assets/be9c1526-1df4-48a0-a8b7-2e9cdddaae8c" alt="one_wing" width="500"/> | <img src="https://github.com/user-attachments/assets/04ff28d8-6317-44dc-bbd5-d3244b54f45a" alt="one_wing_AC" width="500"/> |<img src="https://github.com/user-attachments/assets/4f04c4ee-680e-4efc-88e7-928d438b20b7" alt="one_wing_vibr" width="500"/>|
