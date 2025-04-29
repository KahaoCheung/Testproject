#include <iostream>
#include <emc/io.h>
#include <emc/rate.h>
#include <chrono>
emc::LaserData scan;

int main()
{
    // Create IO object, which will initialize the io layer
    emc::IO io;
    // Create Rate object, which will help keeping the loop at a fixed frequency
    emc::Rate r(10);
    // Loop while we are properly connected

    bool rotating = false;
    std::chrono::steady_clock::time_point rotation_start_time;

    while(io.ok())
    {
        io.readLaserData(scan);

        int center_index = scan.ranges.size() / 2;
        int window_size = 40; // amount of beams left and right

        bool obstacle_in_front = false;
        for (int i = center_index - window_size; i <= center_index + window_size; ++i)
        {
            if (i >= 0 && i < scan.ranges.size()) 
            {
                if (scan.ranges[i] < 0.4) // If closer than 0.5 meters
            
                {
                    obstacle_in_front = true;
                    break;
                }
            }
        }
        if (rotating)
        {
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed_time = now - rotation_start_time;
            
            if (elapsed_time.count() >= 3.0)
            {
                rotating = false;
            }
            else
            {
                // Send a reference to the base controller (vx, vy, vtheta)
                io.sendBaseReference(0, 0, 0.5);
            }
        }
        else if (obstacle_in_front)
        { 
            rotating = true;
            rotation_start_time = std::chrono::steady_clock::now(); // Starting timer
            // Send a reference to the base controller (vx, vy, vtheta)
            io.sendBaseReference(0, 0, 0.5);
        }
        else 
        {
            // Send a reference to the base controller (vx, vy, vtheta)
            io.sendBaseReference(0.1, 0, 0);
        }

        // Sleep remaining time
        r.sleep(); // sleep maintaining time
    }
    return 0;
}