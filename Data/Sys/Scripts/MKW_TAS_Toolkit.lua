-- general TAS Toolkit
-- used to do inputs of yourself and your ghost simultaneously


local core = require "MKW_core"
local ghost_core = require "MKW_ghost_core"
local input_ghost = {}
local input_runner = {}

local runner_loaded = false
local ghost_loaded = false

local frameCount = 0

function tableLength(tableInput)
	local count = 0
	for _ in pairs(tableInput) do count = count + 1 end
	return count
end

function runInputs()
	local currentFrame = core.getFrameOfInput() + 1
	
	if currentFrame <= frameCount then
	
		local inputs = input_runner[currentFrame]
		
		local aButton = inputs[1]
		local bButton = inputs[2]
		local lButton = inputs[3]
		local horizontalInput = inputs[4]
		local verticalInput = inputs[5]
		local dPad = inputs[6]
		
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
		
		if aButton == 1 then PressButton("A") end
		if bButton == 1 then PressButton("B") end
		if lButton == 1 then PressButton("L") end
		
		if dPad == 1 then PressButton("D-Up")
		elseif dPad == 2 then PressButton("D-Down")
		elseif dPad == 3 then PressButton("D-Left")
		elseif dPad == 4 then PressButton("D-Right")
		end
	
	end
end

-- ##################################

function onScriptStart()
	MsgBox("Script started.")
	runner_loaded, input_runner = pcall(require, "mkw_input_reader_runner")
	ghost_loaded, input_ghost = pcall(require, "mkw_input_reader_ghost")
	
	if(runner_loaded) then
		frameCount = tableLength(input_runner)
	end
	if(ghost_loaded) then
		ghost_core.writeInputsIntoRKG(input_ghost)
	end
end

function onScriptCancel()
	MsgBox("Script ended.")
end

function onScriptUpdate()
	if(runner_loaded) then
		runInputs()
	end
end

function onStateLoaded()
	if(ghost_loaded) then
		ghost_core.writeInputsIntoRKG(input_ghost)
	end
end

function onStateSaved()

end