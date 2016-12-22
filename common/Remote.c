/**
 * \file
 * \brief Radio Remote Module
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * Module to handle accelerometer values passed over the Radio.
 */

#include "Platform.h" /* interface to the platform */
#if PL_CONFIG_HAS_REMOTE
#include "Remote.h"
#include "FRTOS1.h"
#include "CLS1.h"
#include "UTIL1.h"
#include "Shell.h"
#endif
#if PL_CONFIG_HAS_PID
  #include "PID.h"
#endif
#if PL_CONFIG_HAS_MOTOR
  #include "Motor.h"
#endif
#if PL_CONFIG_HAS_RADIO
  #include "RNet_App.h"
  #include "RNet_AppConfig.h"
#endif
#if PL_CONFIG_HAS_DRIVE
  #include "Drive.h"
#endif
#if PL_CONFIG_HAS_LEDS
  #include "LED.h"
#endif
#if PL_CONFIG_HAS_JOYSTICK
  #include "AD1.h"
#endif
#if PL_CONFIG_HAS_SHELL
  #include "Shell.h"
#endif
#if PL_CONFIG_HAS_REFLECTANCE
	#include "Reflectance.h"
	#include "LineFollow.h"
#endif
#if PL_CONFIG_HAS_BUZZER
	#include "Buzzer.h"
#endif
#if PL_CONFIG_HAS_RADIO
  #include "RNet_AppConfig.h"
  #include "RPHY.h"
  #include "RNet_App.h"
  #include "RApp.h"
  #include "Radio.h"
#if PL_CONFIG_HAS_DIGITALJOYSTICK
	#include "SW1.h"
    #include "SW2.h"
    #include "SW3.h"
    #include "SW4.h"
    #include "SW5.h"
    #include "SW6.h"
    #include "SW7.h"
#endif

static bool REMOTE_isOn = FALSE;
static bool REMOTE_isVerbose = FALSE;
static bool REMOTE_useJoystick = TRUE;
static RACE_STATE state = RACE_START;

int8_t setdirection = 50;
int8_t setspeed = 50;
int8_t actualdirection = 0;
int8_t actualspeed = 0;

#if PL_CONFIG_HAS_JOYSTICK
static uint16_t midPointX, midPointY;
#endif

void REMOTE_SetDriveMode(void)
 {
	uint8_t buf = RAPP_BTN_MSG_DRIVE;
	RAPP_SendPayloadDataBlock(&buf, sizeof(buf), RAPP_MSG_TYPE_JOYSTICK_BTN,RNETA_GetDestAddr(), RPHY_PACKET_FLAGS_REQ_ACK);
 }

void REMOTE_StartLineFollowing(void)
 {
	 uint8_t buf = RAPP_BTN_MSG_A;
		 RAPP_SendPayloadDataBlock(&buf, sizeof(buf), RAPP_MSG_TYPE_JOYSTICK_BTN,RNETA_GetDestAddr(), RPHY_PACKET_FLAGS_REQ_ACK);
 }
void REMOTE_StartCalib(void)
 {
	uint8_t buf = RAPP_BTN_MSG_CALIB;
	 RAPP_SendPayloadDataBlock(&buf, sizeof(buf), RAPP_MSG_TYPE_JOYSTICK_BTN,RNETA_GetDestAddr(), RPHY_PACKET_FLAGS_REQ_ACK);
 }
void REMOTE_Stop(void)
 {
	actualdirection = 0;
	actualspeed = 0;
	uint8_t buf[2];
	    buf[0] =  0;
	    buf[1] =  0;
	    //SHELL_SendString("LCD Up\r\n");
	   (void)RAPP_SendPayloadDataBlock(buf, sizeof(buf), RAPP_MSG_TYPE_JOYSTICK_XY, RNETA_GetDestAddr(), RPHY_PACKET_FLAGS_REQ_ACK);
	//uint8_t buf = RAPP_BTN_MSG_STOP;
	// RAPP_SendPayloadDataBlock(&buf, sizeof(buf), RAPP_MSG_TYPE_JOYSTICK_BTN,RNETA_GetDestAddr(), RPHY_PACKET_FLAGS_REQ_ACK);

 }
