--Writes data to file
--Setup for analysis in the R programming language.
package.path = GetScriptsDir() .. "MKW/MKW_core.lua"
local core = require("MKW_core")
input_file = io.open("euler_output.csv", "w")
io.output(input_file)

local prevFrame = core.getFrameOfInput()

function onScriptStart()
	MsgBox("Script started, recording rotation values")
  io.write("")
end

function onScriptCancel()
	MsgBox("Rotation script ended.")
  io.write("")
end

function onScriptUpdate()
  currentFrame = core.getFrameOfInput()
  if currentFrame == 0 then return 0 end
  if currentFrame  == prevFrame + 1 then  --If the framecount has increased, start looking at the next row of the input file.
    prevFrame = currentFrame
  
  local frame = core.getFrameOfInput()
	local basevel = core.getSpd()
  local xz =  core.getSpd().XZ
	local xpos = core.getPos().X
	local ypos = core.getPos().Y
	local zpos = core.getPos().Z
	local xrot = core.calculateDirection().X
	local yrot = core.calculateDirection().Y
	local zrot = core.calculateDirection().Z
  local pitch = core.calculateEuler().X
  local yaw = core.calculateEuler().Y
  local roll = core.calculateEuler().Z
    
    local outputString = string.format("\n%i, %f, %f, %f, %f, %f, %f, ", frame, xpos, ypos, zpos, yaw, pitch, roll) --Output current Euler Rotation to list.
    io.write(outputString)

    
  end
  
end

function onStateLoaded()

end

function onStateSaved()

end
