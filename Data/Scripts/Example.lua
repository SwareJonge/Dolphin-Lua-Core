----- GLOBAL VARIABLES -----
saidHello = false

--Add an underscore (_) to the beginning of the filename if you want the script to auto launch once you start a game!


function onScriptStart()
	MsgBox("Hello World")
end

function onScriptCancel()
	MsgBox("Bye :(")
end

function onScriptUpdate()
	if saidHello == false and GetFrameCount() == 300 then
		saidHello = true
		MsgBox("Test 12, 123")
	end
end

function onStateLoaded()
	
end

function onStateSaved()
	
end