void REMOTE_Horn(void)
 {
	 uint8_t buf = RAPP_BTN_MSG_A;
	 RAPP_SendPayloadDataBlock(&buf, sizeof(buf), RAPP_MSG_TYPE_JOYSTICK_BTN,RNETA_GetDestAddr(), RPHY_PACKET_FLAGS_REQ_ACK);
 }

void REMOTE_SendDirection(int8_t direction){
	actualdirection = direction;
	uint8_t buf[2];
    buf[0] = actualdirection;
    buf[1] = actualspeed;
    //SHELL_SendString("LCD Up\r\n");
    (void)RAPP_SendPayloadDataBlock(buf, sizeof(buf), RAPP_MSG_TYPE_JOYSTICK_XY, RNETA_GetDestAddr(), RPHY_PACKET_FLAGS_REQ_ACK);
    uint8_t txtBuf[48];
	UTIL1_strcpy(txtBuf, sizeof(txtBuf), (unsigned char*) "SendXY: x: ");
	UTIL1_strcatNum8s(txtBuf, sizeof(txtBuf), actualdirection);
	UTIL1_strcat(txtBuf, sizeof(txtBuf), (unsigned char*) " y: ");
	UTIL1_strcatNum8s(txtBuf, sizeof(txtBuf), actualspeed);
	UTIL1_strcat(txtBuf, sizeof(txtBuf), (unsigned char*) "\r\n");
	SHELL_SendString(txtBuf);
}

void REMOTE_SendSpeed(int8_t speed){
	actualspeed = speed;
	uint8_t buf[2];
    buf[0] = actualdirection;
    buf[1] = actualspeed;
    //SHELL_SendString("LCD Up\r\n");
    (void)RAPP_SendPayloadDataBlock(buf, sizeof(buf), RAPP_MSG_TYPE_JOYSTICK_XY, RNETA_GetDestAddr(), RPHY_PACKET_FLAGS_REQ_ACK);
    uint8_t txtBuf[48];
	UTIL1_strcpy(txtBuf, sizeof(txtBuf), (unsigned char*) "SendXY: x: ");
	UTIL1_strcatNum8s(txtBuf, sizeof(txtBuf), actualdirection);
	UTIL1_strcat(txtBuf, sizeof(txtBuf), (unsigned char*) " y: ");
	UTIL1_strcatNum8s(txtBuf, sizeof(txtBuf), actualspeed);
	UTIL1_strcat(txtBuf, sizeof(txtBuf), (unsigned char*) "\r\n");
	SHELL_SendString(txtBuf);
}
void REMOTE_SendXY(int8_t direction, int8_t speed){
	//actualdirection = direction;
	//actualspeed = speed;

	uint8_t buf[2];
    buf[0] = direction;
    buf[1] = speed;
    //SHELL_SendString("LCD Up\r\n");
    (void)RAPP_SendPayloadDataBlock(buf, sizeof(buf), RAPP_MSG_TYPE_JOYSTICK_XY, RNETA_GetDestAddr(), RPHY_PACKET_FLAGS_REQ_ACK);
    uint8_t txtBuf[48];
	UTIL1_strcpy(txtBuf, sizeof(txtBuf), (unsigned char*) "SendXY: x: ");
	UTIL1_strcatNum8s(txtBuf, sizeof(txtBuf), direction);
	UTIL1_strcat(txtBuf, sizeof(txtBuf), (unsigned char*) " y: ");
	UTIL1_strcatNum8s(txtBuf, sizeof(txtBuf), speed);
	UTIL1_strcat(txtBuf, sizeof(txtBuf), (unsigned char*) "\r\n");
	SHELL_SendString(txtBuf);
}

void REMOTE_SetSpeed(int8_t speed){
	setspeed = speed;
}
void REMOTE_SetDirection(int8_t direction){
	setdirection = direction;
}

