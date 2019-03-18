local core = {}

local sa2b_angles = require "sa2b_angles"
local sadx_angles = require "sadx_angles"
local sh_angles = require "sh_angles"
local angles

local smoothTurnAngle = 4000

----- GETTING DATA FROM MEMORY -----

-- SA2B (NTSC-U): GSNE8P
-- SADX (NTSC-U): GXSE8P
-- SH (NTSC-U): G9SE8P

local function getGameID()
	return ReadValueString(0x0, 6)
end
core.getGameID = getGameID

-- getActiveChar: checks the active character (Sonic Heroes only)
local function getActiveChar()
	return ReadValue8(0x9D986B)
end
core.getActiveChar = getActiveChar

local function getSpd()
	-- F, V, Sd
	local pointer1
	local offset1
	if getGameID() == "GSNE8P" then
		pointer1 = 0x1E7728
		offset1 = 0x64
	elseif getGameID() == "GXSE8P" then
		pointer1 = 0x7A8240
		offset1 = 0x38
	elseif getGameID() == "G9SE8P" then
		pointer1 = 0x2AD0D0 + getActiveChar() * 4
		offset1 = 0x60
	end
	local address1 = GetPointerNormal(pointer1)
	
	if ReadValue32(pointer1) == 0 then
		return {F = 0, V = 0, Sd = 0, X = 0, Y = 0, Z = 0, St = 0}
	end
	
	-- X, Y, Z
	local pointer2
	local offset2
	if getGameID() == "GSNE8P" then
		pointer2 = 0x1E7748
		offset2 = 0x0
	elseif getGameID() == "GXSE8P" then
		pointer2 = 0x7A8260
		offset2 = 0x4
	elseif getGameID() == "G9SE8P" then
		pointer2 = 0x2AD0B0 + getActiveChar() * 4
		offset2 = 0x0
	end
	local address2 = GetPointerNormal(pointer2)
	
	-- St
	local pointer3
	local offset3
	if getGameID() == "GSNE8P" then
		pointer3 = 0x1E7728
		offset3 = 0x40
	elseif getGameID() == "GXSE8P" then
		pointer3 = 0x7A8240
		offset3 = 0x0
	elseif getGameID() == "G9SE8P" then
		pointer3 = 0x2AD0D0 + getActiveChar() * 4
		offset3 = 0x60
	end
	local address3 = GetPointerNormal(pointer3)
	
	return {F = ReadValueFloat(address1 + offset1), V = ReadValueFloat(address1 + offset1 + 4), Sd = ReadValueFloat(address1 + offset1 + 8),
			X = ReadValueFloat(address2 + offset2), Y = ReadValueFloat(address2 + offset2 + 4), Z = ReadValueFloat(address2 + offset2 + 8), 
			St = ReadValueFloat(address3 + offset3)}
end
core.getSpd = getSpd

local function getPos()
	local pointer
	local offset
	if getGameID() == "GSNE8P" then
		pointer = 0x1E7768
		offset = 0x14
	elseif getGameID() == "GXSE8P" then
		pointer = 0x7A8280
		offset = 0x20
	elseif getGameID() == "G9SE8P" then
		pointer = 0x2AD090 + getActiveChar() * 4
		offset = 0x18
	end
	local address = GetPointerNormal(pointer)
	if ReadValue32(pointer) == 0 then
		return {X = 0, Y = 0, Z = 0}
	end
	return {X = ReadValueFloat(address + offset), Y = ReadValueFloat(address + offset + 4), Z = ReadValueFloat(address + offset + 8)}
end
core.getPos = getPos

