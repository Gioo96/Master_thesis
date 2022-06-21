function [Phi_hand, hand_variable] = FK_hand_Experiments(marker_variable)

% hand_Phi computes the position of the marker (placed in the HAND) expressed wrt Wrold RF

% Human arm parameters
run('human_arm_parameters');

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

% TRANSLATION: Shoulder_RF wrt M0
trans_BS_M0 = [2;2;2]; % TO BE EVALUATED
% ROTATION: Shoulder_RF wrt M0
Rx_M0 = align.Rz * align.Rx * Rx_sh; % Rotation along x
Ry_x = align.Ry * align.Rz * Ry_sh; % Rotation along y
RBS_y = align.Rx * align.Ry * Rz_sh; % Rotation along z
RBS_M0 = Rx_M0 * Ry_x * RBS_y;

%
% ELBOW
%
Rz_el = [cos(q(4)) -sin(q(4)) 0;
            sin(q(4)) cos(q(4)) 0;
            0 0 1];

% TRANSLATION: Shoulder_RF_final wrt Shoulder_RF
trans_FS_BS = [0; -arm.shoulder.length; 0];
% ROTATION+TRANSLATION: Shoulder_RF_final wrt M0
TFS_M0 = [RBS_M0 RBS_M0*trans_FS_BS+trans_BS_M0;0 0 0 1];
% ROTATION: Elbow_RF wrt Shoulder_RF_final
RBE_FS = Rz_el;

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

% TRANSLATION: Elbow_RF_final wrt Elbow_RF
trans_FE_BE = [0; -arm.forearm.length; 0];
% ROTATION+TRANSLATION: Elbow_RF_final wrt Shoulder_RF_final
TFE_FS = [RBE_FS RBE_FS*trans_FE_BE;0 0 0 1];
% ROTATION+TRANSLATION: Elbow_RF_final wrt M0
TFE_M0 = TFS_M0 * TFE_FS;
% ROTATION: Elbow_RF_final wrt Wrist_RF
RFE_x = align.Rz * align.Rx * Rx_wr; % F_elbow --> wrist.Rx
Rx_y = align.Ry * align.Rz * Ry_wr; % wrist.Rx --> wrist.Ry
Ry_BW = align.Rx * align.Ry * Rz_wr; % wrist.Ry --> B_wrist
RBW_FE = RFE_x * Rx_y * Ry_BW; % F_elbow --> B_wrist

% TRANSLATION: Marker wrt B_wrist
trans_M_BW = hand_variable;
% ROTATION+TRANSLATION: Marker wrt Elbow_RF_final
TM_FE = [RBW_FE RBW_FE*trans_M_BW;0 0 0 1];
% ROTATION+TRANSLATION: Marker to World
Phi_hand = TFE_M0 * TM_FE;

end