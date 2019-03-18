local core = {}

local function getGameID()
    local address = 0x0
    return ReadValueString(address, 6)
end
core.getGameID = getGameID

local function getPos()
  local address = 0
  if getGameID() == "RMGE01" then address = 0xF8EF88
  elseif getGameID() == "RMGJ01" then address = 0xF8F328
  elseif getGameID() == "RMGP01" then address = 0xF8EF88
  end
  local offset1 = 0x3EEC
  local address1 = GetPointerNormal(address)
  if ReadValue32(address) == 0 then
		return {X = 0, Y = 0, Z = 0}
  end
  return {X = ReadValueFloat(address1 + offset1), Y = ReadValueFloat(address1 + offset1 + 4), Z = ReadValueFloat(address1 + offset1 + 8)}
end
core.getPos = getPos

local function getPrevPos()
  local address = 0
  if getGameID() == "RMGE01" then address = 0xF8EF88
  elseif getGameID() == "RMGJ01" then address = 0xF8F328
  elseif getGameID() == "RMGP01" then address = 0xF8EF88
  end
  local offset1 = 0x18DC
  local address1 = GetPointerNormal(address)
  if ReadValue32(address) == 0 then
		return {X = 0, Y = 0, Z = 0}
  end
  return {X = ReadValueFloat(address1 + offset1), Y = ReadValueFloat(address1 + offset1 + 4), Z = ReadValueFloat(address1 + offset1 + 8)}
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

return core