local function getRot()
	local pointer
	local offset
	if getGameID() == "GSNE8P" then
		pointer = 0x1E7768
		offset = 0xA
	elseif getGameID() == "GXSE8P" then
		pointer = 0x7A8280
		offset = 0x16
	elseif getGameID() == "G9SE8P" then
		pointer = 0x2AD090 + getActiveChar() * 4
		offset = 0xE
	end
	local address = GetPointerNormal(pointer)
	if ReadValue32(pointer) == 0 then
		return {X = 0, Y = 0, Z = 0}
	end
	return {X = ReadValue16(address + offset), Y = ReadValue16(address + offset + 4), Z = ReadValue16(address + offset + 8)}
end
core.getRot = getRot

local function getFinalRot()
	local pointer
	local offset
	if getGameID() == "GSNE8P" then
		pointer = 0x1E7748
		offset = 0x1A
	elseif getGameID() == "GXSE8P" then
		pointer = 0x7A8260
		offset = 0x1E
	elseif getGameID() == "G9SE8P" then
		pointer = 0x2AD0B0 + getActiveChar() * 4
		offset = 0x1A
	end
	local address = GetPointerNormal(pointer)
	if ReadValue32(pointer) == 0 then
		return {X = 0, Y = 0, Z = 0}
	end
	return {X = ReadValue16(address + offset), Y = ReadValue16(address + offset + 4), Z = ReadValue16(address + offset + 8)}
end
core.getFinalRot = getFinalRot

local function getAction()
	local pointer
	local offset
	if getGameID() == "GSNE8P" then
		pointer = 0x1E7768
		offset = 0x0
	elseif getGameID() == "GXSE8P" then
		pointer = 0x7A8280
		offset = 0x0
	elseif getGameID() == "G9SE8P" then
		pointer = 0x2AD090 + getActiveChar() * 4
		offset = 0x1
	end
	local address = GetPointerNormal(pointer)
	if ReadValue32(pointer) == 0 then
		return 0
	end
	return ReadValue8(address + offset)
end
core.getAction = getAction

local function getHover()
	local pointer
	local offset
	if getGameID() == "GSNE8P" then
		pointer = 0x1E7768
		offset = 0x132
	elseif getGameID() == "GXSE8P" then
		pointer = 0x7A8240
		offset = 0x8
	elseif getGameID() == "G9SE8P" then
		pointer = 0x2AD0D0 + getActiveChar() * 4
		offset = 0x12
	end
	local address = GetPointerNormal(pointer)
	if ReadValue32(pointer) == 0 then
		return 0
	end
	return ReadValue16(address + offset)
end
core.getHover = getHover

local function getStatus()
	local pointer
	local offset
	if getGameID() == "GSNE8P" then
		pointer = 0x1E7768
		offset = 0x4
	elseif getGameID() == "GXSE8P" then
		pointer = 0x7A8280
		offset = 0x4
	elseif getGameID() == "G9SE8P" then
		pointer = 0x2AD090 + getActiveChar() * 4
		offset = 0x6
	end
	local address = GetPointerNormal(pointer)
	if ReadValue32(pointer) == 0 then
		return 0
	end
	return ReadValue16(address + offset)
end
core.getStatus = getStatus

local function getCameraYRot()
	local address
	local pointer = 0
	local offset
	if getGameID() == "GSNE8P" then
		address = 0x1FF5CA
		offset = 0x0
	elseif getGameID() == "GXSE8P" then
		pointer = 0x6B44B0
		offset = 0x1A
		address = GetPointerNormal(pointer)
	elseif getGameID() == "G9SE8P" then
		pointer = 0x2D59E0
		offset = 0x12
		address = GetPointerNormal(pointer)
	end
	if ReadValue32(pointer) == 0 then
		return 0
	end
	return ReadValue16(address + offset)
end
core.getCameraYRot = getCameraYRot

----- CORE FUNCTIONS -----

