local math_core = {}

-- vector_definement = {X = 0.0, Y = 0.0, Z = 0.0}
-- vector math

local function pureDiagonalVector(val)
	return {X = val, Y = val, Z = val}
end
math_core.pureDiagonalVector = pureDiagonalVector

local function copyVector(vec)
	return {X = vec.X, Y = vec.Y, Z = vec.Z}
end
math_core.copyVector = copyVector

local function vecEquals(vec1, vec2)
	return (vec1.X == vec2.X) and (vec1.Y == vec2.Y) and (vec1.Z == vec2.Z)
end
math_core.vecEquals = vecEquals

local function addVector(vec1, vec2)
	return {X = vec1.X + vec2.X, Y = vec1.Y + vec2.Y, Z = vec1.Z + vec2.Z}
end
math_core.addVector = addVector

local function subtractVector(vec1, vec2)
	return {X = vec1.X - vec2.X, Y = vec1.Y - vec2.Y, Z = vec1.Z - vec2.Z}
end
math_core.subtractVector = subtractVector

local function multiplyVector(vec1, vec2)
	return {X = vec1.X * vec2.X, Y = vec1.Y * vec2.Y, Z = vec1.Z * vec2.Z}
end
math_core.multiplyVector = multiplyVector

local function divideVector(vec1, vec2)
	return {X = vec1.X / vec2.X, Y = vec1.Y / vec2.Y, Z = vec1.Z / vec2.Z}
end
math_core.divideVector = divideVector

local function vectorLength(vec)
	return math.sqrt(vec.X * vec.X + vec.Y * vec.Y + vec.Z * vec.Z)
end
math_core.vectorLength = vectorLength

local function dotProduct(vec1, vec2)
	return (vec1.X * vec2.X + vec1.Y * vec2.Y + vec1.Z * vec2.Z)
end
math_core.dotProduct = dotProduct

local function crossProduct(vec1, vec2)
	local x = vec1.Y * vec2.Z - vec1.Z * vec2.Y
	local y = vec1.Z * vec2.X - vec1.X * vec2.Z
	local z = vec1.X * vec2.Y - vec1.Y * vec2.X
	return {X = x, Y = y, Z = z}
end
math_core.crossProduct = crossProduct

-- selectionFunction == function({X,Y,Z}) -> {U,V,W}
local function intersectionVector(start1, move1, start2, move2, useThirdDimension, selectionFunction)
	local mulVec = selectionFunction(divideVector(move1, move2))
	
	if (mulVec.U == mulVec.V and ((not useThirdDimension) or (mulVec.U == mulVec.W))) then
		return 1
	end
	
	local start1Select = selectionFunction(start1)
	local move1Select = selectionFunction(move1)
	local start2Select = selectionFunction(start2)
	local move2Select = selectionFunction(move2)
	
	if (move1Select.U == 0) and (move1Select.V == 0) then
		return 0
	end
	if (move2Select.U == 0) and (move2Select.V == 0) then
		return 0
	end
	
	-- -- start1.U + t * move1.U = start2.U + r * move2.U
	local t, r = 0
	
	if (move1Select.U == 0) then -- start1.U = start2.U + r * move2.U
		r = (start1Select.U - start2Select.U) / move2Select.U
		t = ((start2Select.V + r * move2Select.V) - start1Select.V) / move1Select.V
	elseif (move1Select.V == 0) then -- start1.V = start2.V + r * move2.V
		r = (start1Select.V - start2Select.V) / move2Select.V
		t = ((start2Select.U + r * move2Select.U) - start1Select.U) / move1Select.U
	else
		local matchMul = move1Select.U / move1Select.V
		local const1 = start1Select.U - (start1Select.V * matchMul)
		local const2 = start2Select.U - (start2Select.V * matchMul)
		local var2 = move2Select.U - (move2Select.V * matchMul)
		
		-- -- const1 = const2 + r*var2
		r = (const1 - const2) / var2
		t = ((start2Select.U + r * move2Select.U) - start1Select.U) / move1Select.U
	end
	
	if (useThirdDimension) then
		if ((start1Select.W + t * move1Select.W) ~= (start2Select.W + r * move2Select.W)) then
			return 0
		end
	end
	
	local u = start1Select.U + t * move1Select.U
	local v = start1Select.V + t * move1Select.V
	local w = start1Select.W + t * move1Select.W
	
	return {U = u, V = v, W = w}, t, r
end
math_core.intersectionVector = intersectionVector

local function vectorMovementDifferenceXZ(start1, move1, start2, move2)
	if (vecEquals(start1, start2) and vecEquals(move1, move2)) then
		return 0
	end
	
	local zeroVector = {X = 0, Y = 0, Z = 0}
	local pointToPointDistance = vectorLength(subtractVector(start1, start2))
	
	if (vecEquals(move1, zeroVector) or vecEquals(move2, zeroVector)) then
		return pointToPointDistance
	end

	local intersectionUVW, t, r = intersectionVector(start1, move1, start2, move2, false,
												  function(vec) return {U = vec.X, V = vec.Z, W = vec.Y} end)
	
	if (intersectionUVW == 0) then
		return pointToPointDistance
	elseif (intersectionUVW == 1) then
		local move1Normal = {X = move1.Z, Y = move1.Y, Z = -move1.X}
		intersectionUVW, t, r = intersectionVector(start1, move1Normal, start2, move2, false,
												  function(vec) return {U = vec.X, V = vec.Z, W = vec.Y} end)
		
		if (intersectionUVW == 0) or (intersectionUVW == 1) then
			return pointToPointDistance
		end
		
		local intersection = {X = intersectionUVW.U, Y = intersectionUVW.W, Z = intersectionUVW.V}
		if (r > 0) then
			return vectorLength(start2, intersection)
		elseif (r < 0) then
			return -vectorLength(start2, intersection)
		else
			return 0
		end
		
	else
		local intersection = {X = intersectionUVW.U, Y = intersectionUVW.W, Z = intersectionUVW.V}
		
		local distance1 = vectorLength(subtractVector(intersection, start1))
		local distance2 = vectorLength(subtractVector(intersection, start2))
		
		
		if (t >= 0) and (r >= 0) then
			return distance2 - distance1
		elseif (t < 0) and (r >= 0) then
			return ((distance1 + distance2) + pointToPointDistance) / 2
		elseif (t >= 0) and (r < 0) then
			return ((distance1 + distance2) + pointToPointDistance) / (-2)
		else
			return distance1 - distance2
		end
	end
	
	return pointToPointDistance
end
math_core.vectorMovementDifferenceXZ = vectorMovementDifferenceXZ

local function distanceToLineXZ(start, move, line, lineMove)
	start.Y = 0
	move.Y = 0
	line.Y = 0
	lineMove.Y = 0

	local intersectionUVW = intersectionVector(start, move, line, lineMove, false,
											   function(vec) return {U = vec.X, V = vec.Z, W = vec.Y} end)
	if (intersectionUVW == 0) or (intersectionUVW == 1) then
		return 0
	else
		local intersection = {X = intersectionUVW.U, Y = intersectionUVW.W, Z = intersectionUVW.V}
		return vectorLength(subtractVector(intersection, start))
	end
	
	return 0
end
math_core.distanceToLineXZ = distanceToLineXZ

return math_core