#if PL_CONFIG_CONTROL_SENDER | PL_CONFIG_HAS_DIGITALJOYSTICK
#if PL_CONFIG_HAS_JOYSTICK
static int8_t ToSigned8Bit(uint16_t val, bool isX) {
  int32_t tmp;

  if (isX) {
    tmp = (int32_t)val-midPointX;
  } else {
    tmp = (int32_t)val-midPointY;
  }
  if (tmp>0) {
    tmp = (tmp*128)/0x7fff;
  } else {
    tmp = (-tmp*128)/0x7fff;
    tmp = -tmp;
  }
  if (tmp<-128) {
    tmp = -128;
  } else if (tmp>127) {
    tmp = 127;
  }
  return (int8_t)tmp;
}

static uint8_t REMOTE_GetXY(uint16_t *x, uint16_t *y, int8_t *x8, int8_t *y8) {
  uint8_t res;
  uint16_t values[2];

  res = AD1_Measure(TRUE);
  if (res!=ERR_OK) {
    return res;
  }
  res = AD1_GetValue16(&values[0]);
  if (res!=ERR_OK) {
    return res;
  }
  if (x!=NULL) {
    *x = values[0];
  }
  if (y!=NULL) {
    *y = values[1];
  }
  /* transform into -128...127 with zero as mid position */
  if (x8!=NULL) {
    *x8 = ToSigned8Bit(values[0], TRUE);
  }
  if (y8!=NULL) {
    *y8 = ToSigned8Bit(values[1], FALSE);
  }
  return ERR_OK;
}
#endif

static void RemoteTask (void *pvParameters) {
  (void)pvParameters;
#if PL_CONFIG_HAS_JOYSTICK
  (void)REMOTE_GetXY(&midPointX, &midPointY, NULL, NULL);
#endif
  FRTOS1_vTaskDelay(1000/portTICK_PERIOD_MS);
  for(;;) {
    if (REMOTE_isOn) {
#if PL_CONFIG_HAS_JOYSTICK
      if (REMOTE_useJoystick) {
    	static int8_t oldx8[2];
    	static int8_t oldy8[2];
        uint8_t buf[2];
        int16_t x, y;
        int8_t x8, y8;

        /* send periodically messages */
        REMOTE_GetXY(&x, &y, &x8, &y8);
        if(x8 != oldx8[1] || y8 != oldy8[1]){
        buf[0] = x8;
        buf[1] = y8;
        if (REMOTE_isVerbose) {
          uint8_t txtBuf[48];

          UTIL1_strcpy(txtBuf, sizeof(txtBuf), (unsigned char*)"TX: x: ");
          UTIL1_strcatNum8s(txtBuf, sizeof(txtBuf), x8);
          UTIL1_strcat(txtBuf, sizeof(txtBuf), (unsigned char*)" y: ");
          UTIL1_strcatNum8s(txtBuf, sizeof(txtBuf), y8);
          UTIL1_strcat(txtBuf, sizeof(txtBuf), (unsigned char*)" to addr 0x");
    #if RNWK_SHORT_ADDR_SIZE==1
          UTIL1_strcatNum8Hex(txtBuf, sizeof(txtBuf), RNETA_GetDestAddr());
    #else
          UTIL1_strcatNum16Hex(txtBuf, sizeof(txtBuf), RNETA_GetDestAddr());
    #endif
          UTIL1_strcat(txtBuf, sizeof(txtBuf), (unsigned char*)"\r\n");
          SHELL_SendString(txtBuf);
        }
        (void)RAPP_SendPayloadDataBlock(buf, sizeof(buf), RAPP_MSG_TYPE_JOYSTICK_XY, RNETA_GetDestAddr(), RPHY_PACKET_FLAGS_REQ_ACK);
        LED1_Neg();
      }
        oldx8[1] = oldx8[0];
        oldy8[1] = oldy8[0];
        oldx8[0] = x8;
        oldy8[0] = y8;
     }
#endif
#if PL_CONFIG_HAS_DIGITALJOYSTICK

  	static int8_t olddirection[2];
  	static int8_t oldspeed[2];
     int8_t direction = 0;
	 int8_t speed = 0;
      if(!SW1_GetVal()) //right
      {
    	  direction = setdirection;
      }
      if(!SW2_GetVal()) //left
      {
    	  direction = -setdirection;
      }

      if(!SW7_GetVal()) //forwards
      {
    	  speed = setspeed;
      }

      if(!SW6_GetVal()) //backwards
      {
    	  speed = -setspeed;
      }
      if(direction != olddirection[1] || speed != oldspeed[1]){
      REMOTE_SendXY(direction, speed);
      }
      olddirection[1] = olddirection[0];
      oldspeed[1] = oldspeed[0];
      olddirection[0] = direction;
      oldspeed[0] = speed;
#endif
      FRTOS1_vTaskDelay(200/portTICK_PERIOD_MS);
    } else {
      FRTOS1_vTaskDelay(1000/portTICK_PERIOD_MS);
    }
  } /* for */
}
#endif

