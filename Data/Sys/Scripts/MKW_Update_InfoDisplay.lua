----- GLOBAL VARIABLES -----
local core = require "MKW_core"
local Pointers = require "MKW_Pointers"
  if core.getGameID() == "RMCP01" then file = io.open("Sys/InfoDisplay/RMCP01.ini", "w")
	elseif core.getGameID() == "RMCE01"then file = io.open("Sys/InfoDisplay/RMCE01.ini", "w")
	elseif core.getGameID() == "RMCJ01"then file = io.open("Sys/InfoDisplay/RMCJ01.ini", "w")
	elseif core.getGameID() == "RMCK01"then	file = io.open("Sys/InfoDisplay/RMCK01.ini", "w")
	end
io.output(file)
--Add an underscore (_) to the beginning of the filename if you want the script to auto launch once you start a game!

function onScriptStart()
	if core.getGameID() == "RMCP01" then MsgBox("Writing pointers to RMCP01.ini")
	elseif core.getGameID() == "RMCE01"then MsgBox("Writing pointers to RMCE01.ini")
	elseif core.getGameID() == "RMCJ01"then MsgBox("Writing pointers to RMCJ01.ini")
	elseif core.getGameID() == "RMCK01"then MsgBox("Writing pointers to RMCK01.ini")
	end
  io.write("\nLine1=\"\"")
  io.write("\nArg1=nil;")
  io.write("\nEnd Line\n")

	io.write("Line2=\"===== Position =====")
  io.write("\nX: %f4b")
  io.write("\nY: %f4b")
  io.write("\nZ: %f4b\"")
  io.write(string.format("\nArg1=0x%X + 0x0", Pointers.getPositionPointer()))
  io.write(string.format("\nArg2=0x%X + 0x4", Pointers.getPositionPointer()))
  io.write(string.format("\nArg3=0x%X + 0x8", Pointers.getPositionPointer()))
	io.write("\nEnd Line\n")

	io.write("\nLine3=\"\"")
	io.write("\nArg1=nil;")
	io.write("\nEnd Line\n")

	io.write("\nLine4=\"===== Checkpoint =====")
	io.write("\nCKPT: %d2b")
	io.write("\nKPT:  %d1b\"")
	io.write(string.format("\nArg1=0x%X + 0xA", Pointers.getRaceData2Pointer()))
  io.write(string.format("\nArg2=0x%X + 0x27", Pointers.getRaceData2Pointer()))
	io.write("\nEnd Line\n")

  io.write("\nLine5=\"\"")
	io.write("\nArg1=nil;")
	io.write("\nEnd Line\n")

	io.write("\nLine6=\"===== Misc =====")
	io.write("\nBase Vel: %f4b")
  io.write("\nMT Charge: %d2b")
  io.write("\nSSMT Charge: %d2b")
  io.write("\nAirtime: %d2b")
  io.write("\nMT Boost: %d2b")
  io.write("\nTrick Boost: %d2b")
	io.write("\nMushroom Boost:  %d2b\"")
	io.write(string.format("\nArg1=0x%X + 0x24", Pointers.getPlayerBasePointer()))
  io.write(string.format("\nArg2=0x%X + 0xFE", Pointers.getPlayerBasePointer()))
  io.write(string.format("\nArg3=0x%X + 0x14C", Pointers.getPlayerBasePointer()))
  io.write(string.format("\nArg4=0x%X + 0x21A", Pointers.getPlayerBasePointer()))
  io.write(string.format("\nArg5=0x%X + 0x10C", Pointers.getPlayerBasePointer()))
  io.write(string.format("\nArg6=0x%X + 0x114", Pointers.getPlayerBasePointer()))
  io.write(string.format("\nArg7=0x%X + 0x110", Pointers.getPlayerBasePointer()))
	io.write("\nEnd Line\n")

	CancelScript()
end

function onScriptCancel()
	if core.getGameID() == "RMCP01" then MsgBox("Pointers have been written to RMCP01.ini")
	elseif core.getGameID() == "RMCE01"then MsgBox("Pointers have been written to RMCE01.ini")
	elseif core.getGameID() == "RMCJ01"then MsgBox("Pointers have been written to RMCJ01.ini")
	elseif core.getGameID() == "RMCK01"then MsgBox("Pointers have been written to RMCK01.ini")
	end
end

function onScriptUpdate()

end
