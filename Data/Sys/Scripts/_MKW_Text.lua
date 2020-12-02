----- GLOBAL VARIABLES -----
local core = require "MKW_core"
--Add an underscore (_) to the beginning of the filename if you want the script to auto launch once you start a game!


function onScriptStart()
	if GetGameID() ~= "RMCP01" and GetGameID() ~= "RMCJ01" and GetGameID() ~= "RMCE01" and GetGameID() ~= "RMCK01" then
		SetScreenText("")
		CancelScript()
	end
end

function onScriptCancel()
	SetScreenText("")
end

function onScriptUpdate()
  local text = ""
	text = text .. string.format("\nFrame: %d\n", core.getFrameOfInput())
	text = text .. "\n===== Speed ====="
	text = text .. string.format("\nY: %12.6f \nXZ: %11.6f \nXYZ: %10.6f", core.getSpd().Y, core.getSpd().XZ, core.getSpd().XYZ)
	text = text .. "\n\n===== Rotation ====="
	text = text .. string.format("\nX: %6.2f \nY: %6.2f \nZ: %6.2f", core.calculateEuler().X, core.calculateEuler().Y, core.calculateEuler().Z)
	SetScreenText(text)
end

function onStateLoaded()

end

function onStateSaved()

end
