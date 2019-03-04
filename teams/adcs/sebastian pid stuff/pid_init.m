clear all;


voltage_output_min = -6; % V
voltage_output_max = 6;

motor_terminal_resistance = 22; % ohm
motor_terminal_inductance = .00057; % Henry
motor_shaft_inertia = 6.9 * 10^-8; % kg * m^2
motor_torque_constant = .00356; % Nm/A
motor_dynamic_friction_torque = 3.42E-7; %Nm/min

Kp = 0.00564134958887913;
Ki = 0.0920866669876409;
Kd = 3.19758992498617e-05;