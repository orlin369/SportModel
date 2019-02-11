// 
// 
// 

#include "DebugPort.h"

/** @brief Configure debug port.
 *  @return Void
 */
void configure_debug_port()
{
#ifdef EANBLE_DEBUG_OUT
	DEBUG_PORT.begin(DEBUG_PORT_BAUDRATE, SERIAL_8N1);
	DEBUG_PORT.print("\r\n\r\n\r\n");
	DEBUG_PORT.setDebugOutput(true);

	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // RELEASE
}
