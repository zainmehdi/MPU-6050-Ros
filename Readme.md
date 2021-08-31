# MPU-6050 ROS node
##### Based on Library by https://github.com/alex-mous ( All the credit goes to him)

  Port of the MPU-6050 Library from https://github.com/alex-mous/MPU6050-C-CPP-Library-for-Raspberry-Pi.
  I have wrapped it up in a ROS node so it could be used conveniently. All the other drivers I tried had
  dependencies that were too hard to fulfill. This package just worked out of the box for me.

  ##### Prerequisites 

The dependencies for this library are libi2c-dev, i2c-tools, and libi2c0. These can be installed with apt. The latest version of this code now works on Raspbian Buster. Note: to run the code, you will need to enable I2C in raspi-config.

```sudo apt-get install libi2c-dev i2c-tools libi2c0```
