----- GLOBAL VARIABLES -----
local core = require "SMG_core"
--Add an underscore (_) to the beginning of the filename if you want the script to auto launch once you start a game!


function onScriptStart()
end

function onScriptCancel()
	MsgBox("Bye :(") SetScreenText("")
end

function onScriptUpdate()
	local text = "\n\n\n\n\n\n\n\n\n\n===== Speed ====="
	text = text .. string.format("\nY: %10.6f | XZ: %10.6f | XYZ: %10.6f", core.getSpd().Y, core.getSpd().XZ, core.getSpd().XYZ)

	text = text .. "\n"


	text = text .. "\n\n===== Position ====="
	text = text .. string.format("\nX: %12.6f \nY: %12.6f \nZ: %12.6f", core.getPos().X, core.getPos().Y, core.getPos().Z)

	SetScreenText(text)
end

function onStateLoaded()

end

function onStateSaved()

end
