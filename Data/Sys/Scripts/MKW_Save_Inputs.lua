-- stores the current race progress inputs into an input file
-- saves an rkg of the current progress

package.path = GetScriptsDir() .. "MKW/CSV_Handler.lua"
local csv_handler = require("CSV_Handler")
package.path = GetScriptsDir() .. "MKW/MKW_RKG_saver.lua"
local rkg_saver = require("MKW_RKG_saver")
package.path = GetScriptsDir() .. "MKW/MKW_Pointers.lua"
local pointers = require("MKW_Pointers")
package.path = GetScriptsDir() .. "config/TASToolKit_config.lua"
local config = require("TASToolKit_config")
package.path = GetScriptsDir() .. "MKW/TTK_Lib.lua"
local TTK_Lib = require("TTK_Lib")

function onScriptStart()
	MsgBox("Script started.")

	
	local input_list = TTK_Lib.readFullDecodedRKGData(TTK_Lib.PlayerTypeEnum.player)
	if (input_list == nil) then
		MsgBox("The Script can't be used after the race ended")
	else
		local trackID = ReadValue32(pointers.getRaceDataPointer(), 0xB68)
		local vehicleID = ReadValue32(pointers.getRaceDataPointer(), 0x30)
		local characterID = ReadValue32(pointers.getRaceDataPointer(), 0x34)
		local driftID = ReadValue16(pointers.getInputDataPointer(), 0xC4)
		
		if (csv_handler.writeCSV(config.textFilePath.player, input_list) == 1) then
			error(config.textFilePath.player .. " is currently locked by another program, make sure to close it there first.", 0)
		end
		
		
		local rkgString, crc = rkg_saver.createRKGFile(input_list, trackID, vehicleID, characterID, driftID)
		
		local write_file = io.open(config.saveProgress.rkgFilePath, "wb")
		io.output(write_file)
		io.write(rkgString .. crc)
		
		if (config.saveProgress.createRKSYScopy) then
			rkg_saver.storeDownloadedGhost(rkgString, config.saveProgress.originalRksysPath, config.saveProgress.rksysCopyFilePath)
		end
	end

	CancelScript()
end

function onScriptCancel()
	MsgBox("Script ended.")
end
