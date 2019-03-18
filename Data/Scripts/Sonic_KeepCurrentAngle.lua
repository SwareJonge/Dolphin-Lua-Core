local core = require "sonic_core"

function onScriptStart()
	
end

function onScriptCancel()
	
end

function onScriptUpdate()
	
	-- input the current Y rotation angle to the main stick
	core.angleInput(core.getRot().Y)
	
end

function onStateLoaded()
	
end

function onStateSaved()
	
end