local core = {}

local function getGameID()
    local address = 0x0
    return ReadValueString(address, 6)
end
core.getGameID = getGameID

--Values from the playerbase
local function getVehicleSpeed()
  local playerbase = 0
  if getGameID() == "RMCP01" then playerbase = 0x9C18F8
  elseif getGameID() == "RMCE01" then playerbase = 0x9BD110
  elseif getGameID() == "RMCJ01" then playerbase = 0x9C0958
  elseif getGameID() == "RMCK01" then playerbase = 0x9AFF38
  end
  local offset1 = 0x20
  local offset2 = 0x0
  local offset3 = 0x10
  local offset4 = 0x10
  local offset5 = 0x20
  local address1 = GetPointerNormal(playerbase)
  local address2 = GetPointerNormal(address1 + offset1)
  local address3 = GetPointerNormal(address2 + offset2)
  local address4 = GetPointerNormal(address3 + offset3)
  local address5 = GetPointerNormal(address4 + offset4)
  return ReadValueFloat(address5 + offset5)
end
core.getVehicleSpeed = getVehicleSpeed

local function getMTCharge()
  local playerbase = 0
  if getGameID() == "RMCP01" then playerbase = 0x9C18F8
  elseif getGameID() == "RMCE01" then playerbase = 0x9BD110
  elseif getGameID() == "RMCJ01" then playerbase = 0x9C0958
  elseif getGameID() == "RMCK01" then playerbase = 0x9AFF38
  end
  local offset1 = 0x20
  local offset2 = 0x0
  local offset3 = 0x10
  local offset4 = 0x10
  local offset5 = 0xFE
  local address1 = GetPointerNormal(playerbase)
  local address2 = GetPointerNormal(address1 + offset1)
  local address3 = GetPointerNormal(address2 + offset2)
  local address4 = GetPointerNormal(address3 + offset3)
  local address5 = GetPointerNormal(address4 + offset4)
  return ReadValue16(address5 + offset5)
end
core.getMTCharge = getMTCharge

local function getOrangeMTCharge()
  local playerbase = 0
  if getGameID() == "RMCP01" then playerbase = 0x9C18F8
  elseif getGameID() == "RMCE01" then playerbase = 0x9BD110
  elseif getGameID() == "RMCJ01" then playerbase = 0x9C0958
  elseif getGameID() == "RMCK01" then playerbase = 0x9AFF38
  end
  local offset1 = 0x20
  local offset2 = 0x0
  local offset3 = 0x10
  local offset4 = 0x10
  local offset5 = 0x100
  local address1 = GetPointerNormal(playerbase)
  local address2 = GetPointerNormal(address1 + offset1)
  local address3 = GetPointerNormal(address2 + offset2)
  local address4 = GetPointerNormal(address3 + offset3)
  local address5 = GetPointerNormal(address4 + offset4)
  return ReadValue16(address5 + offset5)
end
core.getOrangeMTCharge = getOrangeMTCharge

local function getSSMTCharge()
  local playerbase = 0
  if getGameID() == "RMCP01" then playerbase = 0x9C18F8
  elseif getGameID() == "RMCE01" then playerbase = 0x9BD110
  elseif getGameID() == "RMCJ01" then playerbase = 0x9C0958
  elseif getGameID() == "RMCK01" then playerbase = 0x9AFF38
  end
  local offset1 = 0x20
  local offset2 = 0x0
  local offset3 = 0x10
  local offset4 = 0x10
  local offset5 = 0x14D
  local address1 = GetPointerNormal(playerbase)
  local address2 = GetPointerNormal(address1 + offset1)
  local address3 = GetPointerNormal(address2 + offset2)
  local address4 = GetPointerNormal(address3 + offset3)
  local address5 = GetPointerNormal(address4 + offset4)
  return ReadValue8(address5 + offset5)
end
core.getSSMTCharge = getSSMTCharge

