local core = require "sonic_core"

function onScriptStart()
	if core.getGameID() ~= "GSNE8P" and core.getGameID() ~= "GXSE8P" and core.getGameID() ~= "G9SE8P" then
		SetScreenText("")
		CancelScript()
	end
end

function onScriptCancel()

end

function onScriptUpdate()
	
	local text = "\n\n\n===== Speed ====="
	text = text .. string.format("\nFw: %10.6f | Vt: %10.6f | Sd: %10.6f", core.getSpd().F, core.getSpd().V, core.getSpd().Sd)
	text = text .. string.format("\nX: %10.6f | Y: %10.6f | Z: %10.6f", core.getSpd().X, core.getSpd().Y, core.getSpd().Z)
	
	text = text .. "\n"
	
	if core.getGameID() == "GSNE8P" or core.getGameID() == "GXSE8P" then
		text = text .. string.format("St: %10.6f | ", core.getSpd().St)
	end
	
	local xz_spd = math.sqrt(core.getSpd().X*core.getSpd().X + core.getSpd().Z * core.getSpd().Z)
	local xyz_spd = math.sqrt(core.getSpd().X*core.getSpd().X + core.getSpd().Y * core.getSpd().Y + core.getSpd().Z * core.getSpd().Z)
	
	text = text .. string.format("XZ: %10.6f | XYZ: %10.6f", xz_spd, xyz_spd)
	
	text = text .. "\n\n===== Position ====="
	text = text .. string.format("\nX: %12.6f | Y: %12.6f | Z: %12.6f", core.getPos().X, core.getPos().Y, core.getPos().Z)
	
	text = text .. "\n\n===== Rotation ====="
	text = text .. string.format("\nX: %05d (%6.2f deg)\nY: %05d (%6.2f deg)\nZ: %05d (%6.2f deg)", core.getRot().X, core.getRot().X * 360 / 65536, core.getRot().Y, core.getRot().Y * 360 / 65536, core.getRot().Z, core.getRot().Z * 360 / 65536)
	
	local grav_angle = math.deg(math.acos(math.cos(math.rad(core.getRot().X * 360 / 65536)) * math.cos(math.rad(core.getRot().Z * 360 / 65536))))
	
	text = text .. string.format("\nGravity: %6.2f deg", grav_angle)
	
	text = text .. string.format("\nFinal Y:  %05d (%6.2f deg)", core.getFinalRot().Y, core.getFinalRot().Y * 360 / 65536)
	
	text = text .. "\n\n===== Misc ====="
	text = text .. string.format("\nAction: %d | Hover: %d | Status: ", core.getAction(), core.getHover())
	
	status = core.getStatus()
	
	for i = 0,15 do
		text = text .. string.format("%d", status%2)
		status = (status - status%2) / 2
	end
	
	SetScreenText(text)
	
end