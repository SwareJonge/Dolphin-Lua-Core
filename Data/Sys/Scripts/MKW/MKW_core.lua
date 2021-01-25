local core = {}

package.path = GetScriptsDir() .. "MKW/MKW_Pointers.lua"
local Pointers = require("MKW_Pointers")

local function getPos()
  local address = Pointers.getPositionPointer(0x0) -- 0x0 first player in the array, to get the most accurate, read playerindex first
  if address == 0 then
    return {X = 0, Y = 0, Z = 0}
  end
  return {X = ReadValueFloat(address, 0x68), Y = ReadValueFloat(address, 0x6C), Z = ReadValueFloat(address, 0x70)}
end
core.getPos = getPos

local function getPosGhost()
  local address = Pointers.getPositionPointer(0x4)
  if address == 0 then
    return {X = 0, Y = 0, Z = 0}
  end
  return {X = ReadValueFloat(address, 0x68), Y = ReadValueFloat(address, 0x6C), Z = ReadValueFloat(address, 0x70)}
end
core.getPosGhost = getPosGhost

local function getPrevPos()
  local address = Pointers.getPrevPositionPointer(0x0)
  if address == 0 then
    return {X = 0, Y = 0, Z = 0}
  end
  return {X = ReadValueFloat(address, 0x18), Y = ReadValueFloat(address, 0x1C), Z = ReadValueFloat(address, 0x20)}
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

local function calculateDirectX()
	local speed_length = 0
	local speed_width = 0
	local cardinal = 0
	local facing_angle = 0
	local facing_rotation = 0
	local py = core.getPos().Y
	local pz = core.getPos().Z
	local prevy = core.getPrevPos().Y
	local prevz = core.getPrevPos().Z

	speed_length = pz - prevz
	speed_width = math.abs(py - prevy)
	cardinal = 0
	facing_angle = math.atan(speed_width/speed_length)
	facing_rotation = facing_angle + cardinal

	if facing_rotation >= 0 and facing_rotation < 90 then
		return facing_rotation
	else
		speed_length = prevy - py
		speed_width = math.abs(pz - prevz)
		cardinal = 90
		facing_angle = math.atan(speed_width/speed_length)
		facing_rotation = facing_angle + cardinal
	end

	if facing_rotation >= 90 and facing_rotation < 180 then
		return facing_rotation
	else
		speed_length = prevz - pz
		speed_width = math.abs(py - prevy)
		cardinal = 180
		facing_angle = math.atan(speed_width/speed_length)
		facing_rotation = facing_angle + cardinal
	end

	if facing_rotation >= 180 and facing_rotation < 270 then
		return facing_rotation
	else
		speed_length = py - prevy
		speed_width = math.abs(pz - prevz)
		cardinal = 270
		facing_angle = math.atan(speed_width/speed_length)
		facing_rotation = facing_angle + cardinal
		return facing_rotation
	end
end

core.calculateDirectX = calculateDirectX

local function calculateDirectY()
	local speed_length = 0
	local speed_width = 0
	local cardinal = 0
	local facing_angle = 0
	local facing_rotation = 0
	local px = core.getPos().X
	local pz = core.getPos().Z
	local prevx = core.getPrevPos().X
	local prevz = core.getPrevPos().Z

	speed_length = pz - prevz
	speed_width = math.abs(px - prevx)
	cardinal = 0
	facing_angle = math.atan(speed_width/speed_length)
	facing_rotation = facing_angle + cardinal

	if facing_rotation >= 0 and facing_rotation < 90 then
		return facing_rotation
	else
		speed_length = prevx - px
		speed_width = math.abs(pz - prevz)
		cardinal = 90
		facing_angle = math.atan(speed_width/speed_length)
		facing_rotation = facing_angle + cardinal
	end

	if facing_rotation >= 90 and facing_rotation < 180 then
		return facing_rotation
	else
		speed_length = prevz - pz
		speed_width = math.abs(px - prevx)
		cardinal = 180
		facing_angle = math.atan(speed_width/speed_length)
		facing_rotation = facing_angle + cardinal
	end

	if facing_rotation >= 180 and facing_rotation < 270 then
		return facing_rotation
	else
		speed_length = px - prevx
		speed_width = math.abs(pz - prevz)
		cardinal = 270
		facing_angle = math.atan(speed_width/speed_length)
		facing_rotation = facing_angle + cardinal
		return facing_rotation
	end
