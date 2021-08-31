/**
 * Port of the MPU-6050 Library from https://github.com/alex-mous/MPU6050-C-CPP-Library-for-Raspberry-Pi.
 * I have wrapped it up in a ROS node so it could be used conveniently. All the other drivers I tried had
 * dependencies that were too hard to fulfill. This package just worked out of the box for me.
 */

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Imu.h>

#include <mpu_6050_ros/MPU6050.h>

#include <sstream>

// I2c Address for the IMU ( Please check it by scanning i2c interface 
// on your Rpi and chacnge the address if necessary)
MPU6050 device(0x68);

float ax, ay, az, gr, gp, gy;

/**
 * @brief Gets IMU offsets which should be put in the header file 
 * to account for noise in the read values.
 * 
 * Variables to store the accel, gyro and angle values
 * @param ax 
 * @param ay 
 * @param az 
 * @param gr 
 * @param gp 
 * @param gy 
 */
void get_imu_offsets(float &ax,float &ay,float &az,float &gr,float &gp,float &gy){
  //Calculate the offsets
	std::cout << "Calculating the offsets...\n    Please keep the accelerometer level and still\n    This could take a couple of minutes...";
	device.getOffsets(&ax, &ay, &az, &gr, &gp, &gy);
	std::cout << "Gyroscope R,P,Y: " << gr << "," << gp << "," << gy << "\nAccelerometer X,Y,Z: " << ax << "," << ay << "," << az << "\n";
}

/**
 * @brief Gets the imu data 
 * 
 * Variables to store the accel, gyro and angle values
 * @param ax 
 * @param ay 
 * @param az 
 * @param gr 
 * @param gp 
 * @param gy 
 */
void get_imu_data(float &ax,float &ay,float &az,float &gr,float &gp,float &gy){

	//Read the current yaw angle
	device.calc_yaw = true;


  device.getAngle(0, &gr);
  device.getAngle(1, &gp);
  device.getAngle(2, &gy);
  // std::cout << "Current angle around the roll axis: " << gr << "\n";
  // std::cout << "Current angle around the pitch axis: " << gp << "\n";
  // std::cout << "Current angle around the yaw axis: " << gy << "\n";

	//Get the current accelerometer values
	device.getAccel(&ax, &ay, &az);
	// std::cout << "Accelerometer Readings: X: " << ax << ", Y: " << ay << ", Z: " << az << "\n";

	//Get the current gyroscope values
	device.getGyro(&gr, &gp, &gy);
	// std::cout << "Gyroscope Readings: X: " << gr << ", Y: " << gp << ", Z: " << gy << "\n";
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "IMU_Node");

  ros::NodeHandle n;

  ros::Publisher imu_pub = n.advertise<sensor_msgs::Imu>("imu", 1000);

  ros::Rate loop_rate(10);

  while (ros::ok())
  {

    sensor_msgs::Imu imu_msg;
    imu_msg.header.frame_id = "/imu";
    imu_msg.header.stamp= ros::Time::now();

    get_imu_data(ax, ay, az, gr, gp, gy);

    imu_msg.angular_velocity.x=gr;
    imu_msg.angular_velocity.y=gp;
    imu_msg.angular_velocity.z=gy;

    imu_msg.linear_acceleration.x=ax;
    imu_msg.linear_acceleration.y=ay;
    imu_msg.linear_acceleration.z=az;

    imu_pub.publish(imu_msg);

    ros::spinOnce();

    loop_rate.sleep();

  }

  return 0;
}