#if PL_CONFIG_HAS_MOTOR
static void REMOTE_HandleMotorMsg(int16_t speedVal, int16_t directionVal, int16_t z) {
  #define SCALE_DOWN 30
  #define MIN_VALUE  250 /* values below this value are ignored */
  #define DRIVE_DOWN 1

  if (!REMOTE_isOn) {
    return;
  }
  if (z<-900) { /* have a way to stop motor: turn FRDM USB port side up or down */
#if PL_CONFIG_HAS_DRIVE
    DRV_SetSpeed(0, 0);
#else
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_LEFT), 0);
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), 0);
#endif
  } else if ((directionVal>MIN_VALUE || directionVal<-MIN_VALUE) && (speedVal>MIN_VALUE || speedVal<-MIN_VALUE)) {
    int16_t speed, speedL, speedR;
    
    speed = speedVal/SCALE_DOWN;
    if (directionVal<0) {
      if (speed<0) {
        speedR = speed+(directionVal/SCALE_DOWN);
      } else {
        speedR = speed-(directionVal/SCALE_DOWN);
      }
      speedL = speed;
    } else {
      speedR = speed;
      if (speed<0) {
        speedL = speed-(directionVal/SCALE_DOWN);
      } else {
        speedL = speed+(directionVal/SCALE_DOWN);
      }
    }
#if PL_CONFIG_HAS_DRIVE
    DRV_SetSpeed(speedL*SCALE_DOWN/DRIVE_DOWN, speedR*SCALE_DOWN/DRIVE_DOWN);
#else
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_LEFT), speedL);
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), speedR);
#endif
  } else if (speedVal>100 || speedVal<-100) { /* speed */
#if PL_CONFIG_HAS_DRIVE
    DRV_SetSpeed(speedVal, speedVal);
#else
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_LEFT), -speedVal/SCALE_DOWN);
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), -speedVal/SCALE_DOWN);
#endif
  } else if (directionVal>100 || directionVal<-100) { /* direction */
#if PL_CONFIG_HAS_DRIVE
    DRV_SetSpeed(directionVal/DRIVE_DOWN, -directionVal/DRIVE_DOWN);
#else
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_LEFT), -directionVal/SCALE_DOWN);
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), (directionVAl/SCALE_DOWN));
#endif
  } else { /* device flat on the table? */
#if PL_CONFIG_HAS_DRIVE
    DRV_SetSpeed(0, 0);
#else
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_LEFT), 0);
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), 0);
#endif
  }
}
#endif

#if PL_CONFIG_HAS_MOTOR
static int16_t scaleJoystickTo4K(int8_t val) {
  /* map speed from -128...127 to -4000...+4000 */
  int tmp;

  if (val>0) {
    tmp = ((val*10)/127)*400;
  } else {
    tmp = ((val*10)/128)*400;
  }
  if (tmp<-4000) {
    tmp = -4000;
  } else if (tmp>4000) {
    tmp = 4000;
  }
  return tmp;
}
#endif

