--Overwrites ghosts using the output of the mkw_input_writer
--File needs to be named "mkw_input_reader_ghost.lua"

local core = require "MKW_core"
local ghost_core = require "MKW_ghost_core"
local input_file = require "mkw_input_reader_ghost"

write_file = io.open("mkw_input_reader_ghost_raw.rrkg", "wb")
io.output(write_file)

function maskFaceButton(aButton, bButton, lButton, prevMask)
	local x8Mask = 0x0
	-- if bButton == 1 and (prevMask == 0x1 or prevMask == 0x5 or prevMask == 0xB or prevMask == 0xF) then
	if aButton == 1 and bButton == 1 and not(prevMask == 0x2 or prevMask == 0x3 or prevMask == 0x7) then
		x8Mask = 0x8
	end
	return aButton * 0x1 + bButton * 0x2 + lButton * 0x4 + x8Mask
end

function maskDirectionInput(horizonalInput, verticalInput)
	return (horizonalInput << 4) + verticalInput
end

function maskTrickInput(trickInput)
	return trickInput << 4
end

function string.fromhex(str)
    return (str:gsub('..', function (cc)
        return string.char(tonumber(cc, 16))
    end))
end

function onScriptStart()
	MsgBox("Script started.")

	local data = {}
	local dataIndex = 1
	
	local addressFaceButton, addressDirectionInput, addressTrickInput = ghost_core.getGhostAddresses()
	local addressFaceButtonPointer, addressDirectionInputPointer, addressTrickInputPointer = ghost_core.getGhostAddressPointer()
	
	MsgBox(string.format("%X, %X, %X", addressFaceButton, addressDirectionInput, addressTrickInput))
	
	local prevInput = maskFaceButton(input_file[1][1], input_file[1][2], input_file[1][3], 0x0)
	local amountCurrentFrames = 0x0
	
	for _, inputs in ipairs(input_file) do
		local currentInput = maskFaceButton(inputs[1], inputs[2], inputs[3], prevInput)
		if prevInput ~= currentInput then
			data[dataIndex] = prevInput
			dataIndex = dataIndex + 1
			
			data[dataIndex] = amountCurrentFrames
			dataIndex = dataIndex + 1
			
			prevInput = currentInput
			amountCurrentFrames = 0x1
		else
			if amountCurrentFrames >= 0xFF then
				data[dataIndex] = prevInput
				dataIndex = dataIndex + 1
				
				data[dataIndex] = amountCurrentFrames
				dataIndex = dataIndex + 1
				
				prevInput = currentInput
				amountCurrentFrames = 0x1
			else
				amountCurrentFrames = amountCurrentFrames + 1
			end
		end
	end
	
	data[dataIndex] = prevInput
	dataIndex = dataIndex + 1
	
	data[dataIndex] = amountCurrentFrames
	dataIndex = dataIndex + 1
	
	prevInput = maskDirectionInput(input_file[1][4], input_file[1][5])
	amountCurrentFrames = 0x0
	
	for _, inputs in ipairs(input_file) do
		local currentInput = maskDirectionInput(inputs[4], inputs[5])
		if prevInput ~= currentInput then
			data[dataIndex] = prevInput
			dataIndex = dataIndex + 1
			
			data[dataIndex] = amountCurrentFrames
			dataIndex = dataIndex + 1
			
			prevInput = currentInput
			amountCurrentFrames = 0x1
		else
			if amountCurrentFrames >= 0xFF then
				data[dataIndex] = prevInput
				dataIndex = dataIndex + 1
				
				data[dataIndex] = amountCurrentFrames
				dataIndex = dataIndex + 1
				
				prevInput = currentInput
				amountCurrentFrames = 0x1
			else
				amountCurrentFrames = amountCurrentFrames + 1
			end
		end
	end
	
	data[dataIndex] = prevInput
	dataIndex = dataIndex + 1
	
	data[dataIndex] = amountCurrentFrames
	dataIndex = dataIndex + 1
	
	prevInput = maskTrickInput(input_file[1][6])
	amountCurrentFrames = 0x0
	
	for _, inputs in ipairs(input_file) do
		local currentInput = maskTrickInput(inputs[6])
		if prevInput ~= currentInput then
			local inputData = prevInput + math.floor(amountCurrentFrames / 0x100)
			data[dataIndex] = inputData
			dataIndex = dataIndex + 1
			
			data[dataIndex] = amountCurrentFrames % 0x100
			dataIndex = dataIndex + 1
			
			prevInput = currentInput
			amountCurrentFrames = 0x1
		else
			if amountCurrentFrames >= 0xFFF then
				local inputData = prevInput + math.floor(amountCurrentFrames / 0x100)
				data[dataIndex] = inputData
				dataIndex = dataIndex + 1
				
				data[dataIndex] = amountCurrentFrames % 0x100
				dataIndex = dataIndex + 1
				
				prevInput = currentInput
				amountCurrentFrames = 0x1
			else
				amountCurrentFrames = amountCurrentFrames + 1
			end
		end
	end
	
	local inputData = prevInput + math.floor(amountCurrentFrames / 0x100)
	data[dataIndex] = inputData
	dataIndex = dataIndex + 1
	
	data[dataIndex] = amountCurrentFrames % 0x100
	dataIndex = dataIndex + 1
	
	for i, currentData in ipairs(data) do
		local byteData = string.format("%02X", currentData):fromhex()
		io.write(byteData)
		--if i % 16 == 0 then io.write("\n") end
	end
	
	CancelScript()
end

function onScriptCancel()
	MsgBox("Script ended.")
end