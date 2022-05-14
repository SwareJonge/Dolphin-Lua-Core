----- GLOBAL VARIABLES -----
package.path = GetScriptsDir() .. "/MKW/MKW_Core.lua"
local core = require("MKW_Core")

package.path = GetScriptsDir() .. "/MKW/MKW_Pointers.lua"
local Pointers = require("MKW_Pointers")
--Add an underscore (_) to the beginning of the filename if you want the script to auto launch once you start a game!

local xRotPrev = 0
local yRotPrev = 0
local zRotPrev = 0
local xRot = 0
local yRot = 0
local zRot = 0
local xRotSpeed = 0
local yRotSpeed = 0
local zRotSpeed = 0
local curFrame = 0

function onScriptStart()
	if GetGameID() ~= "RMCP01" and GetGameID() ~= "RMCJ01" and GetGameID() ~= "RMCE01" and GetGameID() ~= "RMCK01" then
		SetScreenText("")
		CancelScript()
	end
	
	xRotPrev = core.calculateEuler().X
	yRotPrev = core.calculateEuler().Y
	zRotPrev = core.calculateEuler().Z
	
	curFrame = GetFrameCount()
end

function onScriptCancel()
	SetScreenText("")
end

function onScriptUpdate()
	if GetFrameCount() ~= curFrame then
		xRot = core.calculateEuler().X
		yRot = core.calculateEuler().Y
		zRot = core.calculateEuler().Z
		xRotSpeed = xRot - xRotPrev
		yRotSpeed = yRot - yRotPrev
		zRotSpeed = zRot - zRotPrev
		xRotPrev = xRot
		yRotPrev = yRot
		zRotPrev = zRot
		curFrame = GetFrameCount()
	end
		
		local text = ""
			text = text .. string.format("\nFrame: %d", core.getFrameOfInput())
			text = text .. "\n\n===== Speed ====="
			text = text .. string.format("\nX: %8.4f | Y: %8.4f | Z: %8.4f \nXZ: %12.8f | XYZ: %12.8f", core.getSpd().X, core.getSpd().Y, core.getSpd().Z, core.getSpd().XZ, core.getSpd().XYZ)
			if core.isSinglePlayer() == false then
				text = text .. "\n\n===== Time Difference ====="
				text = text .. string.format("\nFacing: %11.6f (%s) \nMoving: %11.6f (%s) \nZ (Finish): %11.6f (%s) \nX (Perpend): %11.6f (%s)", core.getRotDifference().D, core.isAhead().RD, core.getDirectDifference().D, core.isAhead().DD, core.getFinishDifference().D, core.isAhead().FD, core.getOtherDifference().D, core.isAhead().OD)		
			end
			text = text .. "\n\n===== Rotation ====="
			text = text .. string.format("\nFacing X (Pitch): %8.4f \nFacing Y (Yaw): %8.4f \nMoving Y (Yaw) %8.4f \nFacing Z (Roll): %8.4f", core.calculateEuler().X, core.calculateEuler().Y, core.calculateDirection().Y, core.calculateEuler().Z)
		SetScreenText(text)
end

function onStateLoaded()

end

function onStateSaved()

end
