local core = {}

local function getGameID()
    local address = 0x0
    return ReadValueString(address, 6)
end
core.getGameID = getGameID

local function getXpos()
  local address = 0

  if getGameID() == "RMGE01" then address = 0xF8EF88
  elseif getGameID() == "RMGJ01" then address = 0xF8F328
  elseif getGameID() == "RMGP01" then address = 0xF8EF88
  end
  local offset1 = 0x3EEC
  local address1 = GetPointerNormal(address)
  return ReadValueFloat(address1 + offset1)
end
core.getXpos = getXpos

local function getYpos()
  local address = 0

  if getGameID() == "RMGE01" then address = 0xF8EF88
  elseif getGameID() == "RMGJ01" then address = 0xF8F328
  elseif getGameID() == "RMGP01" then address = 0xF8EF88
  end
  local offset1 = 0x3EF0
  local address1 = GetPointerNormal(address)
  return ReadValueFloat(address1 + offset1)
end
core.getYpos = getYpos

local function getZpos()
  local address = 0

  if getGameID() == "RMGE01" then address = 0xF8EF88
  elseif getGameID() == "RMGJ01" then address = 0xF8F328
  elseif getGameID() == "RMGP01" then address = 0xF8EF88
  end
  local offset1 = 0x3EF4
  local address1 = GetPointerNormal(address)
  return ReadValueFloat(address1 + offset1)
end
core.getYpos = getYpos

local function getPrevXpos()
  local address = 0
  if getGameID() == "RMGE01" then address = 0xF8EF88
  elseif getGameID() == "RMGJ01" then address = 0xF8F328
  elseif getGameID() == "RMGP01" then address = 0xF8EF88
  end
  local offset1 = 0x18DC
  local address1 = GetPointerNormal(address)
  return ReadValueFloat(address1 + offset1)
end
core.getPrevXpos = getPrevXpos

local function getPrevYpos()
  local address = 0

  if getGameID() == "RMGE01" then address = 0xF8EF88
  elseif getGameID() == "RMGJ01" then address = 0xF8F328
  elseif getGameID() == "RMGP01" then address = 0xF8EF88
  end
  local offset1 = 0x18E0
  local address1 = GetPointerNormal(address)
  return ReadValueFloat(address1 + offset1)
end
core.getPrevYpos = getPrevYpos

local function getPrevZpos()
  local address = 0

  if getGameID() == "RMGE01" then address = 0xF8EF88
  elseif getGameID() == "RMGJ01" then address = 0xF8F328
  elseif getGameID() == "RMGP01" then address = 0xF8EF88
  end
  local offset1 = 0x18E4
  local address1 = GetPointerNormal(address)
  return ReadValueFloat(address1 + offset1)
end
core.getPrevZpos = getPrevZpos

local function getXYZSpeed()
  local PrevXpos = getPrevXpos()
  local PrevYpos = getPrevYpos()
  local PrevZpos = getPrevZpos()
  local Xpos = getXpos()
  local Ypos = getYpos()
  local Zpos = getZpos()
  local XYZSpeed = math.sqrt(((Xpos - PrevXpos)^2) + ((Ypos - PrevYpos)^2) + (Zpos - PrevZpos)^2)
  return XYZSpeed
end
core.getXYZSpeed = getXYZSpeed

local function getXZSpeed()
  local PrevXpos = getPrevXpos()
  local PrevZpos = getPrevZpos()
  local Xpos = getXpos()
  local Zpos = getZpos()
  local XZSpeed = math.sqrt(((Xpos - PrevXpos)^2) + (Zpos - PrevZpos)^2)
  return XZSpeed
end
core.getXZSpeed = getXZSpeed

local function getYVel()
  local PrevYpos = getPrevYpos()
  local Ypos = getYpos()
  local YVel = Ypos - PrevYpos
  return YVel
end
core.getYVel = getYVel

return core
