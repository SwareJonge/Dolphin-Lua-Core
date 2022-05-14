-- stores the ghost as a file with the sequence of inputs

-- IMPORTANT: you can only run this, if the track and ghost have been fully loaded
-- after that you can run the script any time you want, before the first inputs can be made or even midrace


package.path = GetScriptsDir() .. "MKW/CSV_Handler.lua"
local csv_handler = require("CSV_Handler")
package.path = GetScriptsDir() .. "config/TASToolKit_config.lua"
local config = require("TASToolKit_config")
package.path = GetScriptsDir() .. "MKW/TTK_Lib.lua"
local TTK_Lib = require("TTK_Lib")


function onScriptStart()
	MsgBox("Script started.")

	local input_list = TTK_Lib.readFullDecodedRKGData(TTK_Lib.PlayerTypeEnum.ghost)
	
	if (csv_handler.writeCSV(config.textFilePath.ghost, input_list) == 1) then
		error(config.textFilePath.ghost .. " is currently locked by another program, make sure to close it there first.", 0)
	end

	CancelScript()
end

function onScriptCancel()
	MsgBox("Script ended.")
end
