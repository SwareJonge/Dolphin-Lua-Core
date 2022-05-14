local core = {}

package.path = GetScriptsDir() .. "MKW/MKW_Pointers.lua"
local Pointers = require("MKW_Pointers")

package.path = GetScriptsDir() .. "MKW/Math_core.lua"
local math_core = require("Math_core")

local function getCourseSlot()
  local address = Pointers.getRaceDataPointer()
  if ReadValue32(address, 0xb68) == 0 then return "MC"
  elseif ReadValue32(address, 0xb68) == 1 then return "MMM"
  elseif ReadValue32(address, 0xb68) == 2 then return "MG"
  elseif ReadValue32(address, 0xb68) == 3 then return "GV"
  elseif ReadValue32(address, 0xb68) == 4 then return "TF"
  elseif ReadValue32(address, 0xb68) == 5 then return "CM"
  elseif ReadValue32(address, 0xb68) == 6 then return "DKS"
  elseif ReadValue32(address, 0xb68) == 7 then return "WGM"
  elseif ReadValue32(address, 0xb68) == 8 then return "LC"
  elseif ReadValue32(address, 0xb68) == 9 then return "DC"
  elseif ReadValue32(address, 0xb68) == 10 then return "MH"
  elseif ReadValue32(address, 0xb68) == 11 then return "MT"
  elseif ReadValue32(address, 0xb68) == 12 then return "BC"
  elseif ReadValue32(address, 0xb68) == 13 then return "RR"
  elseif ReadValue32(address, 0xb68) == 14 then return "DDR"
  elseif ReadValue32(address, 0xb68) == 15 then return "KC"
  elseif ReadValue32(address, 0xb68) == 16 then return "rPB"
  elseif ReadValue32(address, 0xb68) == 17 then return "rMC"
  elseif ReadValue32(address, 0xb68) == 18 then return "rWS"
  elseif ReadValue32(address, 0xb68) == 19 then return "rDKM"
  elseif ReadValue32(address, 0xb68) == 20 then return "rYF"
  elseif ReadValue32(address, 0xb68) == 21 then return "rDS"
  elseif ReadValue32(address, 0xb68) == 22 then return "rPG"
  elseif ReadValue32(address, 0xb68) == 23 then return "rDS"
  elseif ReadValue32(address, 0xb68) == 24 then return "rMC3"
  elseif ReadValue32(address, 0xb68) == 25 then return "rGV2"
  elseif ReadValue32(address, 0xb68) == 26 then return "rMR"
  elseif ReadValue32(address, 0xb68) == 27 then return "rSL"
  elseif ReadValue32(address, 0xb68) == 28 then return "rBC"
  elseif ReadValue32(address, 0xb68) == 29 then return "rDKJP"
  elseif ReadValue32(address, 0xb68) == 30 then return "rBC3"
  elseif ReadValue32(address, 0xb68) == 31 then return "rSGB"
  end
end
core.getCourseSlot = getCourseSlot

