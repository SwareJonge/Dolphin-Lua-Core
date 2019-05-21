local core = {}

local function getGameID()
    return ReadValueString(0x0, 6)
end
core.getGameID = getGameID

local function getPos()
  local pointer
  if getGameID() == "RMCP01" then pointer = 0x9C2EF8
  elseif getGameID() == "RMCE01"then pointer = 0x9BE738
  elseif getGameID() == "RMCJ01" then pointer = 0x9C1F58
  elseif getGameID() == "RMCK01" then pointer = 0x9B1538
  end
  local offset1 = 0x40
  local address1 = GetPointerNormal(pointer)
  if ReadValue32(address1) >= 0x80000000 then
  local address2 = GetPointerNormal(address1 + offset1)
  local offset2 = 0x0
  return {X = ReadValueFloat(address2 + offset2), Y = ReadValueFloat(address2 + offset2 + 4), Z = ReadValueFloat(address2 + offset2 + 8)}
  else return {X = 0, Y = 0, Z = 0}
  end
end
core.getPos = getPos

local function getPosGhost()
  local pointer
  if getGameID() == "RMCP01" then pointer = 0x9C2EF8
  elseif getGameID() == "RMCE01"then pointer = 0x9BE738
  elseif getGameID() == "RMCJ01" then pointer = 0x9C1F58
  elseif getGameID() == "RMCK01" then pointer = 0x9B1538
  end
  local offset1 = 0x64
  local address1 = GetPointerNormal(pointer)
  if ReadValue32(address1) >= 0x80000000 then
  local address2 = GetPointerNormal(address1 + offset1)
  local offset2 = 0x0
  return {X = ReadValueFloat(address2 + offset2), Y = ReadValueFloat(address2 + offset2 + 4), Z = ReadValueFloat(address2 + offset2 + 8)}
  else return {X = 0, Y = 0, Z = 0}
  end
end
core.getPosGhost = getPosGhost

local function getPrevPos()
  local pointer
  if getGameID() == "RMCP01"then pointer = 0x9C2EF8
  elseif getGameID() == "RMCE01"then pointer = 0x9BE738
  elseif getGameID() == "RMCJ01"then pointer = 0x9C1F58
  elseif getGameID() == "RMCK01" then pointer = 0x9B1538
  end
  local offset1 = 0x40
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
  return {X = (Xpos - PrevXpos),
  Y = (Ypos - PrevYpos),
  Z = (Zpos - PrevZpos),
  XZ = math.sqrt(((Xpos - PrevXpos)^2) + (Zpos - PrevZpos)^2),
  XYZ = math.sqrt(((Xpos - PrevXpos)^2) + ((Ypos - PrevYpos)^2) + (Zpos - PrevZpos)^2)}
end
core.getSpd = getSpd

local function getInput()
  local pointer
  if getGameID() == "RMCP01" then pointer = 0x42E324
  elseif getGameID() == "RMCE01" then pointer = 0x429FA4
  elseif getGameID() == "RMCJ01" then pointer = 0x42DCA4
  elseif getGameID() == "RMCK01" then pointer = 0x41C2B4
  end
  if ReadValue32(pointer) >= 0x80000000 then
    local offset = 0x2840
    local address = GetPointerNormal(pointer)
		return {
     ABLR = ReadValue8(address + offset + 0x1),
     X = ReadValue8(address + offset + 0xC),
     Y = ReadValue8(address + offset + 0xD),
     DPAD = ReadValue8(address + offset + 0xF)}
	else return {ABLR = 0, X = 0, Y = 0, DPAD = 0}
  end
end
core.getInput = getInput

local function getDistGhost()
  return {X = (32797 - getPos().X), Y = (5133 - getPos().Y), Z = (974 - getPos().Z)}
end
core.getDistGhost = getDistGhost

--FrameCounter in Race
local function getFrameOfInput()
  local frameaddress
  if getGameID() == "RMCP01" then frameaddress = 0x9C38C2
  elseif getGameID() == "RMCE01" then frameaddress = 0x9BF0BA
  elseif getGameID() == "RMCJ01" then frameaddress = 0x9C2922
  elseif getGameID() == "RMCK01" then frameaddress = 0x9B1F02
  end
  return ReadValue16(frameaddress)
end
core.getFrameOfInput = getFrameOfInput

function math_atan2(x, y)
	local angle = math.asin( y / math.sqrt(x^2 + y^2 ) )

	if x < 0 then
		angle = math.pi - angle
	elseif y < 0 then
		angle = 2*math.pi + angle
	end

	return angle
end
core.math_atan2 = math_atan2

function getQuaternion()
  local pointer
  if getGameID() == "RMCP01" then pointer = 0x9C2EF8
  elseif getGameID() == "RMCE01"then pointer = 0x9BE738
  elseif getGameID() == "RMCJ01" then pointer = 0x9C1F58
  elseif getGameID() == "RMCK01" then pointer = 0x9B1538
  end
	local offset1 = 0x40
	local address1 = GetPointerNormal(pointer)
  if ReadValue32(address1) >= 0x80000000 then
	local address2 = GetPointerNormal(address1 + offset1)
	local offset2 = 0x88
	return {X = ReadValueFloat(address2 + offset2), Y = ReadValueFloat(address2 + offset2 + 4),
			Z = ReadValueFloat(address2 + offset2 + 8), W = ReadValueFloat(address2 + offset2 + 12)}
  else return {X = 0, Y = 0, Z = 0, W = 0}
  end
end
core.getQuaternion = getQuaternion

local function calculateEuler()
  local qw = getQuaternion().W
  local qx = getQuaternion().X
  local qy = getQuaternion().Y
  local qz = getQuaternion().Z
  local qw2 = qw*qw
  local qx2 = qx*qx
  local qy2 = qy*qy
  local qz2 = qz*qz
  local h = math_atan2(2*qy*qw-2*qx*qz, 1-2*qy2-2*qz2)
  local a = math.asin(2*qx*qy + 2*qz*qw)
  local b = math_atan2(2*qx*qw-2*qy*qz, 1-2*qx2-2*qz2)
  Yvalue = (h*180/math.pi)
  Zvalue = (a*180/math.pi)
  Xvalue = (b*180/math.pi)
  return {Y = (Yvalue + 90) %360, X = (Xvalue - 90) %360, Z = Zvalue}
end
core.calculateEuler = calculateEuler

return core
