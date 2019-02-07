file = io.open("sadx_position.lua","w")
io.output(file)

local core = require "sadx_core"

function onScriptStart()

	-- get current position and write it to sa2b_position.lua
	io.write("local position = {\n")
	io.write(string.format("    X = %f,\n", core.getXPos()))
	io.write(string.format("    Y = %f,\n", core.getYPos()))
	io.write(string.format("    Z = %f\n", core.getZPos()))
	io.write("}\n\nreturn position")
	
	MsgBox(string.format("Recorded position: %f, %f, %f", core.getXPos(), core.getYPos(), core.getZPos()))
	
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