local function getCharacter()
  local address = Pointers.getRaceDataPointer()
  if ReadValue32(address, 0x34) == 0 then return "Mario"
  elseif ReadValue32(address, 0x34) == 1 then return "Baby Peach"
  elseif ReadValue32(address, 0x34) == 2 then return "Waluigi"
  elseif ReadValue32(address, 0x34) == 3 then return "Bowser"
  elseif ReadValue32(address, 0x34) == 4 then return "Baby Daisy"
  elseif ReadValue32(address, 0x34) == 5 then return "Dry Bones"
  elseif ReadValue32(address, 0x34) == 6 then return "Baby Mario"
  elseif ReadValue32(address, 0x34) == 7 then return "Luigi"
  elseif ReadValue32(address, 0x34) == 8 then return "Toad"
  elseif ReadValue32(address, 0x34) == 9 then return "Donkey Kong"
  elseif ReadValue32(address, 0x34) == 10 then return "Yoshi"
  elseif ReadValue32(address, 0x34) == 11 then return "Wario"
  elseif ReadValue32(address, 0x34) == 12 then return "Baby Luigi"
  elseif ReadValue32(address, 0x34) == 13 then return "Toadette"
  elseif ReadValue32(address, 0x34) == 14 then return "Koopa Troopa"
  elseif ReadValue32(address, 0x34) == 15 then return "Daisy"
  elseif ReadValue32(address, 0x34) == 16 then return "Peach"
  elseif ReadValue32(address, 0x34) == 17 then return "Birdo"
  elseif ReadValue32(address, 0x34) == 18 then return "Diddy Kong"
  elseif ReadValue32(address, 0x34) == 19 then return "King Boo"
  elseif ReadValue32(address, 0x34) == 20 then return "Bowser Jr."
  elseif ReadValue32(address, 0x34) == 21 then return "Dry Bowser"
  elseif ReadValue32(address, 0x34) == 22 then return "Funky Kong"
  elseif ReadValue32(address, 0x34) == 23 then return "Rosalina"
  elseif ReadValue32(address, 0x34) == 24 then return "Mii Outfit A (M | Light)"
  elseif ReadValue32(address, 0x34) == 25 then return "Mii Outfit A (F | Light)"
  elseif ReadValue32(address, 0x34) == 26 then return "Mii Outfit B (M | Light)"
  elseif ReadValue32(address, 0x34) == 27 then return "Mii Outfit B (F | Light)"
  elseif ReadValue32(address, 0x34) == 28 then return "Mii Outfit C (M | Light)"
  elseif ReadValue32(address, 0x34) == 29 then return "Mii Outfit C (F | Light)"
  elseif ReadValue32(address, 0x34) == 30 then return "Mii Outfit A (M | Medium)"
  elseif ReadValue32(address, 0x34) == 31 then return "Mii Outfit A (F | Medium)"
  elseif ReadValue32(address, 0x34) == 32 then return "Mii Outfit B (M | Medium)"
  elseif ReadValue32(address, 0x34) == 33 then return "Mii Outfit B (F | Medium)"
  elseif ReadValue32(address, 0x34) == 34 then return "Mii Outfit C (M | Medium)"
  elseif ReadValue32(address, 0x34) == 35 then return "Mii Outfit C (F | Medium)"
  elseif ReadValue32(address, 0x34) == 36 then return "Mii Outfit A (M | Heavy)"
  elseif ReadValue32(address, 0x34) == 37 then return "Mii Outfit A (F | Heavy)"
  elseif ReadValue32(address, 0x34) == 38 then return "Mii Outfit B (M | Heavy)"
  elseif ReadValue32(address, 0x34) == 39 then return "Mii Outfit B (F | Heavy)"
  elseif ReadValue32(address, 0x34) == 40 then return "Mii Outfit C (M | Heavy)"
  elseif ReadValue32(address, 0x34) == 41 then return "Mii Outfit C (F | Heavy)"
  end
end
core.getCharacter = getCharacter

local function getVehicle()
  local address = Pointers.getRaceDataPointer()
  if ReadValue32(address, 0x30) == 0 then return "Standard Kart S"
  elseif ReadValue32(address, 0x30) == 1 then return "Standard Kart M"
  elseif ReadValue32(address, 0x30) == 2 then return "Standard Kart L"
  elseif ReadValue32(address, 0x30) == 3 then return "Booster Seat"
  elseif ReadValue32(address, 0x30) == 4 then return "Classic Dragster"
  elseif ReadValue32(address, 0x30) == 5 then return "Offroader"
  elseif ReadValue32(address, 0x30) == 6 then return "Mini Beast"
  elseif ReadValue32(address, 0x30) == 7 then return "Wild Wing"
  elseif ReadValue32(address, 0x30) == 8 then return "Flame Flyer"
  elseif ReadValue32(address, 0x30) == 9 then return "Cheep Charger"
  elseif ReadValue32(address, 0x30) == 10 then return "Super Blooper"
  elseif ReadValue32(address, 0x30) == 11 then return "Piranha Prowler"
  elseif ReadValue32(address, 0x30) == 12 then return "Tiny Titan"
  elseif ReadValue32(address, 0x30) == 13 then return "Daytripper"
  elseif ReadValue32(address, 0x30) == 14 then return "Jetsetter"
  elseif ReadValue32(address, 0x30) == 15 then return "Blue Falcon"
  elseif ReadValue32(address, 0x30) == 16 then return "Sprinter"
  elseif ReadValue32(address, 0x30) == 17 then return "Honeycoupe"
  elseif ReadValue32(address, 0x30) == 18 then return "Standard Bike S"
  elseif ReadValue32(address, 0x30) == 19 then return "Standard Bike M"
  elseif ReadValue32(address, 0x30) == 20 then return "Standard Bike L"
  elseif ReadValue32(address, 0x30) == 21 then return "Bullet Bike"
  elseif ReadValue32(address, 0x30) == 22 then return "Mach Bike"
  elseif ReadValue32(address, 0x30) == 23 then return "Flame Runner"
  elseif ReadValue32(address, 0x30) == 24 then return "Bit Bike"
  elseif ReadValue32(address, 0x30) == 25 then return "Sugarscoot"
  elseif ReadValue32(address, 0x30) == 26 then return "Wario Bike"
  elseif ReadValue32(address, 0x30) == 27 then return "Quacker"
  elseif ReadValue32(address, 0x30) == 28 then return "Zip Zip"
  elseif ReadValue32(address, 0x30) == 29 then return "Shooting Star"
  elseif ReadValue32(address, 0x30) == 30 then return "Magikruiser"
  elseif ReadValue32(address, 0x30) == 31 then return "Sneakster"
  elseif ReadValue32(address, 0x30) == 32 then return "Spear"
  elseif ReadValue32(address, 0x30) == 33 then return "Jet Bubble"
  elseif ReadValue32(address, 0x30) == 34 then return "Dolphin Dasher"
  elseif ReadValue32(address, 0x30) == 35 then return "Phantom"
  end
