//
//  SharedFlightMessaging.h
//
//  Created by Justin Snapp on 1/1/24.
//

#ifndef SharedFlightMessaging_h
#define SharedFlightMessaging_h

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
    const char * mData ;
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
#define MSG_CMD_ACF_SEND_AUTOSAVE_STATE 		0x08534600

// Sent by aircraft to Shared Flight with the response to
// request for "autosave" type data for initial join.  This
// data will be relayed by Shared Flight over the net to the
// newly joining flight.
#define MSG_CMD_SF_RCV_AUTOSAVE_STATE 		0x08534601

#define MSG_CMD_SF_RCV_AUTOSAVE_STATE_SASL 	0x08534602


#define MSG_CMD_ACF_LOAD_AUTOSAVE_STATE		0x08534603

#define MSG_CMD_ACF_OAD_AUTOSAVE_STATE_SASL	0x08534604


#define MSG_MSG_SF_AUTOSAVE_STATE_DONE		0x88534600


#define MSG_CMD_SF_RCV_AIRCRAFT_DATA 		0x08534605

#define MSG_CMD_SF_RCV_AIRCRAFT_DATA_SASL 	0x08534606


#define MSG_CMD_ACF_RCV_AIRCRAFT_DATA		0x08534607

#define MSG_CMD_ACF_RCV_AIRCRAFT_DATA_SASL	0x08534608


typedef void (__stdcall *SharedFlightReceiveAutosaveState)(const void *data, unsigned int size);

typedef void (__stdcall *SharedFlightAutosaveCallback)(const void *data, unsigned int size);
typedef void (__stdcall *SharedFlightSetAutosaveLoadCallback)(SharedFlightAutosaveCallback cb);

typedef void (__stdcall *SharedFlightAutosaveStateApplyDone)();

typedef void (__stdcall *SharedFlightReceiveAircraftData)(const void *data, unsigned int size);

typedef void (__stdcall *SharedFlightReceiveAircraftData)(const void *data, unsigned int size);

typedef void (__stdcall *SharedFlightAircraftDataCallback)(const void *data, unsigned int size);
typedef void (__stdcall *SharedFlightSetAircraftDataCallback)(SharedFlightAircraftDataCallback cb);


#define MSG_CMD_GET_SHARED_FLIGHT_INTERFACE	0x085346FF 			 // for XPLMSendMessageToPlugin, use pointer to SharedValuesInterface as parameter to fill in
//--------------------------------------------------------------------------

typedef struct
{
	SharedFlightReceiveAutosaveState ReceiveAutosaveState;
	SharedFlightSetAutosaveLoadCallback SetAutoSaveCallback; // register an update callback called at each frame in sync with platform and aircraft values update, for using values functions below
	SharedFlightAutosaveStateApplyDone AutosaveStateApplyDone; // remove a registred update callback	
	SharedFlightReceiveAircraftData ReceiveAircraftData;
	SharedFlightAircraftDataCallback SetAircraftDataCallback; // register an update callback called at each frame in sync with platform and aircraft values update, for using values functions below
} SharedFlightInterPluginInterface;


/*
	SharedDataVersionProc DataVersion; // to get actual dataset version
	SharedDataAddUpdateProc DataAddUpdate; // register an update callback called at each frame in sync with platform and aircraft values update, for using values functions below
	SharedDataDelUpdateProc DataDelUpdate; // remove a registred update callback
	SharedValuesCountProc ValuesCount; // get count off all values (including deleted)
	SharedValueIdByIndexProc ValueIdByIndex; // get value id by it's index (0 up to ValuesCount), or -1 if not exists or removed
	SharedValueIdByNameProc ValueIdByName; // get value id by it's name, or -1 if not exists or removed
	SharedValueNameProc ValueName; // get value name
	SharedValueDescProc ValueDesc; // get value description
	SharedValueTypeProc ValueType; // get value type, one of Value_Type_
	SharedValueFlagsProc ValueFlags; // get value flags, OR Value_Flag_
	SharedValueUnitsProc ValueUnits; // get value units, OR Value_Unit_
	SharedValueParentProc ValueParent; // get id of the parent object value
	SharedValueSetProc ValueSet; // set value
	SharedValueGetProc ValueGet; // get value
	SharedValueGetSizeProc ValueGetSize; // get actual size of the value data (for strings)
	SharedValueObjectLoadStateProc ValueObjectLoadState; // deserialize object state
	SharedValueObjectSaveStateProc ValueObjectSaveState; // serialize object state
	SharedValueObjectNewValueProc ValueObjectNewValue; // add a new value

} SharedValuesInterface;
*/


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