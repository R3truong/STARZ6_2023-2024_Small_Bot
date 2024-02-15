using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor RearLeft;
extern motor RearRight;
extern motor CenterLeft;
extern motor CenterRight;
extern motor FrontLeft;
extern motor FrontRight;
extern controller Controller1;
extern motor leftWing;
extern motor rightWing;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );