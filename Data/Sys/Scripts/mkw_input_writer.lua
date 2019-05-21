--Input writer
--Observes input data while using TAS Code on a ghost and writes data to a file.
local core = require "MKW_core"
input_file = io.open("mkw_input_reader_output.lua", "w")
io.output(input_file)

local prevFrame = core.getFrameOfInput()

function onScriptStart()
	MsgBox("Script started.")
  io.write("local mkw_input_reader_output = {")
end

function onScriptCancel()
	MsgBox("Script ended.")
  io.write("}\n\nreturn mkw_input_reader_output")
end

function getInputs()
  button_data = core.getInput().ABLR
  if button_data == 0 then A = 0 B = 0 L = 0
  elseif button_data == 1 then A = 1 B = 0 L = 0
  elseif button_data == 2 then A = 0 B = 1 L = 0
  elseif button_data == 3 then A = 1 B = 1 L = 0
  elseif button_data == 4 then A = 0 B = 0 L = 1
  elseif button_data == 5 then A = 1 B = 0 L = 1
  elseif button_data == 6 then A = 0 B = 1 L = 1
  elseif button_data == 7 then A = 1 B = 1 L = 1
  elseif button_data == 8 then A = 0 B = 1 L = 0
  elseif button_data == 10 then A = 0 B = 1 L = 0
  elseif button_data == 11 then A = 1 B = 1 L = 0
  elseif button_data == 14 then A = 0 B = 1 L = 1
  else A = 1 B = 1 L = 1
  end

	Horiz = core.getInput().X
  Vert = core.getInput().Y
  dpad_data = core.getInput().DPAD
  if dpad_data == 0 then DU = 0 DD = 0 DL = 0 DR = 0
  elseif dpad_data == 1 then DU = 1 DD = 0 DL = 0 DR = 0
  elseif dpad_data == 2 then DU = 0 DD = 1 DL = 0 DR = 0
  elseif dpad_data == 3 then DU = 0 DD = 0 DL = 1 DR = 0
  elseif dpad_data == 4 then DU = 0 DD = 0 DL = 0 DR = 1
  end
  return A, B, L, Horiz, Vert, DU, DD, DL, DR
end

function onScriptUpdate()
  currentFrame = core.getFrameOfInput()
  if currentFrame == 0 then return 0 end
  if currentFrame  == prevFrame + 1 then  --If the framecount has increased, start looking at the next row of the input file.
    prevFrame = currentFrame
    A,B,L, Horiz, Vert, DU, DD, DL, DR = getInputs()
    io.write("\n{Horiz = ")
    io.write(Horiz)
    io.write(", Vert = ")
    io.write(Vert)
    io.write(", A = ")
    io.write(A)
    io.write(", B = ")
    io.write(B)
    io.write(", L = ")
    io.write(L)
    io.write(", DU = ")
    io.write(DU)
    io.write(", DD = ")
    io.write(DD)
    io.write(", DL = ")
    io.write(DL)
    io.write(", DR = ")
    io.write(DR)
    io.write("},")
  elseif currentFrame < prevFrame then  --This occurs when we load a savestate.
    local frameDifference = currentFrame - prevFrame
    for i=1, currentFrame, 1 do
      input_file:seek ("end",frameDifference)


    end
  end

end

function onStateLoaded()

end

function onStateSaved()

end
