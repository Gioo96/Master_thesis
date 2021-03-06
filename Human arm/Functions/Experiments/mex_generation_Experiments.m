function mex_generation_Experiments(links, sample_Time, tSh_0, zero_conf, generate)

% mex_generation generates the following mex functions: - Phi
%                                                       - Jpseudo
%                                                       - f
%                                                       - F_ekf
%                                                       - G_ekf
%                                                       - H_ekf

% Inputs:
% -- links          : Vector containing indices {1, 2, 3} related to the corresponding marker position
% -- sample_Time    : Sampling time
% -- tSh_0          : Position of Shoulder wrt Super_marker_0
% -- zero_conf      : true if tSh_0 is at 0 conf
% -- generate       : true if mex functions need to be generated

%  CasADi
import casadi.*

%% FORWARD KINEMATICS

% Number of joints
n = 7;
% Number of markers
m = size(links, 1);

% Name the markers which are placed in the shoulder, forearm and hand
m_shoulder_str = []; % Name of markers placed in the SHOULDER
m_forearm_str = []; % Name of markers placed in the FOREARM
m_hand_str = []; % Name of markers placed in the HAND

s = 1;
f = 1;
h = 1;
for i = 1 : m

    switch links(i)

        case 1
            m_shoulder_str = [m_shoulder_str, 's' + string(s)];
            s = s + 1;
        
        case 2
            m_forearm_str = [m_forearm_str, 'f' + string(f)];
            f = f + 1;

        case 3
            m_hand_str = [m_hand_str, 'h' + string(h)];
            h = h + 1;

    end
end

% Change directory
directory = strcat('C code/S', num2str(s-1), '_F', num2str(f-1), '_H', num2str(h-1), '/');
cd (directory);

% SHOULDER, FOREARM, HAND variables
shou_vars = [];
fore_vars = [];
hand_vars = [];

% Joint variables
q = SX.sym('q', [n, 1]);

% Forward kinematics initialization
Phi = SX.sym('phi', [m * 3, 1]);

% Forward kinematics computation
% SHOULDER
for s = 1 : size(m_shoulder_str, 2)
    
    [T, variable] = FK_shoulder_Experiments(char(m_shoulder_str(s)), q, tSh_0);
    shou_vars = [shou_vars, variable];
    Phi((s-1)*3+1 : (s-1)*3+3) = T(1:3, 4);
end

% FOREARM
i = 1;
for f = size(m_shoulder_str, 2)+1 : size(m_shoulder_str, 2)+size(m_forearm_str, 2)
    
    [T, variable] = FK_forearm_Experiments(char(m_forearm_str(i)), q, tSh_0);
    fore_vars = [fore_vars, variable];
    Phi((f-1)*3+1 : (f-1)*3+3) = T(1:3, 4);
    i = i + 1;
end

% HAND
j = 1;
for h = size(m_shoulder_str, 2)+size(m_forearm_str, 2)+1 : size(m_shoulder_str, 2)+size(m_forearm_str, 2)+size(m_hand_str, 2)

    [T, variable] = FK_hand_Experiments(char(m_hand_str(j)), q, tSh_0);
    hand_vars = [hand_vars, variable];
    Phi((h-1)*3+1 : (h-1)*3+3) = T(1:3, 4);
    j = j + 1;
end

% Phi function
f_Phi_exp = Function('f_Phi_exp', {q, shou_vars, fore_vars, hand_vars}, {Phi});

%% JACOBIAN

% Jacobian computation
J = jacobian(Phi, q);

% Jacobian function
f_J_exp = Function('f_J_exp', {q, shou_vars, fore_vars, hand_vars}, {J});

%% JACOBIAN PSEUDOINVERSE

% PseudoInverse computation
Jpseudo = (J'*J)^(-1) * J';

% Damped LS PseudoInverse computation
%Jpseudo_Damped = (J'*J + eye(n)*exp(-det(J'*J)))^(-1) * J';

% PseudoInverse function
f_Jpseudo_exp = Function('f_Jpseudo_exp', {q, shou_vars, fore_vars, hand_vars}, {Jpseudo});

% Damped LS PseudoInverse function
%f_Jpseudo_Damped = Function('f_Jpseudo_Damped', {q, shou_vars, fore_vars, hand_vars}, {Jpseudo_Damped});

