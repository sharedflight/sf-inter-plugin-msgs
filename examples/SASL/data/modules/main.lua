

local XP_screenHeight = globalProperty("sim/graphics/view/window_height")
local XP_screenWidth  = globalProperty("sim/graphics/view/window_width")

someMessageID = 0x8034201;
rxMessageID = 0x8034202;
SFD_C_PLUGIN_ID = sasl.findPluginBySignature ( "FlyJSim.SharedFlightDeck" )


include("helpers/SharedFlightMessageDefinitions.lua")


function sendMessageToSharedFlight(msg_type, msg_info)
  if SFD_C_PLUGIN_ID ~= NO_PLUGIN_ID then
    --print("some message ID is " .. tostring(rxMessageID));
    
    messageToSend = {message_type = msg_type, message_info = msg_info};

    sasl.sendMessageToPlugin ( SFD_C_PLUGIN_ID , rxMessageID , TYPE_STRING , json.encode(messageToSend) );
  else
    print("TRYING TO SEND MESSAGE TYPE " .. msg_type .. " TO SFD C PLUGIN BUT CAN'T FIND PLUGIN ID!!!");
  end
end

function update()
  updateAll( components );
end

function draw()
    drawAll ( components )  
end






