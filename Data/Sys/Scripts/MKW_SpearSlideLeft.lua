package.path = GetScriptsDir() .. "MKW/MKW_core.lua"
local core = require("MKW_core")

function onScriptStart()
	MsgBox("Script Started")
end

function onScriptCancel()
	MsgBox("Script Ended")
end

function onScriptUpdate()
	if core.getFrameOfInput() <= 17  then SetMainStickX(255)                                                        -- +7  for 18 frames
	elseif core.getFrameOfInput() == 18 then SetMainStickX(255) PressButton("D-Up")                                 -- +7 And wheelie
	elseif core.getFrameOfInput() >= 19 and core.getFrameOfInput() < 39 then SetMainStickX(1)                       -- -7 for 20 frames
	elseif core.getFrameOfInput() >= 39 and core.getFrameOfInput() < 42 then SetMainStickX(255)                     -- +7 for 3 frames
	elseif core.getFrameOfInput() >= 42 and core.getFrameOfInput() < 58 then SetMainStickX(1)                       -- -7 for 16 frames
	elseif core.getFrameOfInput() >= 58 and core.getFrameOfInput() < 76 then SetMainStickX(255)                     -- +7 for 18 frames
	elseif core.getFrameOfInput() == 76 then SetMainStickX(255) PressButton("D-Up")                                 -- +7 And wheelie
  elseif core.getFrameOfInput() >= 77 and core.getFrameOfInput() < 96 then SetMainStickX(1)                       -- -7 for 19 frames
  elseif core.getFrameOfInput() == 96 then SetMainStickX(1) PressButton("D-Up")                                   -- -7 And wheelie
	elseif core.getFrameOfInput() >= 97 and core.getFrameOfInput() < 116 then SetMainStickX(255)                    -- +7 for 19 frames
	elseif core.getFrameOfInput() >= 116 and core.getFrameOfInput() < 133 then SetMainStickX(1)                     -- -7 for 17 frames
	elseif core.getFrameOfInput() >= 133 and core.getFrameOfInput() < 145 then SetMainStickX(1) PressButton("A")    -- -7 And A for 12 frames
	elseif core.getFrameOfInput() >= 145 and core.getFrameOfInput() < 163 then SetMainStickX(255) PressButton("A")  -- +7 And A for 18 frames
  elseif core.getFrameOfInput() == 163 then SetMainStickX(255) PressButton("D-Up") PressButton("A")               -- +7, A And wheelie
	elseif core.getFrameOfInput() >= 164 and core.getFrameOfInput() < 183 then SetMainStickX(1) PressButton("A")		-- -7 And A for 19 frames
  elseif core.getFrameOfInput() == 183 then SetMainStickX(1) PressButton("D-Up") PressButton("A")                 -- -7, A And wheelie
	elseif core.getFrameOfInput() >= 184 and core.getFrameOfInput() < 208 then SetMainStickX(255) PressButton("A")  -- +7 And A for 24 frames
  elseif core.getFrameOfInput() >= 208 and core.getFrameOfInput() < 240 then SetMainStickX(1) PressButton("A")	  -- -7 for the rest of the slide
  else CancelScript()
	end
end

function onStateLoaded()

end

function onStateSaved()

end
