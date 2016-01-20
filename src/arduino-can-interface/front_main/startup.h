#include <SPI.h>
#include "mcp_can.h"
#include "mc.h"
#include "evdc.h"
#include "bms.h"
#include "ar.h"
#include "pi.h"
#include "IMD.h"


boolean startupSequence(MCP_CAN& lilEngineThatCAN) { // 0 means a normal startup, 1 means BMS resset, 2 means IMD reset
// EVDC get progression button
  //progression: 1-5 normal, 6 is BMS error, 7 is IMD error 
  const int TSmasterSwitch = 0;
  const int Air1 = 0;
  const int Air2 = 0;
  const int Air3 = 0;
  const int Air4 = 0;
  const int precharge = 0;
  const int readyToDriveSound = 0; 
  
  unsigned char msgGet[8];
  unsigned char msgSend[8];
  unsigned char biglen;
  boolean IMDswitch;
  boolean progressButton;
  boolean oldProgressButton;
  boolean needsToProgress;
  while(!needsToProgress){
    RPi::giveProgression(lilEngineThatCAN, 1); // "Press button 1 to begin startup"
    if(CAN_MSGAVAIL == lilEngineThatCAN.checkReceive()) {
      lilEngineThatCAN.readMsgBuf(&biglen, msgGet);
      switch(lilEngineThatCAN.getCanId()) {
        case EVDC::Message:
         
          oldProgressButton = progressButton;
          int derp = EVDC::getButtons(msgGet);
          
          break;
      }
    }
    if(progressButton && !oldProgressButton) {
      needsToProgress = true;
    }
  }
  needsToProgress = false;
  
  
  while(!needsToProgress){
    RPi::giveProgression(lilEngineThatCAN,2); // "checking for BMS errors"
    if(CAN_MSGAVAIL == lilEngineThatCAN.checkReceive()) {
      lilEngineThatCAN.readMsgBuf(&biglen, msgGet);
      switch(lilEngineThatCAN.getCanId()) {
        case BMS::Message_1:
          if(!BMS::getErrors()) {
            needsToProgress = true;
          }
          break;
        default:
          break;
      } 
    }
    if(progressButton && !oldProgressButton) {
      needsToProgress = true;
    }
  }
  needsToProgress = false;
  
  while(!needsToProgress){
    RPi::giveProgression(lilEngineThatCAN, 3); // "checking for IMD errors"
    if(IMD::checkError() == 0) {
      needsToProgress = true;
  }
  needsToProgress = false;

  while(!needsToProgress){
    RPi::giveProgression(lilEngineThatCAN, 4);
    if(CAN_MSGAVAIL == lilEngineThatCAN.checkReceive()) {
      lilEngineThatCAN.readMsgBuf(&biglen, msgGet);
      switch(lilEngineThatCAN.getCanId()) {
        case EVDC::Message:
        
          oldProgressButton = progressButton;
          int demButtons = EVDC::getButtons(msgGet);
          progressButton = ((demButtons & 0x02) == 0x02);
          break;

      }
    }
    if(progressButton & !oldProgressButton) {
      needsToProgress = true;
    }
  }
  needsToProgress = false;
   digitalWrite(TSmasterSwitch, HIGH);  // turn on Tractive System Master Switch if there aren't any BMS or IMD errors

 /* 
 while(!needsToProgress){
    if(CAN_MSGAVAIL == lilEngineThatCAN.checkReceive()) {
      lilEngineThatCAN.readMsgBuf(&biglen, msgGet);
      switch(lilEngineThatCAN.getCanId()) {
        case EVDC::Message:
        
          oldProgressButton = progressButton;
          EVDC::getButtonStates(IMDswitch, progressButton, msgReceive);
          
          break;
        default:
          break;
      }
    }
    if(progressButton && !oldProgressButton) {
      needsToProgress = true;
    }
  }
  needsToProgress = false;
  */
  

  
  while(!needsToProgress){
    if(CAN_MSGAVAIL == lilEngineThatCAN.checkReceive()) {
      lilEngineThatCAN.readMsgBuf(&biglen, msgGet);
      switch(lilEngineThatCAN.getCanId()) {
        case EVDC::Message:
        
          oldProgressButton = progressButton;
          int demButtons = EVDC::getButtons(msgGet);
          progressButton = ((demButtons & 0x02) == 0x02);
          
          break;
      }
    }
    if(progressButton) {
      needsToProgress = true;
    }
  }
  needsToProgress = false;
       
 
  digitalWrite(Air1, HIGH); // turn on 3 AIRs when IMD bypass switch is flipped
  delay(500);
  digitalWrite(Air2, HIGH);
  delay(500);
  digitalWrite(Air3, HIGH);      
  
  
    while(!needsToProgress){
    if(CAN_MSGAVAIL == lilEngineThatCAN.checkReceive()) {
      lilEngineThatCAN.readMsgBuf(&biglen, msgGet);
      switch(lilEngineThatCAN.getCanId()) {
        case EVDC::Message:
        
          oldProgressButton = progressButton;
          int demButtons = EVDC::getButtons(msgGet);
          progressButton = ((demButtons & 0x02) == 0x02);          
          break;
      }
    }
    if(!IMDswitch) {
      needsToProgress = true;
    }
  }
  needsToProgress = false;
 
       
  float brakePedal;
  while(!needsToProgress){
    if(CAN_MSGAVAIL == lilEngineThatCAN.checkReceive()) {
      lilEngineThatCAN.readMsgBuf(&biglen, msgGet);
      switch(lilEngineThatCAN.getCanId()) {
        case EVDC::Message:
          oldProgressButton = progressButton;
          int demButtons = EVDC::getButtons(msgGet);
          progressButton = ((demButtons & 0x02) == 0x02);         
          brakePedal = EVDC::getBrakes(msgGet);
          break;
      }
    }
    if(progressButton && !oldProgressButton && (brakePedal > 0.5)){
      needsToProgress = true;
    }
  }
  needsToProgress = false;
  digitalWrite(precharge, HIGH);
  delay(3000);
  digitalWrite(Air4, HIGH); // brake needs to be pressed in order to go into a ready-to-drive state
  digitalWrite(precharge, LOW);
  digitalWrite(readyToDriveSound, HIGH);
  delay(2000);
  digitalWrite(readyToDriveSound, LOW);
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
    /*    
  while(!EVDC::getProgressionSwitch()){
    RPi::giveProgression(1);
  }
  digitalWrite(TSMasterRelay, HIGH);

  if(BMS::getError()) {
    RPi::giveProgression(6);
    return false;
  }
  if(getImdError()) {
    RPi::giveProgression(7);
    return false;
  }
  
  
  while(!EVDC::getImdBypassSwitch()) {
    RPi::giveProgression(2);
  }
  digitalWrite(Air1, HIGH);
  delay(500);
  digitalWrite(Air2, HIGH);
  delay(500);
  digitalWrite(Air3, HIGH);
  while(!EVDC::getProgressionSwitch() {
    RPi::giveProgression(3);
  }
  digitalWrite(prechargeRelay, HIGH);
  delay(3000); // check how long
  digitalWrite(Air4, HIGH);
  digitalWrite(prechargeRelay, LOW);
  // now we have high voltage power to the motor controller
  digitalWrite(readyToDriveSound, HIGH);
  while(EVDC::getImdBypassSwitch()) {// the bypass swtich needs to be turned off to progress
    RPi::giveProgression(4);
  }
  RPi::giveProgression(5); // this will display ready to drive
  return true;
  */
}
  
  
  
  
  