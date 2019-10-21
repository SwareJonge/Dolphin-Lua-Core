----- GLOBAL VARIABLES -----
saidHello = false

--Add an underscore (_) to the beginning of the filename if you want the script to auto launch once you start a game!


function onScriptStart()
	MsgBox("Hello World")
	SetInfoDisplay()
end

function onScriptCancel()
	MsgBox("Bye :(")
end

function onScriptUpdate()
	CancelScript()

end

function onStateLoaded()

end

function onStateSaved()

end
