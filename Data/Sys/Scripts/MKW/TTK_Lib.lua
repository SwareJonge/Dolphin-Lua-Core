-- Collection of various TTK functionalities, usable for own lua scripts

----- GLOBAL VARIABLES -----
local TTK_Lib = {}

package.path = GetScriptsDir() .. "config/TASToolKit_config.lua"
local config = require("TASToolKit_config")
package.path = GetScriptsDir() .. "MKW/MKW_ghost_core.lua"
local ghost_core = require("MKW_ghost_core")

TTK_Lib.PlayerTypeEnum = {player = 0,
						  ghost = 1}
TTK_Lib.ControllerInputTypeEnum = {faceButton = 0,
								   directionInput = 1,
								   trickInput = 2}

-- ###############################


-- decodes and encodes inputs into/from their hexadecimal representation


local function decodeFaceButton(input)
	local buttonA = input % 0x2
	local buttonB = (input >> 1) % 0x2
	local buttonL = (input >> 2) % 0x2
	
	return buttonA, buttonB, buttonL
end
TTK_Lib.decodeFaceButton = decodeFaceButton

local function decodeDirectionInput(input)
	local stickH = input >> 4
	local stickV = input % 0x10
	
	return stickH, stickV
end
TTK_Lib.decodeDirectionInput = decodeDirectionInput

local function decodeTrickInput(input)
	return input >> 4
end
TTK_Lib.decodeTrickInput = decodeTrickInput

local function encodeFaceButton(aButton, bButton, lButton, prevMask)
	local x8Mask = 0x0
	-- if bButton == 1 and (prevMask == 0x1 or prevMask == 0x5 or prevMask == 0xB or prevMask == 0xF) then
	if aButton == 1 and bButton == 1 and not(prevMask == 0x2 or prevMask == 0x3 or prevMask == 0x7) then
		x8Mask = 0x8
	end
	return aButton * 0x1 + bButton * 0x2 + lButton * 0x4 + x8Mask
end
TTK_Lib.encodeFaceButton = encodeFaceButton

local function encodeDirectionInput(horizonalInput, verticalInput)
	return (horizonalInput << 4) + verticalInput
end
TTK_Lib.encodeDirectionInput = encodeDirectionInput

local function encodeTrickInput(trickInput)
	return trickInput * 0x10
end
TTK_Lib.encodeTrickInput = encodeTrickInput

-- ###############################

function string.fromhex(str)
    return (str:gsub('..', function (cc)
        return string.char(tonumber(cc, 16))
    end))
end

-- ###############################

-- decodes RKG data from their hexadecimal representation
-- variables:
-- rawBinary -> the binary RKG data section
-- inputType -> the specified input section, type TTK_Lib.ControllerInputTypeEnum
-- inputList -> the inputList to add the result to, for TTK_Lib.ControllerInputTypeEnum.faceButton should be {} (empty table)

-- return value -> list of the decoded inputs

-- usage example:
-- local faceButtonBinary = ... -- this should have the binary faceButton data
-- local inputList = TTK_Lib.decodeRKGData(faceButtonBinary, TTK_Lib.ControllerInputTypeEnum.faceButton, {}) -- use empty list for faceButton

