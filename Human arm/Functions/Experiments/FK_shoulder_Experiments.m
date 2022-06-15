function [Phi_shou, shoulder_variable] = FK_shoulder_Experiments(marker_variable)

% shoulder_Phi computes the position of the marker (placed in the SHOULDER) expressed wrt Wrold RF

% Human arm parameters
run('human_arm_parameters');

% CasADi
import casadi.*

% Position of marker placed in the SHOULDER wrt M0
shoulder_variable = SX.sym(marker_variable, [3, 1]);

%
% SHOULDER
%

% ROTATION+TRANSLATION: Marker to M0 (I don't care about rotation)
Phi_shou = [eye(3, 3) shoulder_variable;0 0 0 1];

end