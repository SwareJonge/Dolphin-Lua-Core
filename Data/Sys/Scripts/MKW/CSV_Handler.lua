----- GLOBAL VARIABLES -----
local csv_handler = {}

package.path = GetScriptsDir() .. "config/TASToolKit_config.lua"
local config = require("TASToolKit_config")

-- ###############################

local function loadCSV(csvFileName)
	local inputList = {}
	local csvLoaded = false

	local csvFile = io.open(csvFileName, "r")
	if csvFile ~= nil then
		io.close(csvFile)
		csvLoaded = true
		
		local subtractiveStick = 0
		
		if (config.useNegativeStickInput) then
			subtractiveStick = 7
		end

		for line in io.lines(csvFileName) do
			local matchString = "(%d*)%" .. config.csvSeperator ..
								"(%d*)%" .. config.csvSeperator ..
								"(%d*)%" .. config.csvSeperator ..
								"(%-?%d+)%" .. config.csvSeperator ..
								"(%-?%d+)%" .. config.csvSeperator ..
								"(%d*)"
			-- local aB, bB, lB, hI, vI, dP = line:match("(%d*)%,(%d*)%,(%d*)%,%s*(.-)%,%s*(.-)%,(%d*)")
			local aB, bB, lB, hI, vI, dP = line:match(matchString)
			hI = hI + subtractiveStick
			vI = vI + subtractiveStick
			inputList[#inputList + 1] = {tonumber(aB), tonumber(bB), tonumber(lB), hI, vI, tonumber(dP)}
		end
	end


return csvLoaded, inputList

end
csv_handler.loadCSV = loadCSV

-- ###############################

-- will give an error if you have your table you want to save opened in a different application, such as libre-office
local function writeCSV(csvFileName, inputTable)
	local write_file = io.open(csvFileName, "w")
	if (write_file == nil) then
		return 1
	end
	
	io.output(write_file)
	
	local subtractiveStick = 0
	
	if (config.useNegativeStickInput) then
		subtractiveStick = -7
	end
	
	for _, inputs in ipairs(inputTable) do
		local matchString = "%u" .. config.csvSeperator ..
							"%u" .. config.csvSeperator ..
							"%u" .. config.csvSeperator ..
							"%d" .. config.csvSeperator ..
							"%d"  .. config.csvSeperator ..
							"%u\n"
		local outputString = string.format(matchString, inputs[1], inputs[2], inputs[3],
							inputs[4] + subtractiveStick, inputs[5] + subtractiveStick, inputs[6])
		io.write(outputString)
	end
	
	io.close(write_file)
	return 0
end
csv_handler.writeCSV = writeCSV


-- ###############################

return csv_handler
