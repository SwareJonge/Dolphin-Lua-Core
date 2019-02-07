file = io.open("sadx_target_angle.lua","w")
io.output(file)

local core = require "sadx_core"

function onScriptStart()

	-- get current Y rotation angle and write it to sa2b_target_angle.lua file
	io.write("local target_angle = {\n")
	io.write(string.format("    Y = %d\n", core.getYRot()))
	io.write("}\n\nreturn target_angle")
	
	MsgBox(string.format("Recorded target angle: %d", core.getYRot()))
	
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