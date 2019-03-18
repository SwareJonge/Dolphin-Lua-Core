angleSetPoint = 0

local core = require "sonic_core"

function onScriptStart()

	-- set the angleSetPoint as the starting Y rotation angle
	angleSetPoint = core.getRot().Y
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