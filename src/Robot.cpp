#include "WPILib.h"
class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	double rightDrive, leftDrive, ax, ay, az, lift, dummy;
	bool dumCam;
	std::string autoSelected;
	BuiltInAccelerometer *accel = new BuiltInAccelerometer();
	Joystick *rightStick = new Joystick(0);
	Joystick *leftStick  = new Joystick(1);
	USBCamera *cam =new USBCamera("Camera",1);
	Timer *timer =new Timer();
	Talon *fRight = new Talon(0);
	Talon *fLeft = new Talon(1);
	Talon *bRight = new Talon(2);
	Talon *bLeft = new Talon(3);
	Talon *pickup = new Talon(4);
	Talon *shoot =new Talon(5);
	DigitalOutput *led1 = new DigitalOutput(1);
	DigitalInput *limitswitch =new DigitalInput(2);
	RobotDrive *robotDrive = new RobotDrive(fLeft, bLeft, fRight, bRight);
	void RobotInit()
	{
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
	}


	/**
	 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
	 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
	 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
	 * below the Gyro
	 *
	 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
	 * If using the SendableChooser make sure to add them to the chooser code above as well.
	 */
	void AutonomousInit()
	{
		autoSelected = *((std::string*)chooser->GetSelected());
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}
	void AutonomousPeriodic()
	{
		timer->Start();
		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
			if(timer->Get()<=5)
			{
			pickup->Set(.1);
			rightDrive=1;
			leftDrive=1;
			rightDrive = .6;
			leftDrive  = .6;
			robotDrive->TankDrive(rightDrive, leftDrive);
			dummy++;
			}
		else
			{
			rightDrive = 0;
			leftDrive = 0;
			robotDrive->TankDrive(rightDrive, leftDrive);
				pickup->Set(0);
				if(limitswitch->Get()==true){
	timer->Reset();
				}
			}
			SmartDashboard::PutNumber("dummy",dummy);
			SmartDashboard::PutBoolean("limitswitch", limitswitch);
		}
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		rightDrive = rightStick->GetY();
		leftDrive  = leftStick->GetY();

		rightDrive = .7*rightDrive;
		leftDrive  = .7*leftDrive;
		robotDrive->TankDrive(rightDrive, leftDrive);
		ax = accel-> GetX();
		ay = accel-> GetY();
		az = accel-> GetZ();

		bool triggerRight = rightStick->GetRawButton(1);
		bool triggerLeft = leftStick->GetRawButton(1); // any idea why these are throwing errors? seems to not mess with build or clean
		bool buttonLed = rightStick->GetRawButton(2);
		bool buttonShoot = leftStick->GetRawButton(3);
		bool buttonCam = rightStick->GetRawButton(4);

		SmartDashboard::PutBoolean("trigger", triggerRight);
		SmartDashboard::PutBoolean("trigger", triggerLeft);
		SmartDashboard::PutBoolean("buttonLed", buttonLed);
		SmartDashboard::PutBoolean("buttonShoot", buttonShoot);
		SmartDashboard::PutBoolean("buttonCam", buttonCam);
		SmartDashboard::PutData("Auto Modes", chooser);
		SmartDashboard::PutNumber("ax",ax);
		SmartDashboard::PutNumber("ay",ay);
		SmartDashboard::PutNumber("az",az);

		if(triggerRight){
			pickup->Set(1);
		}
		else if(triggerLeft){
			pickup->Set(-1);
		}
		else{
			pickup->Set(0);
		}
		if(buttonLed){
			led1->Pulse(1);
		}
		else{
			led1->Pulse(0);
		}
		if (buttonCam==true && dumCam==false){
			cam->StartCapture();
		dumCam=true;
		}
		else if(buttonCam==false && dumCam==true){
			dumCam=false;
			cam->StopCapture();
		}
		if(buttonShoot){
			shoot->Set(1);
		}
		else{
			shoot->Set(0);
		}
	}
	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
