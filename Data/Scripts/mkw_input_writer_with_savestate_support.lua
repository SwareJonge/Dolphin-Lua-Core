--Input writer
--Observes input data while using TAS Code on a ghost and writes data to a file.
local core = require "MKW_core"
local prevFrame = core.getFrameOfInput()
local inputs = {}
local currentFrame = 0
local startingFrame = 0
local difference = 0
local startingFrame = 0

function onScriptStart()
	MsgBox("Script started.")
	startingFrame = core.getFrameOfInput()
end

function onScriptCancel()
  MsgBox("Script ended.")
  input_file = io.open("mkw_input_reader_output.lua", "w")
  io.output(input_file)
  io.write("local mkw_input_reader_output = {\n")
  for i = 1, difference, 1 do
    io.write("{Horiz = ", inputs[i]["Horiz"], ", Vert = ", inputs[i]["Vert"], ", A = ", inputs[i]["A"], ", B = ", inputs[i]["B"], ", L = ", inputs[i]["L"], ", DU = ", inputs[i]["DU"], ", DD = ", inputs[i]["DD"], ", DL = ", inputs[i]["DL"], ", DR = ", inputs[i]["DR"], "},\n")
  end
  io.write("{Horiz = ", inputs[difference]["Horiz"], ", Vert = ", inputs[difference]["Vert"], ", A = ", inputs[difference]["A"], ", B = ", inputs[difference]["B"], ", L = ", inputs[difference]["L"], ", DU = ", inputs[difference]["DU"], ", DD = ", inputs[difference]["DD"], ", DL = ", inputs[difference]["DL"], ", DR = ", inputs[difference]["DR"], "}")
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

function storeInputs()
  prevFrame = currentFrame
  A,B,L, Horiz, Vert, DU, DD, DL, DR = getInputs()
  inputs[difference]["Horiz"] = Horiz
  inputs[difference]["Vert"] = Vert
  inputs[difference]["A"] = A
  inputs[difference]["B"] = B
  inputs[difference]["L"] = L
  inputs[difference]["DU"] = DU
  inputs[difference]["DD"] = DD
  inputs[difference]["DL"] = DL
  inputs[difference]["DR"] = DR
end

function onScriptUpdate()
  currentFrame = core.getFrameOfInput()
  difference = currentFrame - startingFrame
  if currentFrame == 0 then return 0 end
  if currentFrame  == prevFrame + 1 then  --If the framecount has increased, start looking at the next row of the input file.
    inputs[difference] = {}
    storeInputs()
  elseif currentFrame < prevFrame then  --This occurs when we load a savestate.
    storeInputs()
  end
end

function onStateLoaded()

end

function onStateSaved()

end
