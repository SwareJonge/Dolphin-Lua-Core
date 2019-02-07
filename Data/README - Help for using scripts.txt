How to execute scripts:

While the game is running, go to Tools - Execute Script. In the new window, select the desired script and click on Start. When you want to stop running the script, click on Cancel.

Important: note that closing the scripts window or changing the script file does NOT stop the current script to execute! Always remember to click on Cancel when you want to make the selected script stop running.

Scripts description:

SADX/SA2B_GetSpecificAngle.lua: makes the character maintain the angle that's specified at sadx/sa2b_target_angle.lua (can be edited manually or through SADX/SA2B_RecordAngle.lua)

SADX/SA2B_GoToPosition.lua: makes the character go towards the position specified at sadx/sa2b_position.lua (can be edited manually or through SADX/SA2B_RecordPosition.lua)

SADX/SA2B_KeepCurrentAngle.lua: maintais facing angle (Y rotation angle) while the script is running (if the facing angle changes during the execution of the script, the set point IS updated)

SADX/SA2B_KeepStartingAngle.lua: maintains the starting facing angle (Y rotation angle) while the script is running (if the facing angle changes during the execution of the script, the set point is NOT updated)

SADX/SA2B_RecordAngle.lua: records current facing angle (Y rotation angle) in sadx/sa2b_target_angle.lua

SADX/SA2B_RecordPosition.lua: records current position (X, Y and Z) in sadx/sa2b_position.lua

SADX/SA2B_TurnAndGetSpecificAngle.lua: makes the character maintain the angle that's specified at sadx/sa2b_target_angle.lua after doing a smooth turn towards that angle (without losing speed)

SADX/SA2B_TurnAndGoToPosition.lua: makes the character go towards the position specified at sadx/sa2b_position.lua after doing a smooth turn towards that position (without losing speed)

SADX/SA2B_TurnLeft.lua: optimally turns to the left without losing speed

SADX/SA2B_TurnRight.lua: optimally turns to the right without losing speed
