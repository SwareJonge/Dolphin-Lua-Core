local Pointers = {}

local core = require "MKW_core"

local function getRaceData2Pointer()
  local raceData2
  if core.getGameID() == "RMCP01" then raceData2 = 0x9BD730
  elseif core.getGameID() == "RMCE01"then raceData2 = 0x9B8F70
  elseif core.getGameID() == "RMCJ01" then raceData2 = 0x9BC790
  elseif core.getGameID() == "RMCK01" then raceData2 = 0x9ABD70
  end
  local offset = 0xAC --0xB0 for ghost's values
  local address = GetPointerNormal(raceData2)
  if ReadValue32(raceData2) == 0 then
		return 0
	end
  return address + offset
end
Pointers.getRaceData2Pointer = getRaceData2Pointer

local function getPositionPointer()
  local pointer
  if core.getGameID() == "RMCP01" then pointer = 0x9C2EF8
  elseif core.getGameID() == "RMCE01"then pointer = 0x9BE738
  elseif core.getGameID() == "RMCJ01" then pointer = 0x9C1F58
  elseif core.getGameID() == "RMCK01" then pointer = 0x9B1538
  end
  local offset = 0x40
  local address = GetPointerNormal(pointer)
  if ReadValue32(pointer) == 0 then
    return 0
  end
  return address + offset
end
Pointers.getPositionPointer = getPositionPointer


local function getPlayerBasePointer()
  local pointer
  if core.getGameID() == "RMCP01" then pointer = 0x9C18F8
  elseif core.getGameID() == "RMCE01"then pointer = 0x9BD110
  elseif core.getGameID() == "RMCJ01" then pointer = 0x9C0958
  elseif core.getGameID() == "RMCK01" then pointer = 0x9AFF38
  end
  local offset1 = 0x20
  local offset2 = 0x0 --0x4 for ghost's values
  local offset3 = 0x10
  local offset4 = 0x10
  local offset5 = 0x0
  if ReadValue32(pointer) == 0 then
    return 0
  end
  local address1 = GetPointerNormal(pointer)
  local address2 = GetPointerNormal(address1 + offset1)
  local address3 = GetPointerNormal(address2 + offset2)
  local address4 = GetPointerNormal(address3 + offset3)
  return address4 + offset4
end
Pointers.getPlayerBasePointer = getPlayerBasePointer

local function getFrameOfInputAddress()
  local frameaddress
  if core.getGameID() == "RMCP01" then frameaddress = 0x9C38C2
  elseif core.getGameID() == "RMCE01" then frameaddress = 0x9BF0BA
  elseif core.getGameID() == "RMCJ01" then frameaddress = 0x9C2922
  elseif core.getGameID() == "RMCK01" then frameaddress = 0x9B1F02
  end
  return frameaddress
end
Pointers.getFrameOfInputAddress = getFrameOfInputAddress

return Pointers
