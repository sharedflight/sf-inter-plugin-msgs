//
//  SharedFlightMessaging.h
//
//  Created by Justin Snapp on 1/1/24.
//

#ifndef SharedFlightMessaging_h
#define SharedFlightMessaging_h

// #ifdef __cplusplus
// 	#if APL
//         #if __GNUC__ >= 4
//             #define SF_MSGING_API extern "C" __attribute__((visibility("default")))
//         #elif __MACH__
// 			#define SF_MSGING_API extern "C"
// 		#else		
// 			#define SF_MSGING_API extern "C" __declspec(dllexport)
// 		#endif
// 	#elif IBM
// 		#define SF_MSGING_API extern "C" __declspec(dllexport)
// 	#elif LIN
// 		#if __GNUC__ >= 4
// 			#define SF_MSGING_API extern "C" __attribute__((visibility("default")))
// 		#else
// 			#define SF_MSGING_API extern "C"
// 		#endif
// 	#else
// 		#error "Platform not defined!"
// 	#endif
// #else
// 	#if APL
//         #if __GNUC__ >= 4
//             #define SF_MSGING_API __attribute__((visibility("default")))
//         #elif __MACH__
// 			#define SF_MSGING_API 
// 		#else
// 			#define SF_MSGING_API __declspec(dllexport)
// 		#endif		
// 	#elif IBM
// 		#define SF_MSGING_API __declspec(dllexport)
// 	#elif LIN
//         #if __GNUC__ >= 4
//             #define SF_MSGING_API __attribute__((visibility("default")))
// 		#else
// 			#define SF_MSGING_API
// 		#endif		
// 	#else
// 		#error "Platform not defined!"
// 	#endif
// #endif

/*

By convention, plugin-defined notifications should have the high bit set 
(e.g. be greater or equal to unsigned 0x8000000) while commands should 
have this bit be cleared.

Plugin messages are defined as 32-bit integers. Messages below 0x00FFFFFF 
are reserved for X-Plane and the plugin SDK.

*/


// S - 83 in ascii =  , F - 70 in ascii
// 0b01010011 0b01000110 => 0b0101001101000110 = 0x5346

// So notifications will be 0x88534600+ and 
// commands will be 0x08534600+



struct SASL_MSG_IntArrayData {
    size_t mSize ;
    int * mData ;
};
struct SASL_MSG_FloatArrayData {
    size_t mSize ;
    float * mData ;
};
struct SASL_MSG_StringData {
    size_t mSize ;
    const char * mData;
};


/*
    int sfd_msg_rcv_val = SF_MSG_RCV;
    
    if (inMessage == sfd_msg_rcv_val) {
        
        SASL_MSG_StringData messageData = *((SASL_MSG_StringData *)inParam);
        
		SFFlightClientController::getInstance().handle_message_from_ui(messageData.mData);
    }*/



#define XPLM_SHARED_FLIGHT_PLUGIN_SIG "justinsnapp.SharedFlight" // for XPLMFindPluginBySignature


// Shared Flight requests local "autosave" state (command)
// Zibomod plugin sends this autosave data to Shared Flight plugin (notification from zibo to Shared Flight)
// Shared Flight sends this info to newly joining participant over Shared Flight networking
// Shared Flight sends received autosave state data to Zibo plugin (notification to zibo from Shared Flight)
// Zibomod receives this notification and autosave data an applies it.

// Sent by Shared Flight to aircraft to request the "autosave"
// type data that will be sent to newly joining instances
// of the aircraft within a Shared Flight.  Any aircraft that has
// autosave should be capable of producing this data.  
// Shared Flight will send this request only to one connected 
// instance on the flight.
#define MSG_CMD_ACF_REQST_AUTOSAVE_STATE 		0x08534600

// Sent by aircraft to Shared Flight with the response to
// request for "autosave" type data for initial join.  This
// data will be relayed by Shared Flight over the net to the
// newly joining flight.
#define MSG_CMD_SF_TRANSMIT_AUTOSAVE_STATE 		0x08534601

