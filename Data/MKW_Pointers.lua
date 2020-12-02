local Pointers = {}

local function getRaceData2Pointer(Offset)
  local raceData2
  if GetGameID() == "RMCP01" then raceData2 = 0x9BD730
  elseif GetGameID() == "RMCE01"then raceData2 = 0x9B8F70
  elseif GetGameID() == "RMCJ01" then raceData2 = 0x9BC790
  elseif GetGameID() == "RMCK01" then raceData2 = 0x9ABD70
  end
  ReadValue32(pointer, 0xC, Offset)
end
Pointers.getRaceData2Pointer = getRaceData2Pointer

local function getPositionPointer(Offset)
  local pointer
  if GetGameID() == "RMCP01" then pointer = 0x9C18F8
  elseif GetGameID() == "RMCE01"then pointer = 0x9BD110
  elseif GetGameID() == "RMCJ01" then pointer = 0x9C0958
  elseif GetGameID() == "RMCK01" then pointer = 0x9AFF38
  end
  local returnValue = ReadValue32(pointer, 0xC, 0x10, Offset, 0x10, 0x10, 0x4, 0x18, 0xC, 0x34, 0x0, 0x8, 0x8, 0x8, 0x90, 0x4) + 0x68 -- innaccurate, fix this next update

  if returnValue == 0 or returnValue == 0x68 then
  returnValue = 0
  end
  return returnValue
end
Pointers.getPositionPointer = getPositionPointer

local function getPlayerBasePointer()
  local pointer
  if GetGameID() == "RMCP01" then pointer = 0x9C18F8
  elseif GetGameID() == "RMCE01"then pointer = 0x9BD110
  elseif GetGameID() == "RMCJ01" then pointer = 0x9C0958
  elseif GetGameID() == "RMCK01" then pointer = 0x9AFF38
  end
  return ReadValue32(pointer, 0xC, 0x10, 0x0, 0x10, 0x10)
end
Pointers.getPlayerBasePointer = getPlayerBasePointer

local function getFrameOfInputAddress()
  local frameaddress
  if GetGameID() == "RMCP01" then frameaddress = 0x9C38C0
  elseif GetGameID() == "RMCE01" then frameaddress = 0x9BF0B8
  elseif GetGameID() == "RMCJ01" then frameaddress = 0x9C2920
  elseif GetGameID() == "RMCK01" then frameaddress = 0x9B1F00
  end
  return frameaddress
end
Pointers.getFrameOfInputAddress = getFrameOfInputAddress

return Pointers
