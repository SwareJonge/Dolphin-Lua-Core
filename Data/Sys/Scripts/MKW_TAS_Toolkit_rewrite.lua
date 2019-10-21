

function onScriptStart()
	MsgBox("Script started.")
	local input_runner = require("mkw_input_reader_runner")
	
	write_file = io.open("mkw_input_reader_runner.lua", "w")
	io.output(write_file)
	
	io.write("local mkw_input_reader_runner = {")
	io.write("\n -- {A, B, L, vert, horiz, dpad}")
	io.write("\n -- dpad values: 1 = up, 2 = down, 3 = left, 4 = right")
	
	for currentFrame, inputs in ipairs(input_runner) do
		local outputString = string.format("\n{%u, %u, %u, %u, %u, %u}, -- frame %u", inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], currentFrame)
		io.write(outputString)
	end
	
	CancelScript()
end

function onScriptCancel()
	MsgBox("Script ended.")
	io.write("\n}\nreturn mkw_input_reader_runner")
end