local core = {}

local angles = require "sa2b_angles"

----- GETTING DATA FROM MEMORY -----

local function getXPos()
	local pointer = 0x1E7768
	local offset = 0x14
	local address = GetPointerNormal(pointer)
	return ReadValueFloat(address + offset)
end
core.getXPos = getXPos

local function getYPos()
	local pointer = 0x1E7768
	local offset = 0x18
	local address = GetPointerNormal(pointer)
	return ReadValueFloat(address + offset)
end
core.getYPos = getYPos

local function getZPos()
	local pointer = 0x1E7768
	local offset = 0x1C
	local address = GetPointerNormal(pointer)
	return ReadValueFloat(address + offset)
end
core.getZPos = getZPos

local function getXRot()
	local pointer = 0x1E7768
	local offset = 0xA
	local address = GetPointerNormal(pointer)
	return ReadValue16(address + offset)
end
core.getXRot = getXRot

local function getYRot()
	local pointer = 0x1E7768
	local offset = 0xE
	local address = GetPointerNormal(pointer)
	return ReadValue16(address + offset)
end
core.getYRot = getYRot

local function getZRot()
	local pointer = 0x1E7768
	local offset = 0x12
	local address = GetPointerNormal(pointer)
	return ReadValue16(address + offset)
end
core.getZRot = getZRot

local function getFinalYRot()
	local pointer = 0x1E7748
	local offset = 0x1E
	local address = GetPointerNormal(pointer)
	return ReadValue16(address + offset)
end
core.getFinalYRot = getFinalYRot

local function getCameraYRot()
	local address = 0x1FF5CA
	return ReadValue16(address)
end
core.getCameraYRot = getCameraYRot

----- CORE FUNCTIONS -----

-- angleInput: sets X and Y in main stick to make the character go to angleTarget
local function angleInput(angleTarget)

	-- angleOffset: the angle between angleTarget and camera Y rotation
	local angleOffset = (angleTarget + getCameraYRot() - 49152) % 65536
	
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
	local xrot = -getXRot() * 360 / 65536
	local yrot = getYRot() * 360 / 65536
	local zrot = -getZRot() * 360 / 65536
	
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
	local dx = X - getXPos()
	local dy = Y - getYPos()
	local dz = Z - getZPos()
	
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

-- smoothTurn: limits the given angle to a range of +4000 and -4000 units around current Y rotation angle
local function smoothTurn(angle)

	if (angle - getYRot()) % 65536 > 4000 and (angle - getYRot()) % 65536 < 65536 / 2 then
		angle = getYRot() + 4000
	end
	
	if (angle - getYRot()) % 65536 >= 65536 / 2 and (angle - getYRot()) % 65536 < 65536 - 4000 then
		angle = getYRot() - 4000
	end
	
	return angle % 65536
end
core.smoothTurn = smoothTurn

return core