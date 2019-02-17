local core = require "SMG_Core"

function onScriptStart()
	MsgBox("Hello Xander!")
end

function onScriptCancel()
	MsgBox("Thanks For using this script! If there are any bugs, message SwareJonge#2388") SetScreenText("")
end

function onScriptUpdate()
  Yvel = core.getYVel()
  XZSpeed = core.getXZSpeed()
  XYZSpeed = core.getXYZSpeed()
  SetScreenText(string.format("\n\n\n\n\n\n\n\n\n\n\n\nY Vel: %f \nXZ Speed: %f \nXYZ Speed: %f", Yvel, XZSpeed, XYZSpeed))
end

function onStateLoaded()

end

function onStateSaved()

end
