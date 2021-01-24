-- stores the inputs made with TAS_Input/Controller into the mkw_input_reader_runner.lua file

-- keeps the previous inputs up to the current frame alive
-- write the file on the cancel button press

package.path = GetScriptsDir() .. "/MKW/MKW_Core.lua"
local core = require("MKW_Core")

local input_list = {}
local prevFrame = core.getFrameOfInput() + 1

local basicInput = {0, 0, 0, 7, 7, 0}

function tableLength(tableInput)
	local count = 0
	for _ in pairs(tableInput) do count = count + 1 end
	return count
end

function deleteOverloadedInputs()
	local currentFrame = core.getFrameOfInput() + 1

	while tableLength(input_list) > currentFrame do
		table.remove(input_list)
	end
end

function onScriptStart()
	MsgBox("Script started.")
	local runner_exists, input_runner = pcall(require, "mkw_input_reader_runner")

	if runner_exists then
		input_list = input_runner
	end
end

function onScriptCancel()
	MsgBox("Script ended.")

	deleteOverloadedInputs()

	write_file = io.open("mkw_input_reader_runner.lua", "w")
	io.output(write_file)

	io.write("local mkw_input_reader_runner = {")
	io.write("\n -- {A, B, L, horiz, vert, dpad}")
	io.write("\n -- dpad values: 1 = up, 2 = down, 3 = left, 4 = right")

	for currentFrame, inputs in ipairs(input_list) do
		local outputString = string.format("\n{%u, %u, %u, %u, %u, %u}, -- frame %u", inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], currentFrame)
		io.write(outputString)
	end

	io.write("\n}\nreturn mkw_input_reader_runner")
end

function storeInputs(currentFrame)
	local ingameFrame = currentFrame - 1
	if ingameFrame == 0 then return nil end

	local buttons = core.getInput()

	local aButton = buttons.ABLR % 2
	local bButton = (buttons.ABLR >> 1) % 2
	local lButton = (buttons.ABLR >> 2) % 2

	input_list[ingameFrame] = {aButton, bButton, lButton, buttons.X, buttons.Y, buttons.DPAD}

	prevFrame = currentFrame
end

function onScriptUpdate()
	local currentFrame = core.getFrameOfInput() + 1
	if(currentFrame ~= prevFrame) then
		while tableLength(input_list) < currentFrame do
			table.insert(input_list, basicInput)
		end
		storeInputs(currentFrame)
	end
end

function onStateLoaded()

end

function onStateSaved()

end
