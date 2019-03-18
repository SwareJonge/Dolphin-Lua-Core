local core = require "sonic_core"

function onScriptStart()
	
end

function onScriptCancel()
	
end

function onScriptUpdate()
	
	-- input current Y rotation angle + smoothTurnAngle units to the main stick
	core.angleInput((core.getYRot() + core.smoothTurnAngle) % 65536)
	
end

function onStateLoaded()
	
end

function onStateSaved()
	
end