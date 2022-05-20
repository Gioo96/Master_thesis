function Callback_velocity(~, message)

%% Defining GLOBAL variables
% Count every time Callback is called since C1 is met
global count;

% Measurement position and velocity
global meas_pos;
global meas_vel;

% Current time of simulation
global current_time;

% Determines when exp starts
global flag;

% Simulation output
global est;

% Saving marker sample mean, sample variance
global marker;
global covariance_pos;
% Initialization
if isempty(count)

    current_time = 0;
    count = 0;
    flag = false;
    real_time = false;

end

% Extract data from the measurement when the marker is moving
% Set threshold to 0.4
meas_pos = [message.Pose.Pose.Position.X message.Pose.Pose.Position.Y message.Pose.Pose.Position.Z];
meas_vel = [message.Twist.Twist.Linear.X message.Twist.Twist.Linear.Y message.Twist.Twist.Linear.Z];

% Condition satisfied at max once (C1)
if (meas_pos(2) > 0 && count == 0)

    flag = true;
end

% Condition always satisfied from condition C1 onward
if flag

    count = count + 1;
    next_time = current_time + 0.01;
    current_time = next_time;

    covariance_pose = reshape(message.Twist.Covariance, sqrt(length(message.Twist.Covariance)), sqrt(length(message.Twist.Covariance)));
    covariance_pos = covariance_pose(1:3, 1:3);
    disp(covariance_pos);
end

% Simulation
if (count == 1) 

    est = sim("master_thesis_simulink.slx");
   
    %% Position
    % Sample mean
    [marker.m1.pos.sample_mean, marker.m1.pos.delay] = super_marker_mean(est.super_m1_pos);
    % Sample variance
    marker.m1.pos.sample_variance = super_marker_variance(est.super_m1_pos, marker.m1.pos.sample_mean, marker.m1.pos.delay);

    %% Velocity
    % Sample mean
    [marker.m1.vel.sample_mean, marker.m1.vel.delay] = super_marker_mean(est.super_m1_vel);
    % Sample variance
    marker.m1.vel.sample_variance = super_marker_variance(est.super_m1_vel, marker.m1.vel.sample_mean, marker.m1.vel.delay);
end

end