local function decodeRKGData(rawBinary, inputType, inputList)
	local inputIndex = 1

	if (inputType == TTK_Lib.ControllerInputTypeEnum.faceButton or
		inputType == TTK_Lib.ControllerInputTypeEnum.directionInput) then
		local rawInput = 0x0
		for dataIndex, dataByte in ipairs(rawBinary) do
			if (dataIndex % 2) == 1 then
				rawInput = dataByte
			else
				if (inputType == TTK_Lib.ControllerInputTypeEnum.faceButton) then
					for i = 1, dataByte do
						inputList[#inputList+1] = {decodeFaceButton(rawInput)}
					end
				elseif (inputType == TTK_Lib.ControllerInputTypeEnum.directionInput) then
					for i = 1, dataByte do
						inputList[inputIndex][4], inputList[inputIndex][5] = decodeDirectionInput(rawInput)
						inputIndex = inputIndex + 1
					end
				end
				
			end
		end
	elseif (inputType == TTK_Lib.ControllerInputTypeEnum.trickInput) then
		local trickInput = 0x0
		local x100Length = 0x0
		for dataIndex, dataByte in ipairs(rawBinary) do
			if (dataIndex % 2) == 1 then
				trickInput = decodeTrickInput(dataByte)
				x100Length = dataByte % 0x10
			else
				local dataLength = x100Length * 0x100 + dataByte
				for i = 1, dataLength do
					inputList[inputIndex][6] = trickInput
					inputIndex = inputIndex + 1
				end
			end
		end
	end
	
	return inputList
end
TTK_Lib.decodeRKGData = decodeRKGData


-- reads binary RKG data from the specified section
-- variables:
-- playerType -> the player type to load from, type TTK_Lib.PlayerTypeEnum (player uses previously used inputs, ghost uses current racing ghost)
-- inputType -> the specified input section, type TTK_Lib.ControllerInputTypeEnum
-- dataList -> the dataList to add the result to, can be {} (empty)

-- return value -> the read binary data

-- usage example:
-- local ghostFaceButtonBinary = TTK_Lib.readRawRKGData(TTK_Lib.PlayerTypeEnum.ghost, TTK_Lib.ControllerInputTypeEnum.faceButton, {})

local function readRawRKGData(playerType, inputType, dataList)
	local currentAddress = 0x0
	local endingAddress = 0x0
	
	if (playerType == TTK_Lib.PlayerTypeEnum.player) then
		currentAddress = ghost_core.getInputStorageAddresses()
		if (currentAddress == 0) then
			return nil
		end
		
		if(inputType == TTK_Lib.ControllerInputTypeEnum.faceButton) then
			currentAddress, endingAddress, _ = ghost_core.getInputStorageAddresses()
		elseif(inputType == TTK_Lib.ControllerInputTypeEnum.directionInput) then
			_, currentAddress, endingAddress = ghost_core.getInputStorageAddresses()
		elseif(inputType == TTK_Lib.ControllerInputTypeEnum.trickInput) then
			_, _, currentAddress = ghost_core.getInputStorageAddresses()
			endingAddress = currentAddress + 0x276C
		else
			return 0
		end
	elseif(playerType == TTK_Lib.PlayerTypeEnum.ghost) then
		currentAddress = ghost_core.getGhostAddresses()
		if (currentAddress == 0) then
			return nil
		end
		
		if(inputType == TTK_Lib.ControllerInputTypeEnum.faceButton) then
			currentAddress, endingAddress, _ = ghost_core.getGhostAddresses()
		elseif(inputType == TTK_Lib.ControllerInputTypeEnum.directionInput) then
			_, currentAddress, endingAddress = ghost_core.getGhostAddresses()
		elseif(inputType == TTK_Lib.ControllerInputTypeEnum.trickInput) then
			_, _, currentAddress = ghost_core.getGhostAddresses()
			local _, _, addressLengthPointer = ghost_core.getGhostAddressLengthPointer()
			endingAddress = currentAddress + ReadValue32(addressLengthPointer)
		else
			return 0
		end
	else
		return 0
	end
	
	local dataTuple = ReadValue16(currentAddress)
	
	repeat
		dataList[#dataList+1] = dataTuple >> 8
		dataList[#dataList+1] = dataTuple % 0x100
		currentAddress = currentAddress + 0x2
		dataTuple = ReadValue16(currentAddress)
	until (dataTuple == 0x0000) or (currentAddress >= endingAddress)
	
	return dataList
end
TTK_Lib.readRawRKGData = readRawRKGData


-- shortcut method for reading the full decoded RKG from either player or ghost
-- variables:
-- playerType -> the player type to load from, type TTK_Lib.PlayerTypeEnum (player uses previously used inputs, ghost uses current racing ghost)

-- return value -> the decoded input list from the specified player type

-- usage example:
-- local ghostInputList = TTK_Lib.readFullDecodedRKGData(TTK_Lib.PlayerTypeEnum.ghost)

local function readFullDecodedRKGData(playerType)
	local fbData = TTK_Lib.readRawRKGData(playerType, TTK_Lib.ControllerInputTypeEnum.faceButton, {})
	local diData = TTK_Lib.readRawRKGData(playerType, TTK_Lib.ControllerInputTypeEnum.directionInput, {})
	local tiData = TTK_Lib.readRawRKGData(playerType, TTK_Lib.ControllerInputTypeEnum.trickInput, {})
	
	if ((fbData == nil) or (diData == nil) or (tiData == nil)) then
		return nil
	end

	local inputList = TTK_Lib.decodeRKGData(fbData, TTK_Lib.ControllerInputTypeEnum.faceButton, {})
	inputList = TTK_Lib.decodeRKGData(diData, TTK_Lib.ControllerInputTypeEnum.directionInput, inputList)
	inputList = TTK_Lib.decodeRKGData(tiData, TTK_Lib.ControllerInputTypeEnum.trickInput, inputList)
	
	return inputList
end
TTK_Lib.readFullDecodedRKGData = readFullDecodedRKGData


-- encodes an input list into binary RKG data
-- variables:
-- inputList -> the input table containing the inputs

-- return values (multiple): data, fbBytes, diBytes, tiBytes
-- data -> the encoded rkg data
-- fbBytes -> the amount of faceButton tuples in $data
-- diBytes -> the amount of directionInput tuples in $data
-- tiBytes -> the amount of trickInput tuples in $data

-- usage example:
-- local inputList = ... -- contains the decoded input list
-- local data, fbBytes, diBytes, tiBytes = TTK_Lib.encodeRKGData(inputList)
-- local data2 = TTK_Lib.encodeRKGData(inputList) -- also works but you wont receive any section size information

local function encodeRKGData(inputList)
	local data = {}
	local dataIndex = 1
	
	local fbBytes, diBytes, tiBytes = 0, 0, 0
	
	local prevInput = encodeFaceButton(inputList[1][1], inputList[1][2], inputList[1][3], 0x0)
	local amountCurrentFrames = 0x0
	
	for _, inputs in ipairs(inputList) do
		local currentInput = encodeFaceButton(inputs[1], inputs[2], inputs[3], prevInput)
		if prevInput ~= currentInput then
			data[#data+1] = prevInput
			data[#data+1] = amountCurrentFrames
			
			prevInput = currentInput
			amountCurrentFrames = 0x1
			fbBytes = fbBytes + 1
		else
			if amountCurrentFrames >= 0xFF then
				data[#data+1] = prevInput
				data[#data+1] = amountCurrentFrames
				
				prevInput = currentInput
				amountCurrentFrames = 0x1
				fbBytes = fbBytes + 1
			else
				amountCurrentFrames = amountCurrentFrames + 1
			end
		end
	end
	
	data[#data+1] = prevInput
	data[#data+1] = amountCurrentFrames
	fbBytes = fbBytes + 1
	
	prevInput = encodeDirectionInput(inputList[1][4], inputList[1][5])
	amountCurrentFrames = 0x0
	
	for _, inputs in ipairs(inputList) do
		local currentInput = encodeDirectionInput(inputs[4], inputs[5])
		if prevInput ~= currentInput then
			data[#data+1] = prevInput
			data[#data+1] = amountCurrentFrames
			
			prevInput = currentInput
			amountCurrentFrames = 0x1
			diBytes = diBytes + 1
		else
			if amountCurrentFrames >= 0xFF then
				data[#data+1] = prevInput
				data[#data+1] = amountCurrentFrames
				
				prevInput = currentInput
				amountCurrentFrames = 0x1
				diBytes = diBytes + 1
			else
				amountCurrentFrames = amountCurrentFrames + 1
			end
		end
	end
	
	data[#data+1] = prevInput
	data[#data+1] = amountCurrentFrames
	diBytes = diBytes + 1
	
	prevInput = encodeTrickInput(inputList[1][6])
	amountCurrentFrames = 0x0
	
	for _, inputs in ipairs(inputList) do
		local currentInput = encodeTrickInput(inputs[6])
		if prevInput ~= currentInput then
			data[#data+1] = prevInput + (amountCurrentFrames >> 8)
			data[#data+1] = amountCurrentFrames % 0x100
			
			prevInput = currentInput
			amountCurrentFrames = 0x1
			tiBytes = tiBytes + 1
		else
			if amountCurrentFrames >= 0xFFF then
				data[#data+1] = prevInput + (amountCurrentFrames >> 8)
				data[#data+1] = amountCurrentFrames % 0x100
				
				prevInput = currentInput
				amountCurrentFrames = 0x1
				tiBytes = tiBytes + 1
			else
				amountCurrentFrames = amountCurrentFrames + 1
			end
		end
	end
	
	data[#data+1] = prevInput + (amountCurrentFrames >> 8)
	data[#data+1] = amountCurrentFrames % 0x100
	tiBytes = tiBytes + 1
		
	return data, fbBytes, diBytes, tiBytes
end
TTK_Lib.encodeRKGData = encodeRKGData


-- converts a byte array table to a string, for example the binary rkg data
-- Tip: the outputed string can be used for writing binary files (io.open(filename, "wb"))
-- variables:
-- byteArray -> the byte table

-- return value -> the string format

-- usage example:
-- local binaryRKGdata = TTK_Lib.encodeRKGData({{1,1,0,7,7,0}, {1,0,0,14,7,0}})
-- local rkgString = TTK_Lib.byteArrayToString(binaryRKGdata)

local function byteArrayToString(byteArray)
	local dataString = ""
	
	for _, dataByte in ipairs(byteArray) do
		dataString = dataString .. string.format("%02X", dataByte):fromhex()
	end
	
	return dataString
end
TTK_Lib.byteArrayToString = byteArrayToString


-- converts a string to a byte array table using ASCII code
-- variables:
-- dataString -> the string to be converted

-- return value -> the converted byte table/array

-- usage example:
-- local hexadecimalHelloWorld = TTK_Lib.stringToByteArray("Hello World!")

local function stringToByteArray(dataString)
	local data = {}
	
	dataString:gsub(".", function(c) table.insert(data, string.byte(c)) end)
	
	return data
end
TTK_Lib.stringToByteArray = stringToByteArray


-- runs the specified input in dolphin on gc controller 1
-- variables:
-- currentInput -> the input to be made in {A, B, L, horizonalInput, verticalInput, dpad} format

-- usage example:
-- TTK_Lib.runInput({1, 0, 0, 7, 7, 0})

local function runInput(currentInput)
	local aButton = currentInput[1]
	local bButton = currentInput[2]
	local lButton = currentInput[3]
	local horizontalInput = currentInput[4]
	local verticalInput = currentInput[5]
	local dPad = currentInput[6]

	if horizontalInput == nil then SetMainStickX(128)
	else
		if horizontalInput == 0 then SetMainStickX(59)
		elseif horizontalInput == 1 then SetMainStickX(68)
		elseif horizontalInput == 2 then SetMainStickX(77)
		elseif horizontalInput == 3 then SetMainStickX(86)
		elseif horizontalInput == 4 then SetMainStickX(95)
		elseif horizontalInput == 5 then SetMainStickX(104)
		elseif horizontalInput == 6 then SetMainStickX(112)
		elseif horizontalInput == 7 then SetMainStickX(128)
		elseif horizontalInput == 8 then SetMainStickX(152)
		elseif horizontalInput == 9 then SetMainStickX(161)
		elseif horizontalInput == 10 then SetMainStickX(170)
		elseif horizontalInput == 11 then SetMainStickX(179)
		elseif horizontalInput == 12 then SetMainStickX(188)
		elseif horizontalInput == 13 then SetMainStickX(197)
		elseif horizontalInput == 14 then SetMainStickX(205)
		end
	end

	if verticalInput == nil then SetMainStickY(128)
	else
		if verticalInput == 0 then SetMainStickY(59)
		elseif verticalInput == 1 then SetMainStickY(68)
		elseif verticalInput == 2 then SetMainStickY(77)
		elseif verticalInput == 3 then SetMainStickY(86)
		elseif verticalInput == 4 then SetMainStickY(95)
		elseif verticalInput == 5 then SetMainStickY(104)
		elseif verticalInput == 6 then SetMainStickY(112)
		elseif verticalInput == 7 then SetMainStickY(128)
		elseif verticalInput == 8 then SetMainStickY(152)
		elseif verticalInput == 9 then SetMainStickY(161)
		elseif verticalInput == 10 then SetMainStickY(170)
		elseif verticalInput == 11 then SetMainStickY(179)
		elseif verticalInput == 12 then SetMainStickY(188)
		elseif verticalInput == 13 then SetMainStickY(197)
		elseif verticalInput == 14 then SetMainStickY(205)
		end
	end
	
	local bString = "B"
	if (config.useRbutton) then
		bString = "R"
	end

	if aButton == 1 then PressButton("A") end
	if bButton == 1 then PressButton(bString) end
	if lButton == 1 then PressButton("L") end

	if dPad == 1 then PressButton("D-Up")
	elseif dPad == 2 then PressButton("D-Down")
	elseif dPad == 3 then PressButton("D-Left")
	elseif dPad == 4 then PressButton("D-Right")
	end
end
TTK_Lib.runInput = runInput

-- ###############################

return TTK_Lib
