local core = require "MKW_core"

turnCount = 1  -- Keeps track of how long you've been turning for (only cares about turn values that will drop wheelie)
turnDir = 1 --Turn direction variable. -1 if turning left, 1 if turning right
prevFrameCount = 0
preventedDrop = false

values = {0, 68, 77, 86, 95, 104, 112, 128, 152, 161, 170, 179, 188, 197, 255}

local function autoWheelie()
	input = getInput().X  --horizontal input value(0-14)
	drift = getInput().ABLR --Prevents script from overwriting your turn value when drifting
	inputXtrmeRight = (input-13)*(input-14) --A polynomial defined to have roots of 13,14.
	inputXtrmeLeft = input*(input-1) --A polynomial defined to have roots of 0,1.
	--Now you are either extreme turning, or you are not
	if inputXtrmeLeft * inputXtrmeRight == 0 then
	--You have either been extreme turning for less, or greater than or equal to 14 frames
		if turnCount < 15 then
		--No issue here, simply increment the turn counter
			turnCount = turnCount + 1
			if inputXtrmeLeft == 0 then
				turnDir = -1
        return 0
			else
				turnDir = 1
        return 255
			end
		else --Turn count is greater or equal to 14 frames
		--You either want your wheelie to break or you do not
			if drift ~= 11 then
			--You do not want your wheelie to break
      preventedDrop = true
        turnCount = 1
				if inputXtrmeLeft == 0 then
					turnDir = -1
          return 77
				else
          turnDir = 1
          return 188
				end
			else
			--You want your wheelie to break
				turnCount = turnCount + 1
        if inputXtrmeLeft == 0 then
          return 0
        else
          return 255
        end
			end
		end
  elseif preventedDrop == true then
    if turnDir == -1 then
      return 0
    else
      return 255
    end
	else
		turnCount = 1
    return values[input+1]
	end
end

function onScriptStart()
	MsgBox("Script started. Press drift to drift.")
end

function onScriptUpdate()
  currentFrameCount = GetFrameCount()
  if currentFrameCount > prevFrameCount then
    correctInput = autoWheelie()
    prevFrameCount = currentFrameCount
  end
  SetMainStickX(correctInput)
end

function onScriptCancel()

end
