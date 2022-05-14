----- GLOBAL VARIABLES -----
package.path = GetScriptsDir() .. "MKW/MKW_core.lua"
local core = require("MKW_core")
package.path = GetScriptsDir() .. "MKW/MKW_Pointers.lua"
local Pointers = require("MKW_Pointers")

local ghost_core = {}

local function getGhostAddressBase()
	local raceData = Pointers.getRaceData2Pointer(0x4)
	local address = ReadValue32(raceData, 0x48, 0x4)
	
	local offsetFaceButton = 0x94
	local addressF1 = GetPointerNormal(address + offsetFaceButton)
	local offsetDirectionInput = 0x98
	local addressD1 = GetPointerNormal(address + offsetDirectionInput)
	local offsetTrickInput = 0x9C
	local addressT1 = GetPointerNormal(address + offsetTrickInput)

	return addressF1, addressD1, addressT1
end
ghost_core.getGhostAddressBase = getGhostAddressBase

local function getGhostAddressPointer()
	local addressF1, addressD1, addressT1 = getGhostAddressBase()

	local offset = 0x4
	return addressF1 + offset, addressD1 + offset, addressT1 + offset
end
ghost_core.getGhostAddressPointer = getGhostAddressPointer

local function getGhostAddressLengthPointer()
	local addressF1, addressD1, addressT1 = getGhostAddressBase()

	local offset = 0xC
	return addressF1 + offset, addressD1 + offset, addressT1 + offset
end
ghost_core.getGhostAddressLengthPointer = getGhostAddressLengthPointer

local function getGhostAddresses()
	local addressF1, addressD1, addressT1 = getGhostAddressPointer()

	local addressF2 = GetPointerNormal(addressF1)
	local addressD2 = GetPointerNormal(addressD1)
	local addressT2 = GetPointerNormal(addressT1)

	return addressF2, addressD2, addressT2
end
ghost_core.getGhostAddresses = getGhostAddresses

local function getInputStorageAddresses()
	local raceData = Pointers.getRaceData2Pointer(0x0)
	
	local addressF = GetPointerNormal(raceData, 0x48, 0xE8, 0x2C)
	local addressD = addressF + 0x276C
	local addressT = addressD + 0x276C
	
	return addressF, addressD, addressT
end
ghost_core.getInputStorageAddresses = getInputStorageAddresses


-- ###############################

local function maskFaceButton(aButton, bButton, lButton, prevMask)
	local x8Mask = 0x0
	if aButton == 1 and bButton == 1 and not(prevMask == 0x2 or prevMask == 0x3 or prevMask == 0x7) then
		x8Mask = 0x8
	end
	return aButton * 0x1 + bButton * 0x2 + lButton * 0x4 + x8Mask
end
ghost_core.maskFaceButton = maskFaceButton

function maskDirectionInput(horizontalInput, verticalInput)
	return (horizontalInput << 4) + verticalInput
end
ghost_core.maskDirectionInput = maskDirectionInput

function maskTrickInput(trickInput)
	return trickInput << 4
end
ghost_core.maskTrickInput = maskTrickInput

-- ###############################

local function writeRKGData(writeAddress, inputData, durationData)
	WriteValue8(writeAddress, inputData)
	WriteValue8(writeAddress + 0x1, durationData)

	return writeAddress + 0x2
end

-- ###############################

function writeInputsIntoRKG(input_ghost)
	local currentAddress = 0x0

	local addressFaceButton, addressDirectionInput, addressTrickInput = getGhostAddresses()
	local addressFaceButtonPointer, addressDirectionInputPointer, addressTrickInputPointer = getGhostAddressPointer()
	local addressFaceButtonLengthPointer, addressDirectionInputLengthPointer, addressTrickInputLengthPointer = getGhostAddressLengthPointer()

	local prevInput = maskFaceButton(input_ghost[1][1], input_ghost[1][2], input_ghost[1][3], 0x0)
	local amountCurrentFrames = 0x0

	currentAddress = addressFaceButton

	for _, inputs in ipairs(input_ghost) do
		local currentInput = maskFaceButton(inputs[1], inputs[2], inputs[3], prevInput)
		if prevInput ~= currentInput then
			currentAddress = writeRKGData(currentAddress, prevInput, amountCurrentFrames)

			prevInput = currentInput
			amountCurrentFrames = 0x1
		else
			if amountCurrentFrames >= 0xFF then
				currentAddress = writeRKGData(currentAddress, prevInput, amountCurrentFrames)

				prevInput = currentInput
				amountCurrentFrames = 0x1
			else
				amountCurrentFrames = amountCurrentFrames + 1
			end
		end
	end

	currentAddress = writeRKGData(currentAddress, prevInput, amountCurrentFrames)
	local faceButtonLength = currentAddress - addressFaceButton

	WriteValue32(addressDirectionInputPointer, currentAddress + 0x80000000)
	WriteValue32(addressFaceButtonLengthPointer, faceButtonLength)

	local newDirectionInput = currentAddress

	prevInput = maskDirectionInput(input_ghost[1][4], input_ghost[1][5])
	amountCurrentFrames = 0x0

	for _, inputs in ipairs(input_ghost) do
		local currentInput = maskDirectionInput(inputs[4], inputs[5])
		if prevInput ~= currentInput then
			currentAddress = writeRKGData(currentAddress, prevInput, amountCurrentFrames)

			prevInput = currentInput
			amountCurrentFrames = 0x1
		else
			if amountCurrentFrames >= 0xFF then
				currentAddress = writeRKGData(currentAddress, prevInput, amountCurrentFrames)

				prevInput = currentInput
				amountCurrentFrames = 0x1
			else
				amountCurrentFrames = amountCurrentFrames + 1
			end
		end
	end

	currentAddress = writeRKGData(currentAddress, prevInput, amountCurrentFrames)
	local directionInputLength = currentAddress - newDirectionInput

	WriteValue32(addressTrickInputPointer, currentAddress + 0x80000000)
	WriteValue32(addressDirectionInputLengthPointer, directionInputLength)

	local newTrickInput = currentAddress

	prevInput = maskTrickInput(input_ghost[1][6])
	amountCurrentFrames = 0x0

	for _, inputs in ipairs(input_ghost) do
		local currentInput = maskTrickInput(inputs[6])
		if prevInput ~= currentInput then
			local inputData = prevInput + math.floor(amountCurrentFrames / 0x100)

			currentAddress = writeRKGData(currentAddress, inputData, amountCurrentFrames % 0x100)

			prevInput = currentInput
			amountCurrentFrames = 0x1
		else
			if amountCurrentFrames >= 0xFFF then
				local inputData = prevInput + math.floor(amountCurrentFrames / 0x100)

				currentAddress = writeRKGData(currentAddress, inputData, amountCurrentFrames % 0x100)

				prevInput = currentInput
				amountCurrentFrames = 0x1
			else
				amountCurrentFrames = amountCurrentFrames + 1
			end
		end
	end

	local inputData = prevInput + math.floor(amountCurrentFrames / 0x100)
	currentAddress = writeRKGData(currentAddress, inputData, amountCurrentFrames % 0x100)
	local trickInputLength = currentAddress - newTrickInput

	WriteValue32(addressTrickInputLengthPointer, trickInputLength)

	endOfFile = addressFaceButton + 0x2774
	while currentAddress < endOfFile do
		WriteValue16(currentAddress, 0x0)
		currentAddress = currentAddress + 0x2
	end


end
ghost_core.writeInputsIntoRKG = writeInputsIntoRKG

-- ###############################

return ghost_core
