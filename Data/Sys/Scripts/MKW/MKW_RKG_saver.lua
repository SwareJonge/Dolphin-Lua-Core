--Overwrites ghosts using the output of the mkw_input_writer
--File needs to be named "mkw_input_reader_ghost.lua"

package.path = GetScriptsDir() .. "MKW/CSV_Handler.lua"
local csv_handler = require("CSV_Handler")
package.path = GetScriptsDir() .. "MKW/MKW_Pointers.lua"
local pointers = require("MKW_Pointers")
package.path = GetScriptsDir() .. "MKW/TTK_Lib.lua"
local TTK_Lib = require("TTK_Lib")

local rkg_saver = {}

-- ##################################

function string.fromhex(str)
    return (str:gsub('..', function (cc)
        return string.char(tonumber(cc, 16))
    end))
end

-- https://github.com/lancelijade/qqwry.lua/blob/master/crc32.lua

local CRC32 = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 
    0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3, 
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 
    0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 
    0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7, 
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 
    0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5, 
    0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 
    0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59, 
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 
    0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f, 
    0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 
    0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 
    0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433, 
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 
    0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01, 
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 
    0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 
    0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65, 
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 
    0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 
    0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 
    0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 
    0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f, 
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 
    0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad, 
    0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 
    0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 
    0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1, 
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 
    0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7, 
    0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 
    0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b, 
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 
    0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79, 
    0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 
    0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 
    0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d, 
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 
    0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713, 
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 
    0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 
    0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777, 
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 
    0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45, 
    0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 
    0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 
    0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9, 
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 
    0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf, 
    0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
}

function hash(str)
    str = tostring(str)
    local count = string.len(str)
    local crc = 2 ^ 32 - 1 
    local i = 1

    while count > 0 do
        local dataByte = string.byte(str, i)
        crc = (crc >> 8) ~ CRC32[((crc & 0xFF) ~ dataByte) + 1]
        i = i + 1
        count = count - 1
    end
    crc = crc ~ 0xFFFFFFFF
    -- dirty hack for bitop return number < 0
    if crc < 0 then crc = crc + 2 ^ 32 end 
	
	local crcString = string.format("%02X", math.floor(crc / 0x1000000)):fromhex()
	crcString = crcString .. string.format("%02X", math.floor((crc & 0x00FF0000) / 0x10000)):fromhex()
	crcString = crcString .. string.format("%02X", math.floor((crc & 0x0000FF00) / 0x100)):fromhex()
	crcString = crcString .. string.format("%02X", math.floor(crc % 0x100)):fromhex()

    return crcString
end

function replace_char(rkgString, pos, r)
	return rkgString:sub(1, pos-1) .. r .. rkgString:sub(pos+1)
end

-- ####################################

function loadCurrentRksys()
	local rksysString = ""
	local pointerIndex = 0
	
	repeat
		rksysString = rksysString .. string.format("%02X", ReadValue8(pointers.getRKSYSPointer() + pointerIndex)):fromhex()
		pointerIndex = pointerIndex + 1
	until pointerIndex == 0x27FFC
	
	return rksysString
end

-- ####################################

function createRKGFile(input_data, trackID, vehicleID, characterID, driftID)
	local data, fbBytes, diBytes, tiBytes = TTK_Lib.encodeRKGData(input_data)
	local dataIndex = (fbBytes + diBytes + tiBytes) * 0x2

	MsgBox(string.format("%X, %X, %X, %X", trackID, vehicleID, characterID, driftID))
	
	local inputLength = dataIndex + 0x8
	
	local byteNr8 = (vehicleID << 2) + ((characterID >> 4) & 0x3)
	local byteNr9 = (characterID << 4) & 0xFF
	local byteNrD = 0x4 + (driftID << 1)
	
	local accessData =
		{0x54, 0xA8, 0x2A, trackID << 2, byteNr8, byteNr9, 0x02, 0x10, 0x00, byteNrD, math.floor(inputLength / 0x100), inputLength % 0x100
		,0x03, 0x54, 0x00, 0x00, 0x00, 0xA8, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x00 ,0x00, 0x00, 0x00, 0x00
		,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		,0x00, 0x00, 0x00, 0x00, 0xAA, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x10, 0x00, 0x54
		,0x00, 0x41, 0x00, 0x53, 0x00, 0x54, 0x00, 0x6F, 0x00, 0x6F, 0x00, 0x6C, 0x00, 0x6B, 0x00, 0x69
		,0x00, 0x74, 0x00, 0x22, 0x87, 0x30, 0x89, 0x66, 0xC2, 0xC4, 0xED, 0xC3, 0x20, 0x44, 0x3C, 0x40
		,0x28, 0x38, 0x0C, 0x84, 0x48, 0xCF, 0x0E, 0x00, 0x08, 0x00, 0xB9, 0x09, 0x00, 0x8A, 0x81, 0x06
		,0xC4, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7A, 0x6E, math.floor(fbBytes / 0x100), fbBytes % 0x100, math.floor(diBytes / 0x100), diBytes % 0x100, math.floor(tiBytes / 0x100), tiBytes % 0x100, 0x00, 0x00}
	
	local fileString = "RKGD" .. TTK_Lib.byteArrayToString(accessData)
	fileString = fileString .. TTK_Lib.byteArrayToString(data)
	
	local zeroByte = string.format("%02X", 0x00):fromhex()
	
	while dataIndex < 0x276C do
		dataIndex = dataIndex + 1
		fileString = fileString .. zeroByte
	end
	
	local crcString = hash(fileString)
	
	return fileString, crcString
end
rkg_saver.createRKGFile = createRKGFile

-- ###############################

function storeDownloadedGhost(rkgString, rksysFilePath, rksysWritePath)
	local byteNrD = string.byte(rkgString, 0xD + 1)
	if byteNrD < 0x8 then
		byteNrD = (byteNrD - 0x4) + 0x8
		rkgString = replace_char(rkgString, 0xD + 1, string.format("%02X", byteNrD):fromhex())
	end

	rkgString = rkgString .. hash(rkgString)
	
	if string.len(rkgString) ~= 0x2800 then
		MsgBox("String Length Error")
		return 1
	end
	
	local load_file = io.open(rksysFilePath, "rb")
	local rksysString = load_file:read("*all")
	load_file:close()
	
	local downloadGhostFlag = string.byte(rksysString, 0x13 + 1)
	if downloadGhostFlag > 0x1 then return 1
	elseif downloadGhostFlag == 0x0 then
		rksysString = replace_char(rksysString, 0x13 + 1, string.format("%02X", 0x1):fromhex())
		
		local rksysHead = rksysString:sub(1, 0x27FFC)
		local crcString = hash(rksysHead)
		local rksysTail = rksysString:sub(0x28000 + 1)
		
		rksysString = rksysHead .. crcString .. rksysTail
	end
	
	rksysString = rksysString:sub(1, 0x78000) .. rkgString .. rksysString:sub(0x7A800 + 1)
	
	local write_file = io.open(rksysWritePath, "wb")
	io.output(write_file)
	io.write(rksysString)
end
rkg_saver.storeDownloadedGhost = storeDownloadedGhost

-- ###############################

return rkg_saver