-- angleInput: sets X and Y in main stick to make the character go to angleTarget
local function angleInput(angleTarget)

	if getGameID() == "GSNE8P" then
		angles = sa2b_angles
	elseif getGameID() == "GXSE8P" then
		angles = sadx_angles
	elseif getGameID() == "G9SE8P" then
		angles = sh_angles
	end
	
	-- angleOffset: the angle between angleTarget and camera Y rotation
	local angleOffset = (angleTarget + getCameraYRot()) % 65536
	
	-- binary search for the inputs that better fit angleOffset
	local i = 1

	while angleOffset > angles[i].angle
	do
		i = i + 1
	end
	
	if i > 1 then
		if math.abs(angleOffset - angles[i-1].angle) < math.abs(angleOffset - angles[i].angle) then
			i = i - 1
		end
	end
	
	-- send the found input to the main stick
	SetMainStickX(angles[i].X)
	SetMainStickY(angles[i].Y)
end
core.angleInput = angleInput

-- rotateCoordinates: undoes the in-game rotation system to get the coordinates of an arbitrary point in space relatively to the character's local coordinate system
-- thanks to OnVar for helping me on this function
local function rotateCoordinates(x, y, z)

	-- character's rotation angles; X and Z are negative because of how the game handles angles
	local xrot = -getRot().X * 360 / 65536
	local yrot = getRot().Y * 360 / 65536
	local zrot = -getRot().Z * 360 / 65536
	
	-- rotation matrix: Y1X2Z3
	local c1 = math.cos(math.rad(yrot))
	local s1 = math.sin(math.rad(yrot))
	local c2 = math.cos(math.rad(xrot))
	local s2 = math.sin(math.rad(xrot))
	local c3 = math.cos(math.rad(zrot))
	local s3 = math.sin(math.rad(zrot))
	
	local rot = {{c1 * c3 + s1 * s2 * s3,  c3 * s1 * s2 - c1 * s3, c2 * s1},
				 {c2 * s3,                 c2 * c3,                -s2},
				 {c1 * s2 * s3 - c3 * s1,  c1 * c3 * s2 + s1 * s3, c1 * c2}}
	
	local x_2 = x*rot[1][1] + y*rot[1][2] + z*rot[1][3]
	local y_2 = x*rot[2][1] + y*rot[2][2] + z*rot[2][3]
	local z_2 = x*rot[3][1] + y*rot[3][2] + z*rot[3][3]
	
	return x_2, y_2, z_2
end
core.rotateCoordinates = rotateCoordinates

-- angleToPosition: calculates the angle between the character's position and given position (returned angle is in the character's local coordinate)
local function angleToPosition(X, Y, Z)

	-- deltas in position
	local dx = X - getPos().X
	local dy = Y - getPos().Y
	local dz = Z - getPos().Z
	
	-- uncomment the line below when rotateCoordinates is fixed
	dx, dy, dz = rotateCoordinates(dx, dy, dz)
	
	-- calculating the angle itself based on the position deltas (for some reason, atan2 wasn't working, so I had to do this work around)
	
	-- if angle is 0, return 0 right away to avoid div/0 issues
	if dx == 0 and dz == 0 then
		return 0
	end
	
	local angle = math.deg(math.asin( dz / math.sqrt(dx^2 + dz^2 ) ) )
	
	if dx < 0 then
		angle = 180 - angle
	elseif dz < 0 then
		angle = 360 + angle
	end
	
	-- conversion from degrees to 2 bytes value, which is what's used in the game
	return angle * 65536 / 360
end
core.angleToPosition = angleToPosition

-- smoothTurn: limits the given angle to a range of +smoothTurnAngle and -smoothTurnAngle units around current Y rotation angle
local function smoothTurn(angle)

	if (angle - getRot().Y) % 65536 > smoothTurnAngle and (angle - getRot().Y) % 65536 < 65536 / 2 then
		angle = getRot().Y + smoothTurnAngle
	end
	
	if (angle - getRot().Y) % 65536 >= 65536 / 2 and (angle - getRot().Y) % 65536 < 65536 - smoothTurnAngle then
		angle = getRot().Y - smoothTurnAngle
	end
	
	return angle % 65536
end
core.smoothTurn = smoothTurn

return core