local function getMTBoost()
  local playerbase = 0
  if getGameID() == "RMCP01" then playerbase = 0x9C18F8
  elseif getGameID() == "RMCE01" then playerbase = 0x9BD110
  elseif getGameID() == "RMCJ01" then playerbase = 0x9C0958
  elseif getGameID() == "RMCK01" then playerbase = 0x9AFF38
  end
  local offset1 = 0x20
  local offset2 = 0x0
  local offset3 = 0x10
  local offset4 = 0x10
  local offset5 = 0x10D
  local address1 = GetPointerNormal(playerbase)
  local address2 = GetPointerNormal(address1 + offset1)
  local address3 = GetPointerNormal(address2 + offset2)
  local address4 = GetPointerNormal(address3 + offset3)
  local address5 = GetPointerNormal(address4 + offset4)
  return ReadValue8(address5 + offset5)
end
core.getMTBoost = getMTBoost

local function getLapCompleted()
  local raceData = 0
  if getGameID() == "RMCP01" then raceData = 0x9BD730
  elseif getGameID() == "RMCE01" then raceData = 0x9B8F70
  end
  local offset1 = 0xAC
  local offset2 = 0xC
  local address1 = GetPointerNormal(raceData)
  local address2 = GetPointerNormal(address1 + offset1)
  return ReadValueFloat(address2 + offset2)
end
  core.getLapCompleted = getLapCompleted

local function getTrickBoost()
  local playerbase = 0
  if getGameID() == "RMCP01" then playerbase = 0x9C18F8
  elseif getGameID() == "RMCE01" then playerbase = 0x9BD110
  elseif getGameID() == "RMCJ01" then playerbase = 0x9C0958
  elseif getGameID() == "RMCK01" then playerbase = 0x9AFF38
  end
  local offset1 = 0x20
  local offset2 = 0x0
  local offset3 = 0x10
  local offset4 = 0x10
  local offset5 = 0x115
  local address1 = GetPointerNormal(playerbase)
  local address2 = GetPointerNormal(address1 + offset1)
  local address3 = GetPointerNormal(address2 + offset2)
  local address4 = GetPointerNormal(address3 + offset3)
  local address5 = GetPointerNormal(address4 + offset4)
  return ReadValue8(address5 + offset5)
end
core.getTrickBoost = getTrickBoost

local function getMushroomBoost()
  local playerbase = 0
  if getGameID() == "RMCP01" then playerbase = 0x9C18F8
  elseif getGameID() == "RMCE01" then playerbase = 0x9BD110
  elseif getGameID() == "RMCJ01" then playerbase = 0x9C0958
  elseif getGameID() == "RMCK01" then playerbase = 0x9AFF38
  end
  local offset1 = 0x20
  local offset2 = 0x0
  local offset3 = 0x10
  local offset4 = 0x10
  local offset5 = 0x148
  local address1 = GetPointerNormal(playerbase)
  local address2 = GetPointerNormal(address1 + offset1)
  local address3 = GetPointerNormal(address2 + offset2)
  local address4 = GetPointerNormal(address3 + offset3)
  local address5 = GetPointerNormal(address4 + offset4)
  return ReadValue8(address5 + offset5)
end
core.getMushroomBoost = getMushroomBoost

local function getAirTime()
  local playerbase = 0
  if getGameID() == "RMCP01" then playerbase = 0x9C18F8
  elseif getGameID() == "RMCE01" then playerbase = 0x9BD110
  elseif getGameID() == "RMCJ01" then playerbase = 0x9C0958
  elseif getGameID() == "RMCK01" then playerbase = 0x9AFF38
  end
  local offset1 = 0x20
  local offset2 = 0x0
  local offset3 = 0x10
  local offset4 = 0x10
  local offset5 = 0x21A
  local address1 = GetPointerNormal(playerbase)
  local address2 = GetPointerNormal(address1 + offset1)
  local address3 = GetPointerNormal(address2 + offset2)
  local address4 = GetPointerNormal(address3 + offset3)
  local address5 = GetPointerNormal(address4 + offset4)
  return ReadValue16(address5 + offset5)
end
core.getAirTime = getAirTime

--Positions
local function getXpos()
  local pointer = 0x9C2EF8
  local offset1 = 0x40
  local offset2 = 0x0
  local address1 = GetPointerNormal(pointer)
  local address2 = GetPointerNormal(address1 + offset1)
  return ReadValueFloat(address2 + offset2)
end
core.getXpos = getXpos

