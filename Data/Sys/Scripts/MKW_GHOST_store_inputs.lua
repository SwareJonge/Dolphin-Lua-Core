-- stores the ghost as a file with the sequence of inputs

-- IMPORTANT: you can only run this, if the track and ghost have been fully loaded
-- after that you can run the script any time you want, before the first inputs can be made or even midrace


package.path = GetScriptsDir() .. "/MKW/MKW_Core.lua"
local core = require("MKW_Core")
package.path = GetScriptsDir() .. "/MKW/MKW_ghost_core.lua"
local ghost_core = require("MKW_ghost_core") 

write_file = io.open("mkw_input_reader_ghost.lua", "w")
io.output(write_file)

function onScriptStart()
	MsgBox("Script started.")

	local input_list = {}
	local input_index = 1

	local addressFaceButton, addressDirectionInput, addressTrickInput = ghost_core.getGhostAddresses()


	for currentAddress = addressFaceButton, addressDirectionInput - 2, 2 do
		local dataByte = ReadValue8(currentAddress)
		local lengthByte = ReadValue8(currentAddress + 0x1)
		for dataIndex = 1, lengthByte do
			input_list[input_index] = {}
			input_list[input_index][1] = dataByte % 0x2
			input_list[input_index][2] = (dataByte >> 1) % 0x2
			input_list[input_index][3] = (dataByte >> 2) % 0x2
			input_index = input_index + 1
		end
	end

	local frameCount = input_index - 1
	input_index = 1

	for currentAddress = addressDirectionInput, addressTrickInput - 2, 2 do
		local dataByte = ReadValue8(currentAddress)
		local lengthByte = ReadValue8(currentAddress + 0x1)
		for dataIndex = 1, lengthByte do
			input_list[input_index][4] = dataByte >> 4
			input_list[input_index][5] = dataByte % 0x10
			input_index = input_index + 1
		end
	end

	input_index = 1

	local currentAddress = addressTrickInput
	local rawData = ReadValue16(currentAddress)

	repeat
		local dataByte = rawData >> 12
		local lengthByte = rawData % 0x1000

		for dataIndex = 1, lengthByte do
			input_list[input_index][6] = dataByte
			input_index = input_index + 1
		end

		currentAddress = currentAddress + 0x2
		rawData = ReadValue16(currentAddress)
	until frameCount < input_index

	io.write("local mkw_input_reader_ghost = {")
	io.write("\n -- {A, B, L, vert, horiz, dpad}")
	io.write("\n -- dpad values: 1 = up, 2 = down, 3 = left, 4 = right")

	for currentFrame, inputs in ipairs(input_list) do
		local outputString = string.format("\n{%u, %u, %u, %u, %u, %u}, -- frame %u", inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6], currentFrame)
		io.write(outputString)
	end

	CancelScript()
end

function onScriptCancel()
	MsgBox("Script ended.")
	io.write("\n}\nreturn mkw_input_reader_ghost")
end
