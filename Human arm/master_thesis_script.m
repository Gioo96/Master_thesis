% CasADi
import casadi.*

% Human arm parameters
run('human_arm_param.m');

%% Set parameters

% Sampling time
sample_Time = 0.01;

% Simulation time
simulink.time = 4.5;

% Simulation IC [rad]
simulink.q0 = zeros(n, 1);

% NLDM IC
set_param('master_thesis_simulink/System/NLDM delay', 'InitialCondition', 'simulink.q0');

% Marker is placed in the : shoulder --> 1
%                           forearm  --> 2
%                           hand     --> 3
marker.links = [1;
                1;
                1;
                1;
                2;
                2;
                2;
                3;
                3];
marker.shoulder_variables = [arm.shoulder.radius+arm.markers.radius -3/4*arm.shoulder.length 0;
                   -arm.shoulder.radius-arm.markers.radius -3/4*arm.shoulder.length 0;
                   arm.shoulder.radius+arm.markers.radius -1/4*arm.shoulder.length 0;
                   -arm.shoulder.radius-arm.markers.radius -1/4*arm.shoulder.length 0]';
marker.forearm_variables = [arm.forearm.radius+arm.markers.radius -3/4*arm.forearm.length 0;
                   -arm.forearm.radius-arm.markers.radius -1/2*arm.forearm.length 0;
                   arm.forearm.radius+arm.markers.radius -1/4*arm.forearm.length 0]';
marker.hand_variables = [0 -3/4*arm.hand.dimensions(2) arm.hand.dimensions(3)/2+arm.markers.radius;
                   0 -1/4*arm.hand.dimensions(2) arm.hand.dimensions(3)/2+arm.markers.radius]';

% Number of markers
m = size(marker.links, 1);

% % Comment System
% set_param('master_thesis_simulink/System', 'commented', 'on');
% 
% % Noise (TO BE ESTIMATED)
% % Simulation of 10s
% simulink.time = 10;
% 
% % MARKER M1 POSITION
% 
% % Simulation
% output_est = sim("master_thesis_simulink.slx");
% % Sample mean
% [marker.m1.pos.sample_mean, marker.m1.pos.delay] = super_marker_mean(output_est.super_m1_pos);
% % Sample variance
% marker.m1.pos.sample_variance = super_marker_variance(output_est.super_m1_pos, marker.m1.pos.sample_mean, marker.m1.pos.delay);
% 
% % MARKER M1 VELOCITY
% 
% global meas;
% global pp;
% sub = rossubscriber("/qualisys/Super_marker_1/odom", @Callback, 'DataFormat', 'struct');
% % % Sample mean
% % [marker.m1.pos.sample_mean, marker.m1.pos.delay] = super_marker_mean(output_est);
% % % Sample variance
% % marker.m1.pos.sample_variance = super_marker_variance(output_est, marker.m1.pos.sample_mean, marker.m1.pos.delay);

noise.measurement_var = 0.00001 * ones(m*3, 1);
noise.measurement_seed = 1;
noise.process_var = 0.00001 * diag(ones(n, 1));
noise.process_seed = 2;
noise.input_var = 0.0000001 * ones(m*3, 1);
noise.input_seed = 3;

% Simulation time
simulink.time = 4.5;

% Comment Kalman Filter function
set_param('master_thesis_simulink/System/Kalman Filter', 'commented', 'on');

% Comment Extended Kalman Filter function
set_param('master_thesis_simulink/System/Extended Kalman Filter', 'commented', 'on');

% Comment Linearized Discrete model
set_param('master_thesis_simulink/System/Linearized Discrete Model', 'commented', 'on');

% Comment State-Observer
set_param('master_thesis_simulink/System/State-observer', 'commented', 'on');

% Joints' trajectory
% q1
%set_param('master_thesis_simulink/Human arm/RightShoulder_joint/jRightShoulder_rotx', 'TorqueActuationMode', 'NoTorque', 'MotionActuationMode', 'ComputedMotion');
set_param('master_thesis_simulink/System/Human arm/RightShoulder_joint/jRightShoulder_rotx', 'TorqueActuationMode', 'ComputedTorque', 'MotionActuationMode', 'InputMotion');

