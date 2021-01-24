----- GLOBAL VARIABLES -----
package.path = GetScriptsDir() .. "/MKW/MKW_Core.lua"
local core = require("MKW_Core")

package.path = GetScriptsDir() .. "/MKW/MKW_Pointers.lua"
local Pointers = require("MKW_Pointers")
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
	text = text .. "\n\n===== Speed ====="
	text = text .. string.format("\nX: %12.6f | Y: %12.6f | Z: %12.6f \nXZ: %11.6f | XYZ: %10.6f", core.getSpd().X, core.getSpd().Y, core.getSpd().Z, core.getSpd().XZ, core.getSpd().XYZ)
	text = text .. "\n\n===== Rotation (Facing|Moving) ====="
	text = text .. string.format("\nX: %6.2f | %6.2f \nY: %6.2f | %6.2f \nZ: %6.2f | %6.2f", core.calculateEuler().X, core.calculateDirectX(), core.calculateEuler().Y, core.calculateDirectY(), core.calculateEuler().Z, core.calculateDirectZ())
if core.isSinglePlayer() == false then
	text = text .. "\n\n===== Time Difference ====="
	text = text .. string.format("\nCurrent: %11.6f \nFinish: %11.6f", core.getDifference(), core.getFinishDifference())
end
	SetScreenText(text)
end

function onStateLoaded()

end

function onStateSaved()

end
