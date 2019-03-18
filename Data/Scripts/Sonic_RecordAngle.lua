file = io.open("sonic_target_angle.lua","w")
io.output(file)

local core = require "sonic_core"

function onScriptStart()

	-- get current Y rotation angle and write it to sonic_target_angle.lua file
	io.write("local target_angle = {\n")
	io.write(string.format("    Y = %d\n", core.getRot().Y))
	io.write("}\n\nreturn target_angle")
	
	MsgBox(string.format("Recorded target angle: %d", core.getRot().Y))
	
	CancelScript()
	
end

function onScriptCancel()
	
end

function onScriptUpdate()

end

function onStateLoaded()
	
end

function onStateSaved()
	
end