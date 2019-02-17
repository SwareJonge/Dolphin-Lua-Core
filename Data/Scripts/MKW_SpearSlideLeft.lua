----- GLOBAL VARIABLES -----
saidHello = false

--Add an underscore (_) to the beginning of the filename if you want the script to auto launch once you start a game!
local core = require "MKW_core"

function onScriptStart()
	MsgBox("Script Started")
end

function onScriptCancel()
	MsgBox("Script Ended")
end

function onScriptUpdate()
	if core.getFrameOfinput() <= 17  then SetMainStickX(255)                                                        -- +7  for 18 frames
	elseif core.getFrameOfinput() == 18 then SetMainStickX(255) PressButton("D-Up")                                 -- +7 And wheelie
	elseif core.getFrameOfinput() >= 19 and core.getFrameOfinput() < 39 then SetMainStickX(1)                       -- -7 for 20 frames
	elseif core.getFrameOfinput() >= 39 and core.getFrameOfinput() < 42 then SetMainStickX(255)                     -- +7 for 3 frames
	elseif core.getFrameOfinput() >= 42 and core.getFrameOfinput() < 58 then SetMainStickX(1)                       -- -7 for 16 frames
	elseif core.getFrameOfinput() >= 58 and core.getFrameOfinput() < 76 then SetMainStickX(255)                     -- +7 for 18 frames
	elseif core.getFrameOfinput() == 76 then SetMainStickX(255) PressButton("D-Up")                                 -- +7 And wheelie
  elseif core.getFrameOfinput() >= 77 and core.getFrameOfinput() < 96 then SetMainStickX(1)                       -- -7 for 19 frames
  elseif core.getFrameOfinput() == 96 then SetMainStickX(1) PressButton("D-Up")                                   -- -7 And wheelie
	elseif core.getFrameOfinput() >= 97 and core.getFrameOfinput() < 116 then SetMainStickX(255)                    -- +7 for 19 frames
	elseif core.getFrameOfinput() >= 116 and core.getFrameOfinput() < 133 then SetMainStickX(1)                     -- -7 for 17 frames
	elseif core.getFrameOfinput() >= 133 and core.getFrameOfinput() < 145 then SetMainStickX(1) PressButton("A")    -- -7 And A for 12 frames
	elseif core.getFrameOfinput() >= 145 and core.getFrameOfinput() < 163 then SetMainStickX(255) PressButton("A")  -- +7 And A for 18 frames
  elseif core.getFrameOfinput() == 163 then SetMainStickX(255) PressButton("D-Up") PressButton("A")               -- +7, A And wheelie
	elseif core.getFrameOfinput() >= 164 and core.getFrameOfinput() < 183 then SetMainStickX(1) PressButton("A")		-- -7 And A for 19 frames
  elseif core.getFrameOfinput() == 183 then SetMainStickX(1) PressButton("D-Up") PressButton("A")                 -- -7, A And wheelie
	elseif core.getFrameOfinput() >= 184 and core.getFrameOfinput() < 208 then SetMainStickX(255) PressButton("A")  -- +7 And A for 24 frames
  elseif core.getFrameOfinput() >= 208 and core.getFrameOfinput() < 240 then SetMainStickX(1) PressButton("A")	  -- -7 for the rest of the slide
  else CancelScript()
	end
end

function onStateLoaded()

end

function onStateSaved()

end
