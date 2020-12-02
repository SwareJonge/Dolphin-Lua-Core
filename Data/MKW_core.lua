local core = {}

local Pointers = require "MKW_Pointers"

local function getPos()
  local address = Pointers.getPositionPointer(0x0) -- 0x0 first player in the array, to get the most accurate, read playerindex first
  if address == 0 then
    return {X = 0, Y = 0, Z = 0}
  end
  return {X = ReadValueFloat(address), Y = ReadValueFloat(address + 0x4), Z = ReadValueFloat(address + 0x8)}
end
core.getPos = getPos

local function getPosGhost()
  local address = Pointers.getPositionPointer(0x4)
  if address == 0 then
    return {X = 0, Y = 0, Z = 0}
  end
  return {X = ReadValueFloat(address), Y = ReadValueFloat(address + 0x4), Z = ReadValueFloat(address + 0x8)}
end
core.getPosGhost = getPosGhost

local function getPrevPos() -- TODO rewrite this function but with accurate previous position
  local address = Pointers.getPositionPointer(0x0)
  local prevOffset = 0x160
  if address == 0 then
    return {X = 0, Y = 0, Z = 0}
  end
  return {X = ReadValueFloat(address - prevOffset), Y = ReadValueFloat((address - prevOffset) + 0x4), Z = ReadValueFloat((address - prevOffset) + 0x8)}
end
core.getPrevPos = getPrevPos

local function getSpd()
  local PrevXpos = getPrevPos().X
  local PrevYpos = getPrevPos().Y
  local PrevZpos = getPrevPos().Z
  local Xpos = getPos().X
  local Ypos = getPos().Y
  local Zpos = getPos().Z
  return {X = (Xpos - PrevXpos), Y = (Ypos - PrevYpos), Z = (Zpos - PrevZpos), XZ = math.sqrt(((Xpos - PrevXpos)^2) + (Zpos - PrevZpos)^2), XYZ = math.sqrt(((Xpos - PrevXpos)^2) + ((Ypos - PrevYpos)^2) + (Zpos - PrevZpos)^2)}
end
core.getSpd = getSpd

local function getInput()
  local address = Pointers.getInputPointer(0x0) -- change this to 0x4 for ghost
  local offset = 0x8 -- too lazy to adjust the values beneath...

  if address == 0 then return {ABLR = 0, X = 0, Y = 0, DPAD = 0}
  else return {
   ABLR = ReadValue8(address + offset + 0x1),
   X = ReadValue8(address + offset + 0xC),
   Y = ReadValue8(address + offset + 0xD),
   DPAD = ReadValue8(address + offset + 0xF)}
 end
end
core.getInput = getInput

local function PosToAngle()
  local Xspd = getSpd().X
  if getSpd().X == 0 then Xspd = math.pi / 2
  end
  local angle = math.atan(getSpd().Z * Xspd)
  local finalAngle = angle * 65536 % 360
  local sine = math.sin(finalAngle)
  return {A = finalAngle, S = sine}
end
core.PosToAngle = PosToAngle

--FrameCounter in Race
local function getFrameOfInput()
  return ReadValue32(Pointers.getFrameOfInputAddress())
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
  local offset2 = 0x88
  local address2 = Pointers.getPositionPointer(0x0)
  if(address2 == 0) then
    return {X = 0, Y = 0, Z = 0, W = 0}
  end
	return {X = ReadValueFloat(address2 + offset2), Y = ReadValueFloat(address2 + offset2 + 4),
			Z = ReadValueFloat(address2 + offset2 + 8), W = ReadValueFloat(address2 + offset2 + 12)}
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
  local test= qx*qy + qz*qw
  if (test > 0.499) then
    Yvalue = 360/math.pi*math_atan2(qx,qw)
    Zvalue = 90
    Xvalue = 0
    return {X = Xvalue, Y = Yvalue, Z = Zvalue}
  end
  if (test < -0.499) then
    Yvalue = -360/math.pi*math_atan2(qx,qw)
    Zvalue = -90
    Xvalue = 0
    return {X = Xvalue, Y = Yvalue, Z = Zvalue}
  end
  local h = math_atan2(2*qy*qw-2*qx*qz, 1-2*qy2-2*qz2)
  local a = math.asin(2*qx*qy + 2*qz*qw)
  local b = math_atan2(2*qx*qw-2*qy*qz, 1-2*qx2-2*qz2)
  Yvalue = (h*180/math.pi)
  Zvalue = (a*180/math.pi)
  Xvalue = (b*180/math.pi)
  return {X = (Xvalue - 90) % 360, Y = (Yvalue - 90) % 360, Z = Zvalue}
end
core.calculateEuler = calculateEuler

return core
