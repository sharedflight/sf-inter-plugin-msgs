
extern "C" {
	#include "XPLMPlugin.h"
	#include "XPLMProcessing.h"
	#include "XPLMUtilities.h"
}

#include <cstddef>
#include <cstring>

#include "SharedFlightMessaging.h"


static XPLMPluginID sfPluginID = XPLM_NO_PLUGIN_ID;

static SharedFlightInterPluginInterface sfPluginInterface;

float flightLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon);

PLUGIN_API int XPluginStart(char * outName, char * outSig, char * outDesc) {
    // Plugin details
	strcpy(outName, "Shared Flight Messaging Example plugin");
	strcpy(outSig, "com.sharedflight.sf_msg_example");
	strcpy(outDesc, "More information https://github.com/sharedflight/sf-inter-plugin-msgs");

    // You probably want this on
	XPLMEnableFeature("XPLM_USE_NATIVE_PATHS", 1);

	return 1;
}

PLUGIN_API void	XPluginStop(void) {
}

PLUGIN_API void XPluginDisable(void) {
	XPLMUnregisterFlightLoopCallback(flightLoopCallback, NULL);	 //  Don't forget to unload this callback.
	sfPluginID = XPLM_NO_PLUGIN_ID;
}

PLUGIN_API int XPluginEnable(void) {
	sfPluginID = XPLM_NO_PLUGIN_ID;
	XPLMRegisterFlightLoopCallback(flightLoopCallback, 1, NULL); // This will get called during each flight loop and we will handle network events
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID, intptr_t inMessage, void * inParam) {

}

float flightLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon)
{
	if (sfPluginID == XPLM_NO_PLUGIN_ID) {
		sfPluginID = XPLMFindPluginBySignature(XPLM_SHARED_FLIGHT_PLUGIN_SIG);
	
		if (sfPluginID == XPLM_NO_PLUGIN_ID) {
			XPLMDebugString("[Shared Flight Messaging Example plugin] Shared Flight plugin not found. Will not make further flight loop callbacks.");
			return 0;
		}

		XPLMSendMessageToPlugin(sfPluginID, MSG_CMD_GET_SHARED_FLIGHT_INTERFACE, &sfPluginInterface);
	}

	SF_MSG_DATA msg;
	msg.mSize = strlen("Foo");
	msg.mData = strdup("Foo");  

	XPLMSendMessageToPlugin(sfPluginID, MSG_CMD_SF_TRANSMIT_AIRCRAFT_DATA, &msg);

	//Return the interval we next want to be called in..
	return 2.0f;
}
