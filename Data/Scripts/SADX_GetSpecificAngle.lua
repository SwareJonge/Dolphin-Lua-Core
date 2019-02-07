angleSetPoint = 0

local core = require "sadx_core"
local target_angle = require "sadx_target_angle"

function onScriptStart()

	-- set the angleSetPoint as the value read in sa2b_target_angle.lua file
	angleSetPoint = target_angle.Y
	MsgBox(string.format("Target angle: %d", angleSetPoint))
	
end

function onScriptCancel()
	
end

function onScriptUpdate()
	
	-- input angleSetPoint to the main stick
	core.angleInput(angleSetPoint)
	
end

function onStateLoaded()
	
end

function onStateSaved()
	
end