%% Discretization Runge-Kutta
% qk+1 = qk + 1/6*Ts*(k1 + 2k2 + 2k3 + k4) ---> qk+1 = f(qk, u_noisyk)
 
% qdot = Jpseudo * u_noisy
u_noisy = SX.sym('u_noisy', [m*3, 1]);
qdot = Jpseudo * u_noisy;

% Function used to compute k1, k2, k3, k4
f_RungeKutta = Function('f_RungeKutta', {q, shou_vars, fore_vars, hand_vars, u_noisy}, {qdot});

% k1, k2, k3, k4 computation
k1 = f_RungeKutta(q, shou_vars, fore_vars, hand_vars, u_noisy);
k2 = f_RungeKutta(q + sample_Time*k1/2, shou_vars, fore_vars, hand_vars, u_noisy);
k3 = f_RungeKutta(q + sample_Time*k2/2, shou_vars, fore_vars, hand_vars, u_noisy);
k4 = f_RungeKutta(q + sample_Time*k3, shou_vars, fore_vars, hand_vars, u_noisy);

% Function f computation
f = q + 1/6*sample_Time*(k1 + 2*k2 + 2*k3 + k4);
f_f_exp = Function('f_f_exp', {q, shou_vars, fore_vars, hand_vars, u_noisy}, {f});

%% Linearization around eq. point
% qk+1 = Fqk + Guk
% pk = Hqk + Juk

% F, G in terms of symbolic variables
F = jacobian(f, q);
G = jacobian(f, u_noisy);

% Function F, G
f_F_exp = Function('f_F_exp', {q, shou_vars, fore_vars, hand_vars, u_noisy}, {F});
f_G_exp = Function('f_G_exp', {q, shou_vars, fore_vars, hand_vars, u_noisy}, {G});

%% Generate the mex functions

switch generate

    case 'true'

        switch zero_conf

            case 'true'

                opts = struct('main', true, 'mex', true);
        
                % Generate Phi
                f_Phi_exp.generate('f_Phi_0Conf_exp_mex.c', opts);
                mex f_Phi_0Conf_exp_mex.c;
            
                % Generate J
                f_J_exp.generate('f_J_0Conf_exp_mex.c', opts);
                mex f_J_0Conf_exp_mex.c;

                % Generate Jpseudo
                f_Jpseudo_exp.generate('f_Jpseudo_0Conf_exp_mex.c', opts);
                mex f_Jpseudo_0Conf_exp_mex.c;
                
                % Generate f
                f_f_exp.generate('f_f_0Conf_exp_mex.c', opts);
                mex f_f_0Conf_exp_mex.c;
                
                % Generate F
                f_F_exp.generate('f_Fekf_0Conf_exp_mex.c', opts);
                mex f_Fekf_0Conf_exp_mex.c;
                
                % Generate G
                f_G_exp.generate('f_Gekf_0Conf_exp_mex.c', opts);
                mex f_Gekf_0Conf_exp_mex.c;
                
                % Generate H
                f_J_exp.generate('f_Hekf_0Conf_exp_mex.c', opts);
                mex f_Hekf_0Conf_exp_mex.c;

            case 'false'

                opts = struct('main', true, 'mex', true);
        
                % Generate Phi
                f_Phi_exp.generate('f_Phi_exp_mex.c', opts);
                mex f_Phi_exp_mex.c;
            
                % Generate J
                f_J_exp.generate('f_J_exp_mex.c', opts);
                mex f_J_exp_mex.c;
                
                % Generate Jpseudo
                f_Jpseudo_exp.generate('f_Jpseudo_exp_mex.c', opts);
                mex f_Jpseudo_exp_mex.c;
                
                % Generate f
                f_f_exp.generate('f_f_exp_mex.c', opts);
                mex f_f_exp_mex.c;
                
                % Generate F
                f_F_exp.generate('f_Fekf_exp_mex.c', opts);
                mex f_Fekf_exp_mex.c;
                
                % Generate G
                f_G_exp.generate('f_Gekf_exp_mex.c', opts);
                mex f_Gekf_exp_mex.c;
                
                % Generate H
                f_J_exp.generate('f_Hekf_exp_mex.c', opts);
                mex f_Hekf_exp_mex.c;
              
        end
end

%% Rechange directory
cd ../..;
end