% q2
%set_param('master_thesis_simulink/Human arm/RightShoulder_joint/jRightShoulder_roty', 'TorqueActuationMode', 'NoTorque', 'MotionActuationMode', 'ComputedMotion');
set_param('master_thesis_simulink/System/Human arm/RightShoulder_joint/jRightShoulder_roty', 'TorqueActuationMode', 'ComputedTorque', 'MotionActuationMode', 'InputMotion');

% q3
%set_param('master_thesis_simulink/Human arm/RightShoulder_joint/jRightShoulder_rotz', 'TorqueActuationMode', 'NoTorque', 'MotionActuationMode', 'ComputedMotion');
set_param('master_thesis_simulink/System/Human arm/RightShoulder_joint/jRightShoulder_rotz', 'TorqueActuationMode', 'ComputedTorque', 'MotionActuationMode', 'InputMotion');

% q4
%set_param('master_thesis_simulink/Human arm/Elbow_joint/jRightElbow_rotx', 'TorqueActuationMode', 'NoTorque', 'MotionActuationMode', 'ComputedMotion');
set_param('master_thesis_simulink/System/Human arm/Elbow_joint/jRightElbow_rotx', 'TorqueActuationMode', 'ComputedTorque', 'MotionActuationMode', 'InputMotion');

%% FORWARD KINEMATICS

% Name the markers which are placed in the shoulder, forearm and hand
marker.m_shoulder_str = []; % Name of markers placed in the SHOULDER
marker.m_forearm_str = []; % Name of markers placed in the FOREARM
marker.m_hand_str = []; % Name of markers placed in the HAND
s = 1;
f = 1;
h = 1;
for i = 1 : m

    switch marker.links(i)

        case 1
            marker.m_shoulder_str = [marker.m_shoulder_str, 's' + string(s)];
            s = s + 1;
        
        case 2
            marker.m_forearm_str = [marker.m_forearm_str, 'f' + string(f)];
            f = f + 1;

        case 3
            marker.m_hand_str = [marker.m_hand_str, 'h' + string(h)];
            h = h + 1;

    end
end

% Forward kinematics initialization
model.Phi = SX.sym('phi', [m * 3, 1]);

% SHOULDER, FOREARM, HAND variables
model.arm_parameters.shou_vars = [];
model.arm_parameters.fore_vars = [];
model.arm_parameters.hand_vars = [];
% Joint variables
model.q = SX.sym('q', [n, 1]);

% Forward kinematics computation
% SHOULDER
for s = 1 : size(marker.m_shoulder_str, 2)
    
    [T, variable] = shoulder_Phi(char(marker.m_shoulder_str(s)), model.q);
    model.arm_parameters.shou_vars = [model.arm_parameters.shou_vars, variable];
    model.Phi((s-1)*3+1 : (s-1)*3+3) = T(1:3, 4);
end

% FOREARM
i = 1;
for f = size(marker.m_shoulder_str, 2)+1 : size(marker.m_shoulder_str, 2)+size(marker.m_forearm_str, 2)
    
    [T, variable] = forearm_Phi(char(marker.m_forearm_str(i)), model.q);
    model.arm_parameters.fore_vars = [model.arm_parameters.fore_vars, variable];
    model.Phi((f-1)*3+1 : (f-1)*3+3) = T(1:3, 4);
    i = i + 1;
end

% HAND
j = 1;
for h = size(marker.m_shoulder_str, 2)+size(marker.m_forearm_str, 2)+1 : size(marker.m_shoulder_str, 2)+size(marker.m_forearm_str, 2)+size(marker.m_hand_str, 2)

    [T, variable] = hand_Phi(char(marker.m_hand_str(j)), model.q);
    model.arm_parameters.hand_vars = [model.arm_parameters.hand_vars, variable];
    model.Phi((h-1)*3+1 : (h-1)*3+3) = T(1:3, 4);
    j = j + 1;
end