#define MSG_CMD_SF_TRANSMIT_AUTOSAVE_STATE_SASL 0x08534602


// Sent by Shared Flight to the aircraft with the "autosave"
// type data that has been sent by existing system reference
// to the newly joining instance of the aircraft within 
// Shared Flight.  Any aircraft that has autosave should be
// capable of processing this data.
#define MSG_CMD_ACF_LOAD_AUTOSAVE_STATE			0x08534603

#define MSG_CMD_ACF_LOAD_AUTOSAVE_STATE_SASL	0x08534604

// Sent by aircraft to Shared Flight to notify that the aircraft
// has successfully processed the data, in case it takes a long
// while to get all the custom systems/state/flight model all
// processed.
#define MSG_MSG_SF_AUTOSAVE_STATE_DONE			0x88534600


// Sent by aircraft to Shared Flight when there is data that 
// should be sent to all the other instances of the aircraft
// connected within the Shared Flight. Logic of who is system
// master, pilot flying, crew, passenger is the responsibility
// of the aircraft to handle with this data.
// (NOTE: If thats a problem, let us know, we can add more
// message types with your own required logic.)
#define MSG_CMD_SF_TRANSMIT_AIRCRAFT_DATA 		0x08534605

#define MSG_CMD_SF_TRANSMIT_AIRCRAFT_DATA_SASL 	0x08534606


// Sent by Shared Flight to aircraf with the newly transmitted
// data from other instance of the aircraft.
#define MSG_CMD_ACF_NEW_AIRCRAFT_DATA			0x08534607

#define MSG_CMD_ACF_NEW_AIRCRAFT_DATA_SASL		0x08534608


typedef void (__cdecl *SharedFlightAutosaveRequestCallback)();
typedef void (__cdecl *SharedFlightAutosaveLoadCallback)(const void *data, unsigned int size);
typedef void (__cdecl *SharedFlightAircraftDataCallback)(const void *data, unsigned int size);

typedef void (__cdecl *SharedFlightSetAutosaveRequestCallback)(SharedFlightAutosaveRequestCallback cb);
typedef void (__cdecl *SharedFlightTransmitAutosaveState)(const void *data, unsigned int size);
typedef void (__cdecl *SharedFlightSetAutosaveLoadCallback)(SharedFlightAutosaveLoadCallback cb);
typedef void (__cdecl *SharedFlightAutosaveStateApplyDone)();
typedef void (__cdecl *SharedFlightTransmitAircraftData)(const void *data, unsigned int size);
typedef void (__cdecl *SharedFlightSetAircraftDataCallback)(SharedFlightAircraftDataCallback cb);


// for XPLMSendMessageToPlugin, use pointer to SharedFlightInterPluginInterface as parameter to fill in
#define MSG_CMD_GET_SHARED_FLIGHT_INTERFACE	0x085346FF 			 

typedef struct
{
	SharedFlightSetAutosaveRequestCallback SetAutosaveRequestCallback;
	SharedFlightTransmitAutosaveState TransmitAutosaveState;
	SharedFlightSetAutosaveLoadCallback SetAutosaveLoadCallback;
	SharedFlightAutosaveStateApplyDone AutosaveStateApplyDone;	
	SharedFlightTransmitAircraftData TransmitAircraftData;
	SharedFlightSetAircraftDataCallback SetAircraftDataCallback;
} SharedFlightInterPluginInterface;



// function sfdMessageCallback ( id , messageID, data_table)
// 	--print( " Message received ! " );

// 	if id == SFD_C_PLUGIN_ID and messageID == someMessageID then 
		
// 		mainWindowController:handleMessage(data_table);
// 		--print ( "And the message is the type we expected." );
// 		--print(data_table);
// 	end 
// end

// sasl.registerMessageHandler ( someMessageID , TYPE_STRING , sfdMessageCallback )




#endif