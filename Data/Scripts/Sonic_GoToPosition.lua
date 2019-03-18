targetX = 0
targetY = 0
targetZ = 0

local core = require "sonic_core"
local position = require "sonic_position"

function onScriptStart()

	-- set the target position as the variables read in sonic_position.lua file
	targetX = position.X
	targetY = position.Y
	targetZ = position.Z
	
	MsgBox(string.format("Target position: %f, %f, %f", targetX, targetY, targetZ))
	
end

function onScriptCancel()
	
end

function onScriptUpdate()
	
	-- adds the angle to target to the current Y rotation and inputs it to the main stick
	core.angleInput( (core.getRot().Y + core.angleToPosition(targetX, targetY, targetZ)) % 65536 )
	
end

function onStateLoaded()
	
end

function onStateSaved()
	
end
