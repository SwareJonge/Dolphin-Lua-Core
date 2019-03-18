local core = {}

local function getGameID()
    return ReadValueString(0x0, 6)
end
core.getGameID = getGameID

local function getPos()
  local pointer = 0x9BD16C
  local offset1 = 0x0
  local address1 = GetPointerNormal(pointer)
  if ReadValue32(address1) >= 0x80000000 then
  local address2 = GetPointerNormal(address1 + offset1)
  local offset2 = 0x0
  return {X = ReadValueFloat(address2 + offset2), Y = ReadValueFloat(address2 + offset2 + 4), Z = ReadValueFloat(address2 + offset2 + 8)}
  else return {X = 0, Y = 0, Z = 0}
  end
end
core.getPos = getPos

local function getPrevPos()
  local pointer = 0x9BD16C
  local offset1 = 0x0
  local address1 = GetPointerNormal(pointer)
  if ReadValue32(address1) >= 0x80000000 then
  local address2 = GetPointerNormal(address1 + offset1)
  local offset2 = -0x160
  return {X = ReadValueFloat(address2 + offset2), Y = ReadValueFloat(address2 + offset2 + 4), Z = ReadValueFloat(address2 + offset2 + 8)}
  else return {X = 0, Y = 0, Z = 0}
  end
end
core.getPrevPos = getPrevPos

local function getSpd()
  local PrevXpos = getPrevPos().X
  local PrevYpos = getPrevPos().Y
  local PrevZpos = getPrevPos().Z
  local Xpos = getPos().X
  local Ypos = getPos().Y
  local Zpos = getPos().Z
  return {Y = (Ypos - PrevYpos), XZ = math.sqrt(((Xpos - PrevXpos)^2) + (Zpos - PrevZpos)^2), XYZ = math.sqrt(((Xpos - PrevXpos)^2) + ((Ypos - PrevYpos)^2) + (Zpos - PrevZpos)^2)}
end
core.getSpd = getSpd

local function getInput()
  local pointer = 0
  if getGameID() == "RMCP01" then pointer = 0x42E324
  elseif getGameID() == "RMCE01" then pointer = 0x429FA4
  elseif getGameID() == "RMCJ01" then pointer = 0x42DC14
  elseif getGameID() == "RMCK01" then pointer = 0x41C2B4
  end
  local offset = 0x2840
  local address = GetPointerNormal(pointer)
  if ReadValue32(pointer) == 0 then
		return {ABLR = 0, X = 0, Y = 0, DPAD = 0}
	end
  return {
   ABLR = ReadValue8(address + offset + 0x1),
   X = ReadValue8(address + offset + 0xC),
   Y = ReadValue8(address + offset + 0xD),
   DPAD = ReadValue8(address + offset + 0xF)}
end
core.getInput = getInput

--FrameCounter in Race
local function getFrameOfInput()
  local frameaddress = 0x0
  if getGameID() == "RMCP01" then frameaddress = 0x9C38C2
  elseif getGameID() == "RMCE01" then frameaddress = 0x9BF0BA
  elseif getGameID() == "RMCJ01" then frameaddress = 0x9C2922
  elseif getGameID() == "RMCK01" then frameaddress = 0x9B1F02
  end
  return ReadValue16(frameaddress)
end
core.getFrameOfInput = getFrameOfInput

return core 
