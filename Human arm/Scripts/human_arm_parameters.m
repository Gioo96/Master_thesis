%% HUMAN ARM PARAMETERS

% Number of DoF
n = 7;

% CHANGE HERE NUMBER OF MARKERS
shoulder_num = 2;
forearm_num = 0;
hand_num = 2;
m = shoulder_num + forearm_num + hand_num;

% Shoulder
arm.shoulder.length = 0.25; % [m]
arm.shoulder.radius = 0.05; % [m]
arm.shoulder.mass = 2.4459; % [Kg]
arm.shoulder.moment_inertia = [0.0059341, 0.0010123, 0.0059341]; % [Kg*m^2]

% Forearm
arm.forearm.length = 0.25; % [m]
arm.forearm.radius = 0.03; % [m]
arm.forearm.mass = 1.578; % [Kg]
arm.forearm.moment_inertia = [0.0089856, 0.00041027, 0.0089856]; % [Kg*m^2]

% Hand
arm.hand.dimensions = [0.1, 0.18, 0.04]; % [m]
arm.hand.mass = 0.4734; % [Kg]
arm.hand.moment_inertia = [0.0015629, 0.0007401900000000001, 0.0021389]; % [Kg*m^2]

% Markers
arm.markers.radius = 0.01; % [m]
arm.markers.links = [1*ones(shoulder_num, 1);
                2*ones(forearm_num, 1);
                3*ones(hand_num, 1)];

% Name directory
directory = strcat('S', num2str(shoulder_num), '_F', num2str(forearm_num), '_H', num2str(hand_num));

%% Global variables

global q0_model;
global q0_LS;
global kf;
global ekf;
global f_J f_f f_Phi f_F;
global markers_shoulder markers_forearm markers_hand;

%% Align Reference frames

align.Rx = [1 0 0;
            0 0 -1;
            0 1 0];
align.Ry = [0 0 1;
            0 1 0;
            -1 0 0];
align.Rz = [0 -1 0;
            1 0 0;
            0 0 1];

%% Sample Time

sample_Time = 0.01;

%% Simulation time

simulink.time = 4.5;

%% Number of samples

number_samples = simulink.time/sample_Time;

%% Noise (ESTIMATED) --> ASSUMPTION: All markers have the same noise

global noise;
noise.R = 7.9099e-07 * diag(ones(m*3, 1)); % ESTIMATED
noise.R_seed = 1;
noise.Q = 1 * diag(ones(n, 1));
noise.Q_seed = 2;
%noise.Nu = 0.0285 * diag(ones(m*3, 1)); % ESTIMATED
noise.Nu = 0.0285 * diag(ones(m*3, 1));
noise.Nu_seed = 3;