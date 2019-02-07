local core = require "sadx_core"

function onScriptStart()
	
end

function onScriptCancel()
	
end

function onScriptUpdate()
	
	-- input current Y rotation angle + smoothTurnIncrement units to the main stick
	core.angleInput((core.getYRot() + core.smoothTurnIncrement) % 65536)
	
end

function onStateLoaded()
	
end

function onStateSaved()
	
end