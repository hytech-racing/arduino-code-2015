#define BMS_TIMED_OUT 1
#define EVDC_TIMED_OUT 2
#define MC_TIMED_OUT 3
#define AR_TIMED_OUT 4
#define LOW_SOC 5
#define VERY_LOW_SOC 6
#define HIGH_BATT_TEMP 7

#define CELL_CRITICAL_LOW 9
#define TOO_MUCH_CURRENT 10
#define HIGH_PHASE_TEMP 11
#define HIGH_MOTOR_TEMP 12
#define EVDC_BASE_ERROR 13 // 13 to 18
#define AR_BASE_ERROR 19 // 19 to 25
#define IMD_BASE_ERROR 26

const int AirDCDC = 2;
const int AIRdcdc = 2;
const int software_shutdown_control = 4;
const int software_pushbutton_control = 5;

/*
okay, so the software_shutdown_control is turned on. then, the software_pushbutton_control is pulsed on for a second or so. then, AIRdcdc is turned on or off to turn AIRs on or off

*/


const int start_LED = 6;
const int IMD_LED = 7;
const int BMS_LED = 8;
const int plex_LSB = 10;
const int readyToDriveSound = 3; // ready to derp sound?
const int precharge = 2;
const int discharge = 2;
const int TSMasterRelay = 2;
const int IMDpin = 3;

