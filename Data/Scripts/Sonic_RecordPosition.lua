file = io.open("sonic_position.lua","w")
io.output(file)

local core = require "sonic_core"

function onScriptStart()

	-- get current position and write it to sonic_position.lua
	io.write("local position = {\n")
	io.write(string.format("    X = %f,\n", core.getPos().X))
	io.write(string.format("    Y = %f,\n", core.getPos().Y))
	io.write(string.format("    Z = %f\n", core.getPos().Z))
	io.write("}\n\nreturn position")
	
	MsgBox(string.format("Recorded position: %f, %f, %f", core.getPos().X, core.getPos().Y, core.getPos().Z))
	
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