-- stores the ghost as both the runner file and the ghost file

-- IMPORTANT: you can only run this, if the track and ghost have been fully loaded
-- after that you can run the script any time you want, before the first inputs can be made or even midrace

-- WARNING: this will overwrite any changes you have made to your runner file, make sure to use the other one to only write onto the ghost

package.path = GetScriptsDir() .. "MKW/CSV_Handler.lua"
local csv_handler = require("CSV_Handler")
package.path = GetScriptsDir() .. "config/TASToolKit_config.lua"
local config = require("TASToolKit_config")
package.path = GetScriptsDir() .. "MKW/TTK_Lib.lua"
local TTK_Lib = require("TTK_Lib")

function onScriptStart()
	MsgBox("Script started.")
	
	local input_list = TTK_Lib.readFullDecodedRKGData(TTK_Lib.PlayerTypeEnum.ghost)

	local playerResult = csv_handler.writeCSV(config.textFilePath.player, input_list)
	
	if (playerResult == 1) then
		error(config.textFilePath.player .. " is currently locked by another program, make sure to close it there first.", 0)
	end

	CancelScript()
end

function onScriptCancel()
	MsgBox("Script ended.")
end