uint8_t REMOTE_HandleRemoteRxMessage(RAPP_MSG_Type type, uint8_t size, uint8_t *data, RNWK_ShortAddrType srcAddr, bool *handled, RPHY_PacketDesc *packet) {
#if PL_CONFIG_HAS_SHELL
  uint8_t buf[48];
#endif
  uint8_t val;
  int16_t x, y, z;
  static bool sigSend = FALSE;
  (void)size;
  (void)packet;
  switch(type) {
#if PL_CONFIG_HAS_MOTOR
    case RAPP_MSG_TYPE_JOYSTICK_XY: /* values are -128...127 */
      {
        int8_t x, y;
        int16_t x1000, y1000;

        *handled = TRUE;
        x = *data; /* get x data value */
        y = *(data+1); /* get y data value */
        if (REMOTE_isVerbose) {
          UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"x/y: ");
          UTIL1_strcatNum8s(buf, sizeof(buf), (int8_t)x);
          UTIL1_chcat(buf, sizeof(buf), ',');
          UTIL1_strcatNum8s(buf, sizeof(buf), (int8_t)y);
          UTIL1_strcat(buf, sizeof(buf), (unsigned char*)"\r\n");
          SHELL_SendString(buf);
        }
  #if 0 /* using shell command */
        UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"motor L duty ");
        UTIL1_strcatNum8s(buf, sizeof(buf), scaleSpeedToPercent(x));
        SHELL_ParseCmd(buf);
        UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"motor R duty ");
        UTIL1_strcatNum8s(buf, sizeof(buf), scaleSpeedToPercent(y));
        SHELL_ParseCmd(buf);
  #endif
        /* filter noise around zero */
        if (x>-5 && x<5) {
          x = 0;
        }
        if (y>-5 && y<5) {
          y = 0;
        }
        x1000 = scaleJoystickTo4K(x);
        y1000 = scaleJoystickTo4K(y);
        if (REMOTE_useJoystick) {
          REMOTE_HandleMotorMsg(y1000, x1000, 0); /* first param is forward/backward speed, second param is direction */
          if(y1000 > 49 && !sigSend)
          {
        	  sigSend = TRUE;
        	  SendSignal(RAPP_SIG_A);
          }
        }
      }
      break;
#endif
    case RAPP_MSG_TYPE_JOYSTICK_BTN:
      *handled = TRUE;
      val = *data; /* get data value */
#if PL_CONFIG_HAS_SHELL && PL_CONFIG_HAS_BUZZER && PL_CONFIG_HAS_REMOTE
      if (val=='F') { /* F button, disable remote */
        //SHELL_ParseCmd((unsigned char*)"buzzer buz 300 500");
        REMOTE_SetOnOff(FALSE);
        DRV_SetSpeed(0,0); /* turn off motors */
        SHELL_SendString("Remote OFF\r\n");
      } else if (val=='G') { /* center joystick button: enable remote */
        //SHELL_ParseCmd((unsigned char*)"buzzer buz 300 1000");
        REMOTE_SetOnOff(TRUE);
        DRV_SetMode(DRV_MODE_SPEED);
        SHELL_SendString("Remote ON\r\n");
      } else if (val=='C') { /* red 'C' button calibrate reflectance sensors*/
    	 REF_CalibrateStartStop();
    	 //DRV_SetMode(DRV_MODE_STOP);
      } else if (val=='A') { /* green 'A' button hooorn */
			SendSignal(RAPP_SIG_B);
			LF_StartFollowing();

      } else if (val=='B') { /* green 'A' button hooorn */
      	//BUZ_PlayTune(BUZ_TUNE_WELCOME);
      	  //send signal B
      }
#else
      *handled = FALSE; /* no shell and no buzzer? */
#endif
      break;

    default:
      break;
  } /* switch */
  return ERR_OK;
}

void SendSignal(RAPP_SIG_MSG_t symbol){
	  uint8_t buf[2];
	  buf[0]  =  0x01 ;
	  buf[1]  =  symbol;
	  RAPP_SendPayloadDataBlock(&buf, sizeof(buf), RAPP_MSG_TYPE_SIGNALS, DST_TIMEMEASURE, RPHY_PACKET_FLAGS_REQ_ACK);
}