end

core.calculateDirectY = calculateDirectY

local function calculateDirectZ()
	local speed_length = 0
	local speed_width = 0
	local cardinal = 0
	local facing_angle = 0
	local facing_rotation = 0
	local px = core.getPos().X
	local py = core.getPos().Y
	local prevx = core.getPrevPos().X
	local prevy = core.getPrevPos().Y

	speed_length = py - prevy
	speed_width = math.abs(px - prevx)
	cardinal = 0
	facing_angle = math.atan(speed_width/speed_length)
	facing_rotation = facing_angle + cardinal

	if facing_rotation >= 0 and facing_rotation < 90 then
		return facing_rotation
	else
		speed_length = prevx - px
		speed_width = math.abs(py - prevy)
		cardinal = 90
		facing_angle = math.atan(speed_width/speed_length)
		facing_rotation = facing_angle + cardinal
	end

	if facing_rotation >= 90 and facing_rotation < 180 then
		return facing_rotation
	else
		speed_length = prevy - py
		speed_width = math.abs(px - prevx)
		cardinal = 180
		facing_angle = math.atan(speed_width/speed_length)
		facing_rotation = facing_angle + cardinal
	end

	if facing_rotation >= 180 and facing_rotation < 270 then
		return facing_rotation
	else
		speed_length = px - prevx
		speed_width = math.abs(py - prevy)
		cardinal = 270
		facing_angle = math.atan(speed_width/speed_length)
		facing_rotation = facing_angle + cardinal
		return facing_rotation
	end
end

core.calculateDirectZ = calculateDirectZ

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
  local offset2 = 0xF0
  local address2 = Pointers.getPositionPointer(0x0)
  if(address2 == 0) then
    return {X = 0, Y = 0, Z = 0, W = 0}
  end
	return {X = ReadValueFloat(address2, 0xF0), Y = ReadValueFloat(address2, 0xF4),
			Z = ReadValueFloat(address2, 0xF8), W = ReadValueFloat(address2, 0xFC)}
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

local function isSinglePlayer()
  local address = Pointers.getPositionPointer(0x4)
  if address == 0 then return true
  else return false
  end
end
core.isSinglePlayer = isSinglePlayer

local function getDifference()
	local xz = getSpd().XZ
	local player_rotation = calculateEuler().Y
	local px = getPos().X
	local pz = getPos().Z
	local gx = getPosGhost().X
	local gz = getPosGhost().Z
	local prevx = getPrevPos().X
	local prevz = getPrevPos().Z
	local new_rotation = calculateDirectY()

	rotation_id = 0

	if new_rotation >= 0 and new_rotation <  90 then rotation_id = 0
	elseif new_rotation >= 90 and new_rotation < 180 then rotation_id = 1
	elseif new_rotation >= 180 and new_rotation < 270 then rotation_id = 2
	elseif new_rotation >= 270 and new_rotation < 360 then rotation_id = 3
	end

	if rotation_id == 0 then
		length = pz - gz
		width = math.abs(gx - px)
	elseif rotation_id == 1 then
		length = gx - px
		width = math.abs(gz - pz)
	elseif rotation_id == 2 then
		length = gz - pz
		width = math.abs(px - gx)
	elseif rotation_id == 3 then
		length = px - gx
		width = math.abs(pz - gz)
	end

	angle_right = player_rotation - (90*rotation_id)
	angle = math.atan(width/length)
	hypotenuse = math.sqrt(length^2 + width^2)
	angle_left = math.abs(angle - angle_right)
	distance = math.cos(angle_left)*hypotenuse

	return ((distance/xz)/(60))
end
core.getDifference = getDifference

local function getFinishDifference()
    local length = 0
	local zspd = getSpd().Z
	local zpos = getPos().Z
	local ghost_zpos = getPosGhost().Z

    length = zpos - ghost_zpos

    return math.abs((length/zspd)/(60))
end
core.getFinishDifference = getFinishDifference


return core
