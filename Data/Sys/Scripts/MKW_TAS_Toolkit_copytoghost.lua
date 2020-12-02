local input_runner = require("mkw_input_reader_runner")
local input_mask = require("input_mask")


function maskInput(x, y)
	local index = x * 15 + y + 1
	return input_mask[index][2]
end

function onScriptStart()
	MsgBox("Script started.")

	ghost_file = io.open("mkw_input_reader_ghost.lua", "w")
	runner_file = io.open("mkw_input_reader_runner.lua", "w")
	
	io.output(ghost_file)
	
	io.write("local mkw_input_reader_ghost = {")
	io.write("\n -- {A, B, L, horiz, vert, dpad}")
	io.write("\n -- dpad values: 1 = up, 2 = down, 3 = left, 4 = right")
	
	io.output(runner_file)
	
	io.write("local mkw_input_reader_runner = {")
	io.write("\n -- {A, B, L, horiz, vert, dpad}")
	io.write("\n -- dpad values: 1 = up, 2 = down, 3 = left, 4 = right")
	
	for currentFrame, inputs in ipairs(input_runner) do
		local stickInputs = maskInput(inputs[4], inputs[5])
		local outputString = string.format("\n{%u, %u, %u, %u, %u, %u}, -- frame %u", inputs[1], inputs[2], inputs[3], stickInputs[1], stickInputs[2], inputs[6], currentFrame)
		
		io.output(ghost_file)
		io.write(outputString)
		
		io.output(runner_file)
		io.write(outputString)
	end
	
	CancelScript()
end

function onScriptCancel()
	MsgBox("Script ended.")
	io.output(ghost_file)
	io.write("\n}\nreturn mkw_input_reader_ghost")
	
	io.output(runner_file)
	io.write("\n}\nreturn mkw_input_reader_runner")
end