#if PL_CONFIG_HAS_JOYSTICK
static void StatusPrintXY(CLS1_ConstStdIOType *io) {
  uint16_t x, y;
  int8_t x8, y8;
  uint8_t buf[64];

  if (REMOTE_GetXY(&x, &y, &x8, &y8)==ERR_OK) {
    UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"X: 0x");
    UTIL1_strcatNum16Hex(buf, sizeof(buf), x);
    UTIL1_strcat(buf, sizeof(buf), (unsigned char*)"(");
    UTIL1_strcatNum8s(buf, sizeof(buf), x8);
    UTIL1_strcat(buf, sizeof(buf), (unsigned char*)") Y: 0x");
    UTIL1_strcatNum16Hex(buf, sizeof(buf), y);
    UTIL1_strcat(buf, sizeof(buf), (unsigned char*)"(");
    UTIL1_strcatNum8s(buf, sizeof(buf), y8);
    UTIL1_strcat(buf, sizeof(buf), (unsigned char*)")\r\n");
  } else {
    UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"GetXY() failed!\r\n");
  }
  CLS1_SendStatusStr((unsigned char*)"  analog", buf, io->stdOut);
}
#endif

static void REMOTE_PrintHelp(const CLS1_StdIOType *io) {
  CLS1_SendHelpStr((unsigned char*)"remote", (unsigned char*)"Group of remote commands\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows remote help or status\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  on|off", (unsigned char*)"Turns the remote on or off\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  verbose on|off", (unsigned char*)"Turns the verbose mode on or off\r\n", io->stdOut);
#if PL_CONFIG_HAS_JOYSTICK
  CLS1_SendHelpStr((unsigned char*)"  joystick on|off", (unsigned char*)"Use joystick\r\n", io->stdOut);
#endif
}

static void REMOTE_PrintStatus(const CLS1_StdIOType *io) {
  CLS1_SendStatusStr((unsigned char*)"remote", (unsigned char*)"\r\n", io->stdOut);
  CLS1_SendStatusStr((unsigned char*)"  remote", REMOTE_isOn?(unsigned char*)"on\r\n":(unsigned char*)"off\r\n", io->stdOut);
  CLS1_SendStatusStr((unsigned char*)"  joystick", REMOTE_useJoystick?(unsigned char*)"on\r\n":(unsigned char*)"off\r\n", io->stdOut);
  CLS1_SendStatusStr((unsigned char*)"  verbose", REMOTE_isVerbose?(unsigned char*)"on\r\n":(unsigned char*)"off\r\n", io->stdOut);
#if PL_CONFIG_HAS_JOYSTICK
  StatusPrintXY(io);
#endif
}

uint8_t REMOTE_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io) {
  uint8_t res = ERR_OK;

  if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"remote help")==0) {
    REMOTE_PrintHelp(io);
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"remote status")==0) {
    REMOTE_PrintStatus(io);
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, (char*)"remote on")==0) {
    REMOTE_isOn = TRUE;
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, (char*)"remote off")==0) {
#if PL_CONFIG_HAS_MOTOR
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_LEFT), 0);
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), 0);
#endif
    REMOTE_isOn = FALSE;
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, (char*)"remote verbose on")==0) {
    REMOTE_isVerbose = TRUE;
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, (char*)"remote verbose off")==0) {
    REMOTE_isVerbose = FALSE;
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, (char*)"remote joystick on")==0) {
    REMOTE_useJoystick = TRUE;
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, (char*)"remote joystick off")==0) {
    REMOTE_useJoystick = FALSE;
    *handled = TRUE;
  }
  return res;
}

bool REMOTE_GetOnOff(void) {
  return REMOTE_isOn;
}

void REMOTE_SetOnOff(bool on) {
  REMOTE_isOn = on;
}

void REMOTE_Deinit(void) {
  /* nothing to do */
}

/*! \brief Initializes module */
void REMOTE_Init(void) {
  //REMOTE_isOn = TRUE;
  REMOTE_isVerbose = FALSE;
  REMOTE_useJoystick = TRUE;
#if PL_CONFIG_CONTROL_SENDER | PL_CONFIG_HAS_DIGITALJOYSTICK
  if (FRTOS1_xTaskCreate(RemoteTask, "Remote", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS) {
    for(;;){} /* error */
  }
#endif
}
#endif /* PL_CONFIG_HAS_REMOTE */

