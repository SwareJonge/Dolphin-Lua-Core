--Input reader
--Reads inputs from a file and turns them into inputs in-game.
--If you load a savestate the script will correctly play back inputs starting from whatever frame you go back to.

--TODO: Instead of 0-255 values, read 0-24 from file and convert to 0-255
local core = require "MKW_core"
local input_file = require "mkw_input_reader_output"
local starting_frame = core.getFrameOfInput()

function onScriptStart()
	MsgBox("Script started.")
end

function onScriptCancel()
	MsgBox("Script ended.")
end

function onScriptUpdate()
  currentFrame = core.getFrameOfInput() + 1
  
  inputs = input_file[currentFrame]
  horizInput = inputs.Horiz
  vertInput = inputs.Vert
  A = inputs.A
  B = inputs.B
  L = inputs.L
  DU = inputs.DU
  DD = inputs.DD
  DL = inputs.DL
  DR = inputs.DR
  if horizInput ~= 7 and vertInput ~= 7 then
    
  end
  
  if horizInput == nil then SetMainStickX(128)
  else
    if horizInput == 0 then SetMainStickX(59)
    elseif horizInput == 1 then SetMainStickX(68)
    elseif horizInput == 2 then SetMainStickX(77)
    elseif horizInput == 3 then SetMainStickX(86)
    elseif horizInput == 4 then SetMainStickX(95)
    elseif horizInput == 5 then SetMainStickX(104)
    elseif horizInput == 6 then SetMainStickX(112)
    elseif horizInput == 7 then SetMainStickX(128)
    elseif horizInput == 8 then SetMainStickX(152)
    elseif horizInput == 9 then SetMainStickX(161)
    elseif horizInput == 10 then SetMainStickX(170)
    elseif horizInput == 11 then SetMainStickX(179)
    elseif horizInput == 12 then SetMainStickX(188)
    elseif horizInput == 13 then SetMainStickX(197)
    elseif horizInput == 14 then SetMainStickX(205) end
  end
  
  if vertInput == nil then SetMainStickY(128)
  else
    if vertInput == 0 then SetMainStickY(59)
      elseif vertInput == 1 then SetMainStickY(68)
      elseif vertInput == 2 then SetMainStickY(77)
      elseif vertInput == 3 then SetMainStickY(86)
      elseif vertInput == 4 then SetMainStickY(95)
      elseif vertInput == 5 then SetMainStickY(104)
      elseif vertInput == 6 then SetMainStickY(112)
      elseif vertInput == 7 then SetMainStickY(128)
      elseif vertInput == 8 then SetMainStickY(152)
      elseif vertInput == 9 then SetMainStickY(161)
      elseif vertInput == 10 then SetMainStickY(170)
      elseif vertInput == 11 then SetMainStickY(179)
      elseif vertInput == 12 then SetMainStickY(188)
      elseif vertInput == 13 then SetMainStickY(197)
      elseif vertInput == 14 then SetMainStickY(205) end
  end
  if A == 1 then PressButton("A") end
  if B == 1 then PressButton("B") end
  if X == 1 then PressButton("X") end
  if Y == 1 then PressButton("Y") end
  if Z == 1 then PressButton("Z") end
  if L == 1 then PressButton("L") end
  if R == 1 then PressButton("R") end
  if S == 1 then PressButton("Start") end
  
  if DU == 1 then PressButton("D-Up") end
  if DD == 1 then PressButton("D-Down") end
  if DL == 1 then PressButton("D-Left") end
  if DR == 1 then PressButton("D-Right") end
  
end

function onStateLoaded()

end

function onStateSaved()

end