% Phi function
functions.f_Phi = Function('f_Phi', {model.q, model.arm_parameters.shou_vars, model.arm_parameters.fore_vars, model.arm_parameters.hand_vars}, {model.Phi});

%% JACOBIAN

% Jacobian computation
model.J = jacobian(model.Phi, model.q);

% Jacobian function
functions.f_J = Function('f_J', {model.q, model.arm_parameters.shou_vars, model.arm_parameters.fore_vars, model.arm_parameters.hand_vars}, {model.J});

%% JACOBIAN PSEUDOINVERSE

% PseudoInverse computation
model.Jpseudo = pinv(model.J);

% PseudoInverse function
functions.f_Jpseudo = Function('f_Jpseudo', {model.q, model.arm_parameters.shou_vars, model.arm_parameters.fore_vars, model.arm_parameters.hand_vars}, {model.Jpseudo});

%% Discretization Runge-Kutta
% qk+1 = qk + 1/6*Ts*(k1 + 2k2 + 2k3 + k4) ---> qk+1 = f(qk, uk)
 
% qdot = Jpseudo * u
rk.u = SX.sym('u', [m*3, 1]);
rk.qdot = model.Jpseudo * rk.u;

% Function used to compute k1, k2, k3, k4
functions.f_RungeKutta = Function('f_RungeKutta', {model.q, model.arm_parameters.shou_vars, model.arm_parameters.fore_vars, model.arm_parameters.hand_vars, rk.u}, {rk.qdot});

% k1, k2, k3, k4 computation
rk.k1 = functions.f_RungeKutta(model.q, model.arm_parameters.shou_vars, model.arm_parameters.fore_vars, model.arm_parameters.hand_vars, rk.u);
rk.k2 = functions.f_RungeKutta(model.q + sample_Time*rk.k1/2, model.arm_parameters.shou_vars, model.arm_parameters.fore_vars, model.arm_parameters.hand_vars, rk.u);
rk.k3 = functions.f_RungeKutta(model.q + sample_Time*rk.k2/2, model.arm_parameters.shou_vars, model.arm_parameters.fore_vars, model.arm_parameters.hand_vars, rk.u);
rk.k4 = functions.f_RungeKutta(model.q + sample_Time*rk.k3, model.arm_parameters.shou_vars, model.arm_parameters.fore_vars, model.arm_parameters.hand_vars, rk.u);

% Function f computation
model.f = model.q + 1/6*sample_Time*(rk.k1 + 2*rk.k2 + 2*rk.k3 + rk.k4);
functions.f_f = Function('f_f', {model.q, model.arm_parameters.shou_vars, model.arm_parameters.fore_vars, model.arm_parameters.hand_vars, rk.u}, {model.f});

%% Linearization around eq. point (eq. point : q_eq = 0, u_eq = 0)
% qk+1 = Fqk + Guk
% pk = Hqk + Juk

% F, G in terms of symbolic variables
model.linearized.F_symb = jacobian(model.f, model.q);
model.linearized.G_symb = jacobian(model.f, rk.u);

% Function F, G
functions.f_Fekf = Function('f_Fekf', {model.q, model.arm_parameters.shou_vars, model.arm_parameters.fore_vars, model.arm_parameters.hand_vars, rk.u}, {model.linearized.F_symb});
functions.f_Gekf = Function('f_Gekf', {model.q, model.arm_parameters.shou_vars, model.arm_parameters.fore_vars, model.arm_parameters.hand_vars, rk.u}, {model.linearized.G_symb});

% Equilibrium point
kf.equilibrium_q = zeros(n, 1);
kf.equilibrium_u = zeros(3*m, 1);

%% Generate the mex functions

opts = struct('main', true, 'mex', true);

% % Generate Jpseudo
% functions.f_Jpseudo.generate('f_Jpseudo_mex.c', opts);
% mex f_Jpseudo_mex.c;
% 
% % Generate f
% functions.f_f.generate('f_f_mex.c', opts);
% mex f_f_mex.c;
% 
% % Generate Phi
% functions.f_Phi.generate('f_Phi_mex.c', opts);
% mex f_Phi_mex.c;
% 
% % Generate F
% functions.f_Fekf.generate('f_Fekf_mex.c', opts);
% mex f_Fekf_mex.c;

