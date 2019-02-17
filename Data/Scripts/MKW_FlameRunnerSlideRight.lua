
local core = require "MKW_core"

function onScriptStart()
	MsgBox("Script Started.")
end

function onScriptCancel()
	MsgBox("Script Ended.")
end
-- +7 = 255
-- -7 = 1
function onScriptUpdate()
	if core.getFrameOfinput() <= 12  then SetMainStickX(255) PressButton("A")                                       -- +7 and A for 12 frames
	elseif core.getFrameOfinput() == 13 then SetMainStickX(255) PressButton("D-Up") PressButton("A")                -- +7, A And wheelie
	elseif core.getFrameOfinput() >= 14 and core.getFrameOfinput() < 33 then SetMainStickX(1) PressButton("A")      -- -7 and A for 19 frames
	elseif core.getFrameOfinput() == 33 then SetMainStickX(1) PressButton("A") PressButton("D-Up")                  -- -7, A and Wheelie
	elseif core.getFrameOfinput() >= 34 and core.getFrameOfinput() < 65 then SetMainStickX(1) PressButton("A")      -- -7 and A for 31 frames
	elseif core.getFrameOfinput() >= 65 and core.getFrameOfinput() < 84 then SetMainStickX(255) PressButton("A")    -- +7 and A for 19 frames
	elseif core.getFrameOfinput() == 84 then SetMainStickX(255) PressButton("D-Up")                                 -- +7 And wheelie
  elseif core.getFrameOfinput() >= 85 and core.getFrameOfinput() < 104 then SetMainStickX(1)                      -- -7 for 19 frames
  elseif core.getFrameOfinput() == 104 then SetMainStickX(1) PressButton("D-Up")                                  -- -7 And wheelie
	elseif core.getFrameOfinput() >= 105 and core.getFrameOfinput() < 127 then SetMainStickX(255)                   -- +7 for 22 frames
	elseif core.getFrameOfinput() >= 127 and core.getFrameOfinput() < 140 then SetMainStickX(1)                     -- -7 for 14 frames
	elseif core.getFrameOfinput() >= 140 and core.getFrameOfinput() < 147 then SetMainStickX(1) PressButton("A")    -- -7 And A for 6 frames
	elseif core.getFrameOfinput() == 147 then SetMainStickX(1) PressButton("A") PressButton("D-Up")                 -- -7 and A and Wheelie (1 Frame)
	elseif core.getFrameOfinput() >= 148 and core.getFrameOfinput() < 168 then SetMainStickX(1) PressButton("A")    -- -7 and A (20 Frames)
  elseif core.getFrameOfinput() == 168 then SetMainStickX(1) PressButton("A") PressButton("D-Up")                 -- -7 and A and Wheelie (1 Frame)
	elseif core.getFrameOfinput() >= 169 and core.getFrameOfinput() < 189 then SetMainStickX(1) PressButton("A")    -- -7 and A (20Frames)
  elseif core.getFrameOfinput() == 189 then SetMainStickX(1) PressButton("A") PressButton("D-Up")                 -- -7 and A and Wheelie (1 Frame)
	elseif core.getFrameOfinput() >= 190 and core.getFrameOfinput() < 210 then SetMainStickX(1) PressButton("A")    -- -7 and A (20 Frames)
  elseif core.getFrameOfinput() == 210 then SetMainStickX(1) PressButton("A") PressButton("D-Up")                 -- -7 and A and Wheelie (1 Frame)
	elseif core.getFrameOfinput() >= 211 and core.getFrameOfinput() < 231 then SetMainStickX(255) PressButton("A")    -- +7 and A (27 Frames)
	elseif core.getFrameOfinput() >= 231 and core.getFrameOfinput() < 241 then SetMainStickX(1) PressButton("A")    -- -7 and A (for the rest)
  else CancelScript()
	end
end

function onStateLoaded()

end

function onStateSaved()

end
