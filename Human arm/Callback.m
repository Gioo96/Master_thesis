function Callback(~, message)

% Current measurement
global meas;
global m1_vel;
persistent count;
if isempty(count)

    count = 0;
end
% Extract data from the measurement when the marker is moving
% Set threshold to 0.1
meas = [message.Twist.Twist.Linear.X message.Twist.Twist.Linear.Y message.Twist.Twist.Linear.Z];
if (norm(meas' - zeros(3,1)) > 0.2 && count == 0)

    m1_vel = sim("master_thesis_simulink.slx");
    count = 1;
    %pp = [pp; meas];
end

end