#include <webots/robot.h>
#include <webots/distance_sensor.h>
#include <stdio.h>
#define TIME_STEP 32
#define MAX_SPEED 5.5
#define ULTRA_SPEED 6.0
//Main
int main() 
{
        wb_robot_init();
        // initialize/enable ground sensors
        WbDeviceTag gs_tag_left;
        WbDeviceTag gs_tag_center;
        WbDeviceTag gs_tag_right;
        gs_tag_left   = wb_robot_get_device("gs0");
        gs_tag_center = wb_robot_get_device("gs1");
        gs_tag_right  = wb_robot_get_device("gs2");
        wb_distance_sensor_enable(gs_tag_left, TIME_STEP);
        wb_distance_sensor_enable(gs_tag_center, TIME_STEP);
        wb_distance_sensor_enable(gs_tag_right, TIME_STEP);
        WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
        WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
        wb_motor_set_position(left_motor, INFINITY);
        wb_motor_set_position(right_motor, INFINITY);
        wb_motor_set_velocity(left_motor, 0.0);
        wb_motor_set_velocity(right_motor, 0.0);
        int end = 0;
        double time_steps = 0.0;
        int ol = 0;
 
        
        while (wb_robot_step(TIME_STEP) != -1) {
        //time_steps
        time_steps = time_steps + 1;
        //double time = time_steps * 0.001;
        //gets the value form all three sensors
               
        int senL = wb_distance_sensor_get_value(gs_tag_left);
        int senR = wb_distance_sensor_get_value(gs_tag_right);
        int line = wb_distance_sensor_get_value(gs_tag_center);
        
        bool line_left = (senL > 500 && line > 500 && senR <500);
        //printf("\nLinks ");
        //printf("%d\n", line_left);
        bool line_right = (senR > 500 && line > 500 && senL < 500);
        //printf("\nRechts ");
        //printf("%d\n", line_right);
        bool on_line = (line < 500 && senR > 500 && senL > 500);
        //printf("\nLinie ");
        //printf("%d\n", on_line);
        bool stop = (line < 500 && senR < 500 && senL < 500);
        //printf("\nStop ");
        //printf("%d\n", stop);
        //printf("\nEnd %d",end);
        //printf("\n\nTimesteps: %f",(time_steps*32)/1000);
        
        if (on_line){
               end = 0;
               
               if(ol < 2)
               {
               wb_motor_set_velocity(left_motor, MAX_SPEED);
               wb_motor_set_velocity(right_motor, MAX_SPEED);
               ol = ol + 1;
               }
               else{
               printf("\nULTRA\n");
               wb_motor_set_velocity(left_motor, ULTRA_SPEED);
               wb_motor_set_velocity(right_motor, ULTRA_SPEED);
               }
        }
        else if(line_left){
               end = 0;
               ol = 0;
               wb_motor_set_velocity(right_motor, MAX_SPEED * 0.15); //0.15
        }
        else if(line_right){
               end = 0;
               ol = 0;
               wb_motor_set_velocity(left_motor, MAX_SPEED * 0.15); //0.15
        }
        else if(stop){  
               if(end < 20){
                       wb_motor_set_velocity(left_motor, ULTRA_SPEED);
                       wb_motor_set_velocity(right_motor, ULTRA_SPEED);
                       
                       end = end + 1;
               }
               if (end == 20){
                       //printf("\n");
                       printf("\n\nRoboterzeit: %.3f\n",(time_steps*32)/1000);
                       wb_motor_set_velocity(left_motor, 0.0);
                       wb_motor_set_velocity(right_motor, 0.0);
                       //printf("\n\nRoboterzeit: %.3f",(time_steps*32)/1000);
                       goto Stop;
               }
        }
        }
        Stop:
        
        wb_robot_cleanup();
        return 0;
}