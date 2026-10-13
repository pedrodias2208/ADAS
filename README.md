# ADAS
ADAS Simulator (Advanced Driver Assistance Systems) 🚗💻

This is an academic project developed in C that simulates the operation of advanced driver-assistance systems. The program processes telemetry data and simulated sensors to make real-time safety decisions, such as collision warnings and lane-keeping assistance.

📋 Features

The system features an interactive menu and performs the following mathematical and logical operations:

Sensor Fusion: Receives data from three front sensors (Radar, Lidar, and Camera) and calculates the median to discard anomalous readings, defining the validated distance to the obstacle ahead.

Safe Distance Calculation: Calculates the required space for a full stop considering the current vehicle speed, road friction coefficient, and reaction time based on the driving profile (Sport, Normal, or Safe).

Autonomous Emergency Braking (AEB): Compares the validated distance from the sensors with the calculated safe distance. It classifies the situation as "Safe", "Warning", or triggers the "Collision Risk" (AEB).

Lane Keeping Assist: Monitors lateral sensors and warns about the danger of drifting out of the lane. The safety margin is calculated dynamically, increasing as the vehicle speed exceeds 80 km/h.

Data Generation: Allows manual data entry for specific testing or generating batches of 50 random samples to simulate a continuous data stream.

🛠️ Technologies Used

Language: C

Standard Libraries: stdio.h, stdlib.h, time.h

🚀 How to Compile and Run

To run this project on your machine, you will need a C compiler (such as GCC) installed.

Clone the repository (or download the projeto1.c file):

git clone https://github.com/YOUR_USERNAME/REPOSITORY_NAME.git


Navigate to the project directory and compile the code:

gcc projeto1.c -o adas_simulator


Run the program:

Linux/macOS:

./adas_simulator


Windows:

adas_simulator.exe


🎮 How to Use

When launching the program, you must configure the initial road conditions:

Road Friction (Atrito da via): Enter a decimal value (e.g., 0.8 for dry asphalt, 0.4 for wet roads).

Sensitivity (Sensibilidade): Choose a profile (1-Sport, 2-Normal, 3-Safe) that will affect the simulated reaction time.

Then, the Main Menu will be displayed:

Option 1: Generates 50 random samples of speed and sensor readings.

Option 2: Allows manual input for a specific scenario (exact speeds and distances).

Option 3: Processes the data matrices applying the braking and lane-keeping logic.

Option 4: Exits the application.

📚 Code Structure

The code is organized modularly using functions that receive and manipulate multidimensional arrays:

carregar_iniciais: Populates the system with random data.

fusao_sensores: Redundancy and fault tolerance logic.

distancia_segura_freio: Physics and kinematics calculations.

analise_risco_frontal: Decision logic for the AEB system.

assistente_faixa: Lateral decision logic and dynamic margins.

Project developed for academic purposes.
