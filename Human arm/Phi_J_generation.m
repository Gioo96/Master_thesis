% CasADi
import casadi.*

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
forward_kinematics.Phi = SX.sym('phi', [m * 3, 1]);

% SHOULDER, FOREARM, HAND variables
forward_kinematics.shou_vars = [];
forward_kinematics.fore_vars = [];
forward_kinematics.hand_vars = [];
% Joint variables
forward_kinematics.q = SX.sym('q', [n, 1]);

% Forward kinematics computation
% SHOULDER
for s = 1 : size(marker.m_shoulder_str, 2)
    
    [T, variable] = shoulder_Phi(char(marker.m_shoulder_str(s)), forward_kinematics.q);
    forward_kinematics.shou_vars = [forward_kinematics.shou_vars, variable];
    forward_kinematics.Phi((s-1)*3+1 : (s-1)*3+3) = T(1:3, 4);
end

% FOREARM
i = 1;
for f = size(marker.m_shoulder_str, 2)+1 : size(marker.m_shoulder_str, 2)+size(marker.m_forearm_str, 2)
    
    [T, variable] = forearm_Phi(char(marker.m_forearm_str(i)), forward_kinematics.q);
    forward_kinematics.fore_vars = [forward_kinematics.fore_vars, variable];
    forward_kinematics.Phi((f-1)*3+1 : (f-1)*3+3) = T(1:3, 4);
    i = i + 1;
end

% HAND
j = 1;
for h = size(marker.m_shoulder_str, 2)+size(marker.m_forearm_str, 2)+1 : size(marker.m_shoulder_str, 2)+size(marker.m_forearm_str, 2)+size(marker.m_hand_str, 2)

    [T, variable] = hand_Phi(char(marker.m_hand_str(j)), forward_kinematics.q);
    forward_kinematics.hand_vars = [forward_kinematics.hand_vars, variable];
    forward_kinematics.Phi((h-1)*3+1 : (h-1)*3+3) = T(1:3, 4);
    j = j + 1;
end

% Phi function
f_Phi = Function('f_Phi', {forward_kinematics.q, forward_kinematics.shou_vars, forward_kinematics.fore_vars, forward_kinematics.hand_vars}, {forward_kinematics.Phi});

%% JACOBIAN

% Jacobian computation
jacobian.J = jacobian(forward_kinematics.Phi, forward_kinematics.q);

% Jacobian function
f_J = Function('f_J', {forward_kinematics.q, forward_kinematics.shou_vars, forward_kinematics.fore_vars, forward_kinematics.hand_vars}, {jacobian.J});


%% Generate the mex functions

opts = struct('main', true, 'mex', true);
f_Phi.generate('f_Phi_mex.c', opts);
mex f_Phi_mex.c
f_J.generate('f_J_mex.c', opts);
mex f_J_mex.c

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