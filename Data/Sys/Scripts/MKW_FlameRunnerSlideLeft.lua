
package.path = GetScriptsDir() .. "MKW/MKW_core.lua"
local core = require("MKW_core")

function onScriptStart()
	MsgBox("Script Started.")
end

function onScriptCancel()
	MsgBox("Script Ended.")
end
-- +7 = 255
-- -7 = 1
function onScriptUpdate()
	if core.getFrameOfInput() <= 12  then SetMainStickX(1) PressButton("A")                                           -- -7 and A for 12 frames
	elseif core.getFrameOfInput() == 13 then SetMainStickX(1) PressButton("D-Up") PressButton("A")                    -- -7, A And wheelie
	elseif core.getFrameOfInput() >= 14 and core.getFrameOfInput() < 33 then SetMainStickX(255) PressButton("A")      -- +7 and A for 19 frames
	elseif core.getFrameOfInput() == 33 then SetMainStickX(255) PressButton("A") PressButton("D-Up")                  -- +7, A and Wheelie
	elseif core.getFrameOfInput() >= 34 and core.getFrameOfInput() < 65 then SetMainStickX(255) PressButton("A")      -- +7 and A for 31 frames
	elseif core.getFrameOfInput() >= 65 and core.getFrameOfInput() < 84 then SetMainStickX(1) PressButton("A")        -- -7 and A for 19 frames
	elseif core.getFrameOfInput() == 84 then SetMainStickX(1) PressButton("D-Up")                                     -- -7 And wheelie
  elseif core.getFrameOfInput() >= 85 and core.getFrameOfInput() < 104 then SetMainStickX(255)                      -- +7 for 19 frames
  elseif core.getFrameOfInput() == 104 then SetMainStickX(255) PressButton("D-Up")                                  -- +7 And wheelie
	elseif core.getFrameOfInput() >= 105 and core.getFrameOfInput() < 127 then SetMainStickX(1)                       -- -7 for 22 frames
	elseif core.getFrameOfInput() >= 127 and core.getFrameOfInput() < 140 then SetMainStickX(255)                     -- +7 for 13 frames
	elseif core.getFrameOfInput() >= 140 and core.getFrameOfInput() < 147 then SetMainStickX(255) PressButton("A")    -- +7 And A for 7 frames
	elseif core.getFrameOfInput() == 147 then SetMainStickX(255) PressButton("A") PressButton("D-Up")                 -- +7 and A and Wheelie (1 Frame)
	elseif core.getFrameOfInput() >= 148 and core.getFrameOfInput() < 168 then SetMainStickX(255) PressButton("A")    -- +7 and A (20 Frames)
  elseif core.getFrameOfInput() == 168 then SetMainStickX(255) PressButton("A") PressButton("D-Up")                 -- +7 and A and Wheelie (1 Frame)
	elseif core.getFrameOfInput() >= 169 and core.getFrameOfInput() < 189 then SetMainStickX(255) PressButton("A")    -- +7 and A (20Frames)
  elseif core.getFrameOfInput() == 189 then SetMainStickX(255) PressButton("A") PressButton("D-Up")                 -- +7 and A and Wheelie (1 Frame)
	elseif core.getFrameOfInput() >= 190 and core.getFrameOfInput() < 210 then SetMainStickX(255) PressButton("A")    -- +7 and A (20 Frames)
  elseif core.getFrameOfInput() == 210 then SetMainStickX(255) PressButton("A") PressButton("D-Up")                 -- +7 and A and Wheelie (1 Frame)
	elseif core.getFrameOfInput() >= 211 and core.getFrameOfInput() < 231 then SetMainStickX(1) PressButton("A")      -- -7 and A (27 Frames)
	elseif core.getFrameOfInput() >= 231 and core.getFrameOfInput() < 241 then SetMainStickX(255) PressButton("A")    -- +7 and A (for the rest)
  else CancelScript()
	end
end

function onStateLoaded()

end

function onStateSaved()

end
