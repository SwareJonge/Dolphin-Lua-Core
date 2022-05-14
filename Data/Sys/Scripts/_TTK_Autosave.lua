-- stores the current race progress inputs into an input file
-- saves an rkg of the current progress

package.path = GetScriptsDir() .. "MKW/MKW_core.lua"
local core = require("MKW_core")
package.path = GetScriptsDir() .. "MKW/CSV_Handler.lua"
local csv_handler = require("CSV_Handler")
package.path = GetScriptsDir() .. "MKW/MKW_Pointers.lua"
local pointers = require("MKW_Pointers")
package.path = GetScriptsDir() .. "config/TASToolKit_config.lua"
local config = require("TASToolKit_config")
package.path = GetScriptsDir() .. "MKW/TTK_Lib.lua"
local TTK_Lib = require("TTK_Lib")

local iteration = 1
local prevFrame = 0

local autoFace, autoStick, autoTrick = {}, {}, {}


function updateData()
	autoFace = TTK_Lib.readRawRKGData(TTK_Lib.PlayerTypeEnum.player, TTK_Lib.ControllerInputTypeEnum.faceButton, {})
	autoStick = TTK_Lib.readRawRKGData(TTK_Lib.PlayerTypeEnum.player, TTK_Lib.ControllerInputTypeEnum.directionInput, {})
	autoTrick = TTK_Lib.readRawRKGData(TTK_Lib.PlayerTypeEnum.player, TTK_Lib.ControllerInputTypeEnum.trickInput, {})
end

function onScriptStart()
	if (config.backupAmount <= 0) then
		CancelScript()
	end
end

function onScriptUpdate()
	local currentFrame = core.getFrameOfInput()
	
	if (prevFrame > currentFrame) or ((prevFrame + 3) < currentFrame) then
		if (autoFace ~= nil) and (autoStick ~= nil) and (autoTrick ~= nil) then
			local inputList = TTK_Lib.decodeRKGData(autoFace, TTK_Lib.ControllerInputTypeEnum.faceButton, {})
			inputList = TTK_Lib.decodeRKGData(autoStick, TTK_Lib.ControllerInputTypeEnum.directionInput, inputList)
			inputList = TTK_Lib.decodeRKGData(autoTrick, TTK_Lib.ControllerInputTypeEnum.trickInput, inputList)
			
			local fileString = string.format("%s.%u", config.textFilePath.backup, iteration)
			csv_handler.writeCSV(fileString, inputList)
			
			iteration = (iteration % config.backupAmount) + 1
		end
		updateData()
	elseif (prevFrame < currentFrame) then
		updateData()
	end
	
	prevFrame = currentFrame
end

function onStateLoaded()

end

function onStateSaved()

end

function onScriptCancel()

end