local function getPrevXpos()
  local pointer = 0x9C2EF8
  local offset1 = 0x40
  local offset2 = -0x160
  local address1 = GetPointerNormal(pointer)
  local address2 = GetPointerNormal(address1 + offset1)
  return ReadValueFloat(address2 + offset2)
end
core.getPrevXpos = getPrevXpos

local function getYpos()
  local pointer = 0x9C2EF8
  local offset1 = 0x40
  local offset2 = 0x4
  local address1 = GetPointerNormal(pointer)
  local address2 = GetPointerNormal(address1 + offset1)
  return ReadValueFloat(address2 + offset2)
end
core.getYpos = getYpos

local function getPrevYpos()
  local pointer = 0x9C2EF8
  local offset1 = 0x40
  local offset2 = -0x15C
  local address1 = GetPointerNormal(pointer)
  local address2 = GetPointerNormal(address1 + offset1)
  return ReadValueFloat(address2 + offset2)
end
core.getPrevYpos = getPrevYpos

local function getZpos()
  local pointer = 0x9C2EF8
  local offset1 = 0x40
  local offset2 = 0x8
  local address1 = GetPointerNormal(pointer)
  local address2 = GetPointerNormal(address1 + offset1)
  return ReadValueFloat(address2 + offset2)
end
core.getZpos = getZpos

local function getPrevZpos()
  local pointer = 0x9C2EF8
  local offset1 = 0x40
  local offset2 = -0x158
  local address1 = GetPointerNormal(pointer)
  local address2 = GetPointerNormal(address1 + offset1)
  return ReadValueFloat(address2 + offset2)
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

--Inputs
local function getButtonInput()
  local pointer = 0
  if getGameID() == "RMCP01" then pointer = 0x42E324
  elseif getGameID() == "RMCE01" then pointer = 0x429F14
  elseif getGameID() == "RMCJ01" then pointer = 0x42DC14
  elseif getGameID() == "RMCK01" then pointer = 0x41C2B4
  end
  local offset = 0x2841
  local address = GetPointerNormal(pointer)
  return ReadValue8(address + offset)
end
core.getButtonInput = getButtonInput

local function getDPADInput()
  local pointer = 0
  if getGameID() == "RMCP01" then pointer = 0x42E324
  elseif getGameID() == "RMCE01" then pointer = 0x429F14
  elseif getGameID() == "RMCJ01" then pointer = 0x42DC14
  elseif getGameID() == "RMCK01" then pointer = 0x41C2B4
  end
  local offset = 0x284F
  local address = GetPointerNormal(pointer)
  return ReadValue8(address + offset)
end
core.getDPADInput = getDPADInput

local function getHzInput()
  local pointer = 0
  if getGameID() == "RMCP01" then pointer = 0x42E324
  elseif getGameID() == "RMCE01" then pointer = 0x429F14
  elseif getGameID() == "RMCJ01" then pointer = 0x42DC14
  elseif getGameID() == "RMCK01" then pointer = 0x41C2B4
  end
  local offset = 0x284C
  local address = GetPointerNormal(pointer)
  return ReadValue8(address + offset)
end
core.getHzInput = getHzInput

local function getVerticalInput()
  local pointer = 0
  if getGameID() == "RMCP01" then pointer = 0x42E324
  elseif getGameID() == "RMCE01" then pointer = 0x429F14
  elseif getGameID() == "RMCJ01" then pointer = 0x42DC14
  elseif getGameID() == "RMCK01" then pointer = 0x41C2B4
  end
  local offset = 0x284D
  local address = GetPointerNormal(pointer)
  return ReadValue8(address + offset)
end
core.getVerticalInput = getVerticalInput

--FrameCounter in Race
local function getFrameOfinput()
  local frameaddress = 0x0
  if getGameID() == "RMCP01" then frameaddress = 0x9C38C2
  elseif getGameID() == "RMCE01" then frameaddress = 0x9BF0BA
  elseif getGameID() == "RMCJ01" then frameaddress = 0x9C2922
  elseif getGameID() == "RMCK01" then frameaddress = 0x9B1F02
  end
  return ReadValue16(frameaddress)
end
core.getFrameOfinput = getFrameOfinput

return core