end
core.getVehicle = getVehicle

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

local function getPrevPosGhost()
  local address = Pointers.getPrevPositionPointer(0x4)
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

local function getSpdGhost()
  local PrevXpos = getPrevPosGhost().X
  local PrevYpos = getPrevPosGhost().Y
  local PrevZpos = getPrevPosGhost().Z
  local Xpos = getPosGhost().X
  local Ypos = getPosGhost().Y
  local Zpos = getPosGhost().Z
  return {X = (Xpos - PrevXpos), Y = (Ypos - PrevYpos), Z = (Zpos - PrevZpos), XZ = math.sqrt(((Xpos - PrevXpos)^2) + (Zpos - PrevZpos)^2), XYZ = math.sqrt(((Xpos - PrevXpos)^2) + ((Ypos - PrevYpos)^2) + (Zpos - PrevZpos)^2)}
end
core.getSpdGhost = getSpdGhost

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

local function floatHack(intVal)
  return string.unpack("f", string.pack("I4", intVal))
end

local function getFinishLine()
  local kmpBase = Pointers.getKMPBasePointer()
  local offset = ReadValue32(kmpBase, 0x24)
  local resultList = {}
  
  for i = 0x0, 0xC, 0x4 do
	resultList[#resultList + 1] = floatHack(ReadValue32(kmpBase, 0x4C + offset + 0x8 + i))
  end
  
  return {
    X1 = resultList[1],
    Z1 = resultList[2],
    X2 = resultList[3],
    Z2 = resultList[4]
  }
end
core.getFinishLine = getFinishLine

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

local function direction(a, b)
	if a > 0 and b > 0 then return (270 + (math.atan(math.abs(b)/math.abs(a)) * 180 / math.pi))
	elseif a > 0 and b < 0 then return (90 - (math.atan(math.abs(b)/math.abs(a)) * 180 / math.pi))
	elseif a < 0 and b > 0 then return (270 - (math.atan(math.abs(b)/math.abs(a)) * 180 / math.pi))
	elseif a < 0 and b < 0 then return (90 + (math.atan(math.abs(b)/math.abs(a)) * 180 / math.pi))
	else return 0
	end
end
core.direction = direction

local function calculateDirection()
	local pz, py, px, prevz, prevy, prevx = getPos().Z, getPos().Y, getPos().X, getPrevPos().Z, getPrevPos().Y, getPrevPos().X
	local finz1, finz2, finx1, finx2 = getFinishLine().Z1, getFinishLine().Z2, getFinishLine().X1, getFinishLine().X2
	local zdiff = pz - prevz
	local ydiff = py - prevy
	local xdiff = px - prevx
	local finzdiff = finz1 - finz2
	local finxdiff = finx1 - finx2
	
	if getFrameOfInput() == 0 then return {X = 0, Y = 0, FY = 0, Z = 0}
	else return {X = direction(ydiff, zdiff), Y = direction(zdiff, xdiff), FY = direction(finzdiff, finxdiff), Z = direction(ydiff, xdiff)}
	end
end
core.calculateDirection = calculateDirection

local function calculateDifference(number)
	local xz, px, pz, gx, gz, prevx, prevz = getSpd().XZ, getPos().X, getPos().Z, getPosGhost().X, getPosGhost().Z, getPrevPos().X, getPrevPos().Z
	local direction_id = 0
	local direction_cardinal = 0
	local d = number -- angle offset used for measuring time difference along
	
	if number == nil then d = 0
	end

	length = pz - gz
	width = px - gx
	angle = math.atan(math.abs(width)/math.abs(length))

	if length < 0 and d > 270 and d < 360 then direction_id = -1
	elseif length < 0 and d >= 0 and d < 90 then direction_id = -1
	elseif length > 0 and d > 90 and d < 270 then direction_id = -1
	else direction_id = 1
	end

	if length > 0 and width > 0 then direction_cardinal = 270
	elseif length > 0 and width < 0 then direction_cardinal = 0
	elseif length < 0 and width > 0 then direction_cardinal = 90
	elseif length < 0 and width < 0 then direction_cardinal = 180
	end

	angle_right = d - direction_cardinal
	hypotenuse = math.sqrt(length^2 + width^2)
	angle_left = math.abs(angle - angle_right)
	distance = math.cos(angle_left)*hypotenuse

	if length == 0 then return {D = 0, I = 0}
	elseif xz == 0 then return {D = 0, I = 1}
	else return {D = (math.abs((distance/xz)/(60))*direction_id), I = 0}
	end
end
core.calculateDifference = calculateDifference

local function currentDifference()
	local playerStart, playerMoveFull, ghostStart, ghostMoveFull = getPos(), getSpd(), getPosGhost(), getSpdGhost()
	local playerMove = {X = playerMoveFull.X, Y = playerMoveFull.Y, Z = playerMoveFull.Z}
	local ghostMove = {X = ghostMoveFull.X, Y = ghostMoveFull.Y, Z = ghostMoveFull.Z}
	local distance = math_core.vectorMovementDifferenceXZ(playerStart, playerMove, ghostStart, ghostMove)
	local avgSpeed = (playerMoveFull.XZ + ghostMoveFull.XZ) / 2
	
	if distance == 0 then return {D = 0, I = 0}
	elseif avgSpeed == 0 then return {D = 0, I = 1}
	else return {D = (distance/avgSpeed)/60, I = 0}
	end
end
core.currentDifference = currentDifference

local function differenceText(value, errors)
	if errors == 1 then return "Not Moving"
	else
		if value > 0 then return "Ahead"
		elseif value < 0 then return "Behind"
		else return "Tied"
		end
	end
end
core.differenceText = differenceText

local function exactFinish()
	local address1 = ReadValueFloat(0x1b0)
	local address2 = ReadValueFloat(0x1b4)
	local address3 = ReadValueFloat(0x1b8)
	local basePointer = GetPointerNormal(Pointers.getRaceInfoPointer(0x0), 0x3C)
    local minaddress1 = ReadValue8(basePointer, 0x0 + 0x4) -- find and add in
    local minaddress2 = ReadValue8(basePointer, 0xC + 0x4) -- find and add in
    local minaddress3 = ReadValue8(basePointer, 0x18 + 0x4) -- find and add in
    local secaddress1 = ReadValue8(basePointer, 0x0 + 0x6) -- find and add in
    local secaddress2 = ReadValue8(basePointer, 0xC + 0x6) -- find and add in
    local secaddress3 = ReadValue8(basePointer, 0x18 + 0x6) -- find and add in
	local frame = getFrameOfInput() - 240
	local one = 0
	local two = 0
	local three = 0
	local four = 0
	local onem = 0
	local twom = 0
	local threem = 0
	local fourm = 0
	
------- corrections ---------
if address1 > 999 then secaddress1 = secaddress1 - 1
end
if secaddress1 < 0 then secaddress1 = secaddress1 + 60
end
if address1 > 999 and secaddress1 == 59 then minaddress1 = minaddress1 - 1
end

if address2 > 999 then secaddress2 = secaddress2 - 1
end
if secaddress2 < 0 then secaddress2 = secaddress2 + 60
end
if address2 > 999 and secaddress2 == 59 then minaddress2 = minaddress2 - 1
end

if address3 > 999 then secaddress3 = secaddress3 - 1
end
if secaddress3 < 0 then secaddress3 = secaddress3 + 60
end
if address3 > 999 and secaddress3 == 59 then minaddress3 = minaddress3 - 1
end
	
------- seconds and ms (20,40,60) -------
	if address1 ~= 0 then
		one = secaddress1+(address1/1000)
		if address2 ~= 0 then
			two = (secaddress2-secaddress1)+((address2-address1)/1000)
			if two < 0 then
				two = two + 60
				twom = twom + 1
			end
			if address3 ~= 0 then
			three = (secaddress3-secaddress2)+((address3-address2)/1000)
				if three < 0 then
					three = three + 60
					threem = threem + 1
				end
			four = one+two+three
			fourm = onem+twom+threem
			end
		end
	end

	return {OM = onem, OS = one, TM = twom, TS = two, HM = threem, HS = three, FM = fourm, FS = four}
end
core.exactFinish = exactFinish

local function NormalAccel(spd)
	base = Pointers.getPlayerPointer()
	local stats = GetPointerNormal(base, 0x14, 0x14, 0x0)
	local speed = ReadValueFloat(base, 0xC, 0x10, 0x0, 0x10, 0x10, 0x20) -- current vehicle speed -- from RMCE01.ini
	local p1 = ReadValueFloat(stats, 0x48) -- drift acceleration T1
	local d0 = ReadValueFloat(stats, 0x40) -- drift acceleration A0
	local d1 = ReadValueFloat(stats, 0x44) -- drift acceleration A1
	local t1 = ReadValueFloat(stats, 0x34) -- standard acceleration T1
	local t2 = ReadValueFloat(stats, 0x38) -- standard acceleration T2
	local t3 = ReadValueFloat(stats, 0x3C) -- standard acceleration T3
	local a0 = ReadValueFloat(stats, 0x24) -- standard acceleration A0
	local a1 = ReadValueFloat(stats, 0x28) -- standard acceleration A1
	local a2 = ReadValueFloat(stats, 0x2C) -- standard acceleration A2
	local a3 = ReadValueFloat(stats, 0x30) -- standard acceleration A3
	
	if speed > 0 and speed <= t1*spd then return ((a1-a0)/(t1*spd)*speed)+a0
	elseif speed > t1*spd and speed <= t2*spd then return ((a2-a1)/((t2-t1)*spd)*speed)+(a1-((a2-a1)*t1)/(t2-t1))
	elseif speed > t2*spd and speed <= t3*spd then return ((a3-a2)/((t3-t2)*spd)*speed)+(a2-((a3-a2)*t2)/(t3-t2))
	elseif speed > t3*spd and speed <= spd-a3 then return a3
	elseif speed > spd-a3 and speed <= spd then return spd-speed
	else return 0
	end
end
core.NormalAccel = NormalAccel

local function DriftAccel(spd)
	base = Pointers.getPlayerPointer()
	local speed = ReadValueFloat(base, 0xC, 0x10, 0x0, 0x10, 0x10, 0x20) -- current vehicle speed -- from RMCE01.ini
	local p1 = ReadValueFloat(stats, 0x48) -- drift acceleration T1
	local d0 = ReadValueFloat(stats, 0x40) -- drift acceleration A0
	local d1 = ReadValueFloat(stats, 0x44) -- drift acceleration A1
	
	if speed > 0 and speed <= p1*spd then return ((d1-d0)/(p1*spd)*speed)+d0
	elseif speed > p1*spd and speed <= spd-d1 then return d1
	elseif speed > spd-d1 and speed <= spd then return spd-speed
	else return 0
	end
end
core.DriftAccel = DriftAccel

local function AccelText(spd)
	base = Pointers.getPlayerPointer()
	local speed = ReadValueFloat(base, 0xC, 0x10, 0x0, 0x10, 0x10, 0x20) -- current vehicle speed -- from RMCE01.ini
end
core.AccelText = AccelText

local function detectAction()
	base = Pointers.getPlayerPointer()
	local stats = GetPointerNormal(base, 0x14, 0x14, 0x0)
	local bspeed = ReadValueFloat(stats, 0x18) -- base speed
	local maxspeed = ReadValueFloat(base, 0xC, 0x10, 0x0, 0x10, 0x10, 0x18) -- current max vehicle speed -- from RMCE01.ini
	local ostat1 = ReadValueFloat(stats, 0x70 + 0x8) -- weak offroad
	local ostat2 = ReadValueFloat(stats, 0x70 + 0xC) -- regular offroad
	local ostat3 = ReadValueFloat(stats, 0x70 + 0x10) -- heavy offroad
	local ostat4 = ReadValueFloat(stats, 0x70 + 0x14) -- slippery offroad
	local mtboost = ReadValueFloat(base, 0xC, 0x10, 0x0, 0x10, 0x10, 0x10C)
	local trickboost = ReadValueFloat(base, 0xC, 0x10, 0x0, 0x10, 0x10, 0x114)
	local shroomboost = ReadValueFloat(base, 0xC, 0x10, 0x0, 0x10, 0x10, 0x110)
	
	if maxspeed/bspeed >= 1 then
		if maxspeed == bspeed then return "No Boost: Handling/Drift"
		elseif maxspeed == 1.15*bspeed then return "No Boost: Wheelie"
		elseif maxspeed == 1.2*bspeed then return "Miniturbo Boost"
		elseif maxspeed == 1.35*bspeed then return "Miniturbo Boost"
		elseif maxspeed == 1.3*bspeed then return "Trick Boost"
		elseif maxspeed == 1.45*bspeed then return "Trick Boost"
		elseif maxspeed == 1.4*bspeed then return "Mushroom Boost"
		elseif maxspeed == 1.55*bspeed then return "Mushroom Boost"
		elseif maxspeed == 120 then
			if mtboost > 0 then
				if shroomboost > 0 then
					if trickboost > 0 then return "Trick Boost"
					else return "Mushroom Boost"
					end
				else return "Miniturbo Boost"
				end
			elseif shroomboost > 0 then
				if trickboost > 0 then return "Trick Boost"
				else return "Mushroom Boost"
				end
			elseif trickboost > 0 then return "Trick Boost"
			end
		end
	else
		if maxspeed == ostat1*bspeed then return "Weak Offroad"
		elseif maxspeed == 1.15*ostat1*bspeed then return "Weak Offroad"
		elseif maxspeed == ostat2*bspeed then return "Offroad"
		elseif maxspeed == 1.15*ostat2*bspeed then return "Offroad"
		elseif maxspeed == ostat3*bspeed then return "Heavy Offroad"
		elseif maxspeed == 1.15*ostat3*bspeed then return "Heavy Offroad"
		elseif maxspeed == ostat4*bspeed then return "Slippery Offroad"
		elseif maxspeed == 1.15*ostat4*bspeed then return "Slippery Offroad"
		else return "Against Wall"
		end
	end
end
core.detectAction = detectAction

local function BoostAccel()
	local boost = detectAction()
	
	if boost == "Miniturbo Boost" then return 3
	elseif boost == "Trick Boost" then return 6
	elseif boost == "Mushroom Boost" then return 7
	else return 0
	end
end
core.BoostAccel = BoostAccel

local function AccelRates()
	base = Pointers.getPlayerPointer()
	local stats = GetPointerNormal(base, 0x14, 0x14, 0x0)
	local speed = ReadValueFloat(base, 0xC, 0x10, 0x0, 0x10, 0x10, 0x20) -- current vehicle speed -- from RMCE01.ini
	local maxspeed = ReadValueFloat(base, 0xC, 0x10, 0x0, 0x10, 0x10, 0x18) -- current max vehicle speed -- from RMCE01.ini
	local bspeed = ReadValueFloat(stats, 0x18) -- base speed
	local ostat1 = ReadValueFloat(stats, 0x70 + 0x8) -- weak offroad
	local ostat2 = ReadValueFloat(stats, 0x70 + 0xC) -- regular offroad
	local ostat3 = ReadValueFloat(stats, 0x70 + 0x10) -- heavy offroad
	local ostat4 = ReadValueFloat(stats, 0x70 + 0x14) -- slippery offroad
	local ospeed1 = bspeed * ostat1
	local ospeed2 = bspeed * ostat2
	local ospeed3 = bspeed * ostat3
	local ospeed4 = bspeed * ostat4
	local haccel, waccel, daccel, oaccel1, oaccel2, oaccel3, oaccel4, owaccel1, owaccel2 = NormalAccel(bspeed), NormalAccel(bspeed*1.15), DriftAccel(bspeed), NormalAccel(ospeed1), NormalAccel(ospeed2), NormalAccel(ospeed3), NormalAccel(ospeed4), NormalAccel(ospeed1*1.15), NormalAccel(ospeed2*1.15)
	local owaccel3, owaccel4, odaccel1, odaccel2, odaccel3, odaccel4, baccel, action, note = NormalAccel(ospeed3*1.15), NormalAccel(ospeed4*1.15), DriftAccel(ospeed1), DriftAccel(ospeed2), DriftAccel(ospeed3), DriftAccel(ospeed4), BoostAccel(), detectAction(), ""
	
	if speed <= 0.55*bspeed then note = "Below 55% Spd"
		if speed < 0 then note = "Below 0 Spd"
		end
	else note = ""
	end
	
	return {H = haccel, W = waccel, D = daccel, WO = oaccel1, WOW = owaccel1, WOD = odaccel1, O = oaccel2, OW = owaccel2, OD = odaccel2, HO = oaccel3, HOW = owaccel3, HOD = odaccel3, SO = oaccel4, SOW = owaccel4, SOD = odaccel4, B = baccel, A = action, N = note}

end
core.AccelRates = AccelRates

local function translateKCL()

	local gameID = GetGameID()
		if gameID == "RMCP01" then
			collisionAddress1 = 0x9C38DC
			collisionAddress2 = 0x9C38E8
		elseif gameID == "RMCE01" then
			collisionAddress1 = 0x9BF0D4
			collisionAddress2 = 0x9BF0E0
		elseif gameID == "RMCJ01" then
			collisionAddress1 = 0x9C293C
			collisionAddress2 = 0x9C2948
		elseif gameID == "RMCK01" then
			collisionAddress1 = 0x9B1F1C
			collisionAddress2 = 0x9B1F28
	end

	local writeAddress1 = 0x1500000
	local writeAddress2 = 0x1500008
	local rawCollisionValue1 = ReadValue16(collisionAddress1)
	local rawCollisionValue2 = ReadValue16(collisionAddress2)
	local collisionValue1 = rawCollisionValue1 % 0x20
	local collisionValue2 = rawCollisionValue2 % 0x20
	local collisionType1 = ""
	local collisionType2 = ""
	
	if rawCollisionValue1/0x8000 >= 1 then collisionType1 = "SticWall"
	elseif rawCollisionValue1/0x4000 >= 1 then collisionType1 = "PushAway"
	elseif rawCollisionValue1/0x2000 >= 1 then collisionType1 = "TrickRod"
	else
		if collisionValue1 == 0x0 then collisionType1 = "Road"
		elseif collisionValue1 == 0x1 then collisionType1 = "SlipRoad"
		elseif collisionValue1 == 0x2 then collisionType1 = "WOffRoad"
		elseif collisionValue1 == 0x3 then collisionType1 = "Off-Road"
		elseif collisionValue1 == 0x4 then collisionType1 = "HOffRoad"
		elseif collisionValue1 == 0x5 then collisionType1 = "SlipRoad"
		elseif collisionValue1 == 0x6 then collisionType1 = "BoostPad"
		elseif collisionValue1 == 0x7 then collisionType1 = "B-Ramp"
		elseif collisionValue1 == 0x8 then collisionType1 = "JumpPad "
		elseif collisionValue1 == 0x9 then collisionType1 = "ItemRoad"
		elseif collisionValue1 == 0xA then collisionType1 = "SoldFall"
		elseif collisionValue1 == 0xB then collisionType1 = "MovWater" --KC River
		elseif collisionValue1 == 0xC then collisionType1 = "Wall"
		elseif collisionValue1 == 0xD then collisionType1 = "InvWall "
		elseif collisionValue1 == 0xE then collisionType1 = "ItemWall"
		elseif collisionValue1 == 0xF then collisionType1 = "Wall"
		elseif collisionValue1 == 0x10 then collisionType1 = "FallBoun"
		elseif collisionValue1 == 0x11 then collisionType1 = "CannonTr"
		elseif collisionValue1 == 0x12 then collisionType1 = "ReCalc" --enemy/item route based
		elseif collisionValue1 == 0x13 then collisionType1 = "HalfPipe"
		elseif collisionValue1 == 0x14 then collisionType1 = "Wall"
		elseif collisionValue1 == 0x15 then collisionType1 = "MoveRoad" --TF belt, CM escalator
		elseif collisionValue1 == 0x16 then collisionType1 = "SticRoad"
		elseif collisionValue1 == 0x17 then collisionType1 = "Road"
		elseif collisionValue1 == 0x18 then collisionType1 = "SoundTrg"
		elseif collisionValue1 == 0x19 then collisionType1 = "Unknown?"
		elseif collisionValue1 == 0x1A then collisionType1 = "EffectTr"
		elseif collisionValue1 == 0x1B then collisionType1 = "Unknown?"
		elseif collisionValue1 == 0x1C then collisionType1 = "Unknown?"
		elseif collisionValue1 == 0x1D then collisionType1 = "SlipRoad"
		elseif collisionValue1 == 0x1E then collisionType1 = "SpecWall"
		elseif collisionValue1 == 0x1F then collisionType1 = "Wall" --items can get through
		end
	end

	if rawCollisionValue2/0x8000 >= 1 then collisionType2 = "SticWall"
	elseif rawCollisionValue2/0x4000 >= 1 then collisionType2 = "PushAway"
	elseif rawCollisionValue2/0x2000 >= 1 then collisionType2 = "TrickRod"
	else
		if collisionValue2 == 0x0 then collisionType2 = "Road"
		elseif collisionValue2 == 0x1 then collisionType2 = "SlipRoad"
		elseif collisionValue2 == 0x2 then collisionType2 = "WOffRoad"
		elseif collisionValue2 == 0x3 then collisionType2 = "Off-Road"
		elseif collisionValue2 == 0x4 then collisionType2 = "HOffRoad"
		elseif collisionValue2 == 0x5 then collisionType2 = "SlipRoad"
		elseif collisionValue2 == 0x6 then collisionType2 = "BoostPad"
		elseif collisionValue2 == 0x7 then collisionType2 = "B-Ramp"
		elseif collisionValue2 == 0x8 then collisionType2 = "JumpPad"
		elseif collisionValue2 == 0x9 then collisionType2 = "ItemRoad"
		elseif collisionValue2 == 0xA then collisionType2 = "SoldFall"
		elseif collisionValue2 == 0xB then collisionType2 = "MovWater" --KC River
		elseif collisionValue2 == 0xC then collisionType2 = "Wall"
		elseif collisionValue2 == 0xD then collisionType2 = "InvWall"
		elseif collisionValue2 == 0xE then collisionType2 = "ItemWall"
		elseif collisionValue2 == 0xF then collisionType2 = "Wall"
		elseif collisionValue2 == 0x10 then collisionType2 = "FallBoun"
		elseif collisionValue2 == 0x11 then collisionType2 = "CannonTr"
		elseif collisionValue2 == 0x12 then collisionType2 = "ReCalc" --enemy/item route based
		elseif collisionValue2 == 0x13 then collisionType2 = "HalfPipe"
		elseif collisionValue2 == 0x14 then collisionType2 = "Wall"
		elseif collisionValue2 == 0x15 then collisionType2 = "MoveRoad" --TF belt, CM escalator
		elseif collisionValue2 == 0x16 then collisionType2 = "SticRoad"
		elseif collisionValue2 == 0x17 then collisionType2 = "Road"
		elseif collisionValue2 == 0x18 then collisionType2 = "SoundTrg"
		elseif collisionValue2 == 0x19 then collisionType2 = "Unknown?"
		elseif collisionValue2 == 0x1A then collisionType2 = "EffectTr"
		elseif collisionValue2 == 0x1B then collisionType2 = "Unknown?"
		elseif collisionValue2 == 0x1C then collisionType2 = "Unknown?"
		elseif collisionValue2 == 0x1D then collisionType2 = "SlipRoad"
		elseif collisionValue2 == 0x1E then collisionType2 = "SpecWall"
		elseif collisionValue2 == 0x1F then collisionType2 = "Wall" --items can get through
		end
	end
	
	return {A1 = writeAddress1, A2 = writeAddress2, T1 = collisionType1, T2 = collisionType2}
end
core.translateKCL = translateKCL

return core
