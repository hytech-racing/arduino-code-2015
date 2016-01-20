#include <SPI.h>
#include "mcp_can.h"

#pragma once

/**
 * Functions for sending and receiving data from the rear Arduino
 * Sanjay S
 */

namespace AR {
	int getError(unsigned char*);
	int Message = 0xA2; // 'Arduino 2'
}