% Generate G
% functions.f_Gekf.generate('f_Gekf_mex.c', opts);
% mex f_Gekf_mex.c;
% 
% % Generate H
% functions.f_J.generate('f_Hekf_mex.c', opts);
% mex f_Hekf_mex.c;

%% Functions declaration

function [Phi_shou, shoulder_variable] = shoulder_Phi(marker_variable, q)

% shoulder_Phi computes the position of the marker (placed in the SHOULDER) expressed wrt Wrold RF

% Human arm parameters
run('human_arm_param');

% CasADi
import casadi.*

% Position of marker placed in the SHOULDER wrt its base RF
shoulder_variable = SX.sym(marker_variable, [3, 1]);

%
% SHOULDER
%

Rx_sh = [cos(q(1)) -sin(q(1)) 0;
               sin(q(1)) cos(q(1)) 0;
               0 0 1];
Ry_sh = [cos(q(2)) -sin(q(2)) 0;
               sin(q(2)) cos(q(2)) 0;
               0 0 1];
Rz_sh = [cos(q(3)) -sin(q(3)) 0;
               sin(q(3)) cos(q(3)) 0;
               0 0 1];

% ROTATION: W --> B_shoulder
RW_x = align.Rz * align.Rx * Rx_sh; % W --> shoulder.Rx
Rx_y = align.Ry * align.Rz * Ry_sh; % shoulder.Rx --> shoulder.Ry
Ry_BS = align.Rx * align.Ry * Rz_sh; % shoulder.Ry --> B_shoulder
RW_BS = RW_x * Rx_y * Ry_BS; % W --> B_shoulder

% TRANSLATION: Marker wrt B_shoulder
trans_BS_M = shoulder_variable;
% ROTATION+TRANSLATION: Marker to World
Phi_shou = [RW_BS RW_BS * trans_BS_M;0 0 0 1];

end


function [Phi_fore, forearm_variable] = forearm_Phi(marker_variable, q)

% forearm_Phi computes the position of the marker (placed in the FOREARM) expressed wrt Wrold RF

% Human arm parameters
run('human_arm_param');

% CasADi
import casadi.*

% Position of marker placed in the FOREARM wrt its base RF
forearm_variable = SX.sym(marker_variable, [3, 1]);

%
% SHOULDER
%

Rx_sh = [cos(q(1)) -sin(q(1)) 0;
               sin(q(1)) cos(q(1)) 0;
               0 0 1];
Ry_sh = [cos(q(2)) -sin(q(2)) 0;
               sin(q(2)) cos(q(2)) 0;
               0 0 1];
Rz_sh = [cos(q(3)) -sin(q(3)) 0;
               sin(q(3)) cos(q(3)) 0;
               0 0 1];

% ROTATION: W --> B_shoulder
RW_x = align.Rz * align.Rx * Rx_sh; % W --> shoulder.Rx
Rx_y = align.Ry * align.Rz * Ry_sh; % shoulder.Rx --> shoulder.Ry
Ry_BS = align.Rx * align.Ry * Rz_sh; % shoulder.Ry --> B_shoulder
RW_BS = RW_x * Rx_y * Ry_BS; % W --> B_shoulder

%
% ELBOW
%
Rx_el = [cos(q(4)) -sin(q(4)) 0;
            sin(q(4)) cos(q(4)) 0;
            0 0 1];

% TRANSLATION: F_shoulder wrt B_shoulder
trans_BS_FS = [0; -arm.shoulder.length; 0];
% ROTATION+TRANSLATION: F_shoulder to World
TW_FS = [RW_BS RW_BS * trans_BS_FS;0 0 0 1];
% ROTATION: F_shoulder --> B_elbow
RFS_x = align.Rz * align.Rx * Rx_el; % F_shoulder --> elbow.Rx
Rx_z = (align.Rz * align.Rx)'; % elbow.Rx --> elbow.Rz
RFS_BE = RFS_x * Rx_z; % F_shoulder --> B_elbow

% TRANSLATION: Marker wrt B_elbow
trans_BE_M = forearm_variable;
% ROTATION+TRANSLATION: Marker to F_shoulder
TFS_M = [RFS_BE RFS_BE * trans_BE_M;0 0 0 1];
% ROTATION+TRANSLATION: Marker to World
Phi_fore = TW_FS * TFS_M;

end


function [Phi_hand, hand_variable] = hand_Phi(marker_variable, q)

% hand_Phi computes the position of the marker (placed in the HAND) expressed wrt Wrold RF

% Human arm parameters
run('human_arm_param');

% CasADi
import casadi.*

% Position of marker placed in the HAND wrt its base RF
hand_variable = SX.sym(marker_variable, [3, 1]);

%
% SHOULDER
%

Rx_sh = [cos(q(1)) -sin(q(1)) 0;
               sin(q(1)) cos(q(1)) 0;
               0 0 1];
Ry_sh = [cos(q(2)) -sin(q(2)) 0;
               sin(q(2)) cos(q(2)) 0;
               0 0 1];
Rz_sh = [cos(q(3)) -sin(q(3)) 0;
               sin(q(3)) cos(q(3)) 0;
               0 0 1];

% ROTATION: W --> B_shoulder
RW_x = align.Rz * align.Rx * Rx_sh; % W --> shoulder.Rx
Rx_y = align.Ry * align.Rz * Ry_sh; % shoulder.Rx --> shoulder.Ry
Ry_BS = align.Rx * align.Ry * Rz_sh; % shoulder.Ry --> B_shoulder
RW_BS = RW_x * Rx_y * Ry_BS; % W --> B_shoulder

%
% ELBOW
%
Rx_el = [cos(q(4)) -sin(q(4)) 0;
            sin(q(4)) cos(q(4)) 0;
            0 0 1];

% TRANSLATION: F_shoulder wrt B_shoulder
trans_BS_FS = [0; -arm.shoulder.length; 0];
% ROTATION+TRANSLATION: F_shoulder to World
TW_FS = [RW_BS RW_BS * trans_BS_FS;0 0 0 1];
% ROTATION: F_shoulder --> B_elbow
RFS_x = align.Rz * align.Rx * Rx_el; % F_shoulder --> elbow.Rx
Rx_z = (align.Rz * align.Rx)'; % elbow.Rx --> elbow.Rz
RFS_BE = RFS_x * Rx_z; % F_shoulder --> B_elbow

%
% WRIST
%
Rx_wr = [cos(q(5)) -sin(q(5)) 0;
            sin(q(5)) cos(q(5)) 0;
            0 0 1];
Ry_wr = [cos(q(6)) -sin(q(6)) 0;
            sin(q(6)) cos(q(6)) 0;
            0 0 1];
Rz_wr = [cos(q(7)) -sin(q(7)) 0;
            sin(q(7)) cos(q(7)) 0;
            0 0 1];

% TRANSLATION: F_elbow wrt B_elbow
trans_BE_FE = [0; -arm.forearm.length; 0];
% ROTATION+TRANSLATION: F_elbow to F_shoulder
TFS_FE = [RFS_BE RFS_BE * trans_BE_FE;0 0 0 1];
% ROTATION+TRANSLATION: F_elbow to W
TW_FE = TW_FS * TFS_FE;
% ROTATION: F_elbow --> B_wrist
RFE_x = align.Rz * align.Rx * Rx_wr; % F_elbow --> wrist.Rx
Rx_y = align.Ry * align.Rz * Ry_wr; % wrist.Rx --> wrist.Ry
Ry_BW = align.Rx * align.Ry * Rz_wr; % wrist.Ry --> B_wrist
RFE_BW = RFE_x * Rx_y * Ry_BW; % F_elbow --> B_wrist

% TRANSLATION: Marker wrt B_wrist
trans_BW_M = hand_variable;
% ROTATION+TRANSLATION: Marker to F_elbow
TFE_M = [RFE_BW RFE_BW * trans_BW_M;0 0 0 1];
% ROTATION+TRANSLATION: Marker to World
Phi_hand = TW_FE * TFE_M;

end
