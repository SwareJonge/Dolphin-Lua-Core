local core = require "sa2b_core"

function onScriptStart()
	
end

function onScriptCancel()
	
end

function onScriptUpdate()
	
	-- input current Y rotation angle + 4000 units to the main stick
	core.angleInput((core.getYRot() + 4000) % 65536)
	
end

function onStateLoaded()
	
end

function onStateSaved()
	
end