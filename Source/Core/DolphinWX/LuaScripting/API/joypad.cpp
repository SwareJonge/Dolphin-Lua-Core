// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "DolphinWX/LUA_API.h"
#include "DolphinWX/LuaScripting.h"
#include "Core/Movie.h"

namespace Lua
{
	namespace joypad
	{
		namespace GC
		{        
			// Helper Function
			void iPressButton(const char *button, int number)
			{
				if (!strcmp(button, "A"))
				{					
					LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_A;
					LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).analogA = 0xFF;
				}	        
				else if (!strcmp(button, "B"))
				{
					LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_B;
					LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).analogB = 0xFF;
				}
				else if (!strcmp(button, "X"))
				{
					LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_X;
				}
				else if (!strcmp(button, "Y"))
				{
					LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_Y;
				}
				else if (!strcmp(button, "Z"))
				{
					LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_TRIGGER_Z;
				}
				else if (!strcmp(button, "Start"))
				{
					LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_START;
				}
				else if (!strcmp(button, "D-Up"))
				{
					LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_UP;
				}
	            else if (!strcmp(button, "D-Down"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_DOWN;
	            }
	            else if (!strcmp(button, "D-Left"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_LEFT;
	            }
	            else if (!strcmp(button, "D-Right"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).button |= PadButton::PAD_BUTTON_RIGHT;
	            }
			}

			int getAnalog(lua_State *L)
			{
				int number = lua_tointeger(L, 1) - 1;
				lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastPadStatus(number).stickX);
				lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastPadStatus(number).stickY);
				return 2;
			}

			int setAnalog(lua_State *L)
            {
	            if (Movie::IsPlayingInput())
		            return 0;

	            int number = lua_tointeger(L, 1) - 1;
	            if (lua_gettop(L) != 3)
	            {
		            return luaL_error(L, "Incorrect # of arguments passed to setAnalog. setAnalog expects 3 arguments/n");
	            }

	            u8 x_pos = lua_tointeger(L, 2);
	            u8 y_pos = lua_tointeger(L, 3);

	            LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).stickX = x_pos;
	            LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).stickY = y_pos;

	            return 0;
            }
			// Same thing as setAnalog except it takes polar coordinates
            // Must use an m int the range [0, 128)
            int setAnalogPolar(lua_State *L)
            {
	            if (Movie::IsPlayingInput())
		            return 0;

	            int number = lua_tointeger(L, 1) - 1;
	            int m = lua_tointeger(L, 2);
	            if (m < 0 || m >= 128)
	            {
		            return luaL_error(L, "m is outside of acceptable range [0, 128)");
	            }

	            // Gotta convert theta to radians
	            double theta = lua_tonumber(L, 3) * M_PI / 180.0;

	            // Round to the nearest whole number, then subtract 128 so that our
	            //"origin" is the stick in neutral position.
	            LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).stickX =
	                static_cast<u8>(floor(m * cos(theta)) + 128);
	            LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).stickY =
	                static_cast<u8>(floor(m * sin(theta)) + 128);

	            return 0;
            }

			int getCStick(lua_State *L)
            {
	            int number = lua_tointeger(L, 1) - 1;
	            lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastPadStatus(number).substickX);
	            lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastPadStatus(number).substickY);

	            return 2;
            }

			int setCStick(lua_State *L)
            {
	            if (Movie::IsPlayingInput())
		            return 0;

	            int number = lua_tointeger(L, 1) - 1;
	            LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).substickX = lua_tointeger(L, 2);
	            LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).substickY = lua_tointeger(L, 3);

	            return 0;
            }

			int setCStickPolar(lua_State *L)
            {
	            if (Movie::IsPlayingInput())
		            return 0;

	            int number = lua_tointeger(L, 1) - 1;
	            int m = lua_tointeger(L, 2);
	            if (m < 0 || m >= 128)
	            {
		            return luaL_error(L, "m is outside of acceptable range [0, 128)");
	            }

	            int theta = lua_tointeger(L, 3);
	            // Convert theta to radians
	            theta = theta * M_PI / 180.0;

	            LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).substickX =
	                (u8)(floor(m * cos(theta)) + 128);
	            LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).substickY =
	                (u8)(floor(m * sin(theta)) + 128);

	            return 0;
            }

            int getButtons(lua_State *L)
            {
	            int number = lua_tointeger(L, 1) - 1;
	            lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastPadStatus(number).button);
	            return 1;
            }

			int setButtons(lua_State *L)
            {
	            if (Movie::IsPlayingInput())
		            return 0;

					int argc = lua_gettop(L);					
					if (argc < 2)
						return 0;
					
					int number = lua_tointeger(L, 1) - 1;
	                const char *button = lua_tostring(L, 2);
	                iPressButton(button, number);
	                return 0;
			}

			int getTriggers(lua_State *L)
            {
	            int number = lua_tointeger(L, 1) - 1;
	            lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastPadStatus(number).triggerLeft);
	            lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastPadStatus(number).triggerRight);

	            return 2;
            }

			int setTriggers(lua_State *L)
            {
	            if (Movie::IsPlayingInput())
		            return 0;

	            int number = lua_tointeger(L, 1) - 1;
	            LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).triggerLeft = lua_tointeger(L, 2);
	            LuaScriptFrame::GetCurrentInstance()->GetPadStatus(number).triggerRight = lua_tointeger(L, 3);

	            return 0;
            }
		}

		namespace Wii
		{
			void iPressButton(const char *button, int number)
			{
				if (!strcmp(button, "A"))
				{
		            LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::BUTTON_A;
				}
	            if (!strcmp(button, "B"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::BUTTON_B;
	            }
	            if (!strcmp(button, "-") || !strcmp(button, "MINUS"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::BUTTON_MINUS;
	            }
	            if (!strcmp(button, "+") || !strcmp(button, "PLUS"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::BUTTON_PLUS;
	            }
	            if (!strcmp(button, "1") || !strcmp(button, "ONE"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::BUTTON_ONE;
	            }
	            if (!strcmp(button, "2") || !strcmp(button, "TWO"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::BUTTON_TWO;
	            }
	            if (!strcmp(button, "D-Up"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::PAD_UP;
	            }
	            if (!strcmp(button, "D-Down"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::PAD_DOWN;
	            }
	            if (!strcmp(button, "D-Left"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::PAD_LEFT;
	            }
	            if (!strcmp(button, "D-Right"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetPadWiiStatus(number).hex |= WiimoteEmu::Wiimote::PAD_RIGHT;
	            }
			}

			int getExtension(lua_State *L)
			{
	            int number = lua_tointeger(L, 1) - 1;
	            int ext = LuaScriptFrame::GetCurrentInstance()->GetLastWiimoteExt(number);
	            std::string wiimoteExt = "None"; 
				if (ext == 1)
				{
		            std::string wiimoteExt = "Nunchuk";
				}
	            else if (ext == 2)
	            {
		            std::string wiimoteExt = "Classic";
	            }

	            lua_pushstring(L, wiimoteExt.c_str());
	            return 1;
			}

		    int getButtons(lua_State *L)
			{
	            int number = lua_tointeger(L, 1) - 1;
	            lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastPadWiiStatus(number).hex);
	            return 1;
			}
        
			int setButtons(lua_State *L)
			{
	            if (Movie::IsPlayingInput())
		            return 0;

				int argc = lua_gettop(L);
	            if (argc < 2)
		            return 0;

				int number = lua_tointeger(L, 1) - 1;	        
				const char *button = lua_tostring(L, 2);
	            iPressButton(button, number);
	            return 0;
			}

			namespace Nunchuk
			{            
				int getButtons(lua_State *L)            
				{	            
					int number = lua_tointeger(L, 1) - 1;	            
					lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastNunchuckStatus(number).bt.hex);	            
					return 1;
				}

				int setButtons(lua_State *L)
				{
	                if (Movie::IsPlayingInput())
		                return 0;

	                int argc = lua_gettop(L);
	                if (argc < 2)
		                return 0;

	                int number = lua_tointeger(L, 1) - 1;
	                const char *s = lua_tostring(L, 2);

	                for (size_t i = 0; i < strlen(s); i++)
	                {
		                switch (s[i])
		                {
		                case 'C':
			                LuaScriptFrame::GetCurrentInstance()->GetNunchukStatus(number).bt.hex |= WiimoteEmu::Nunchuk::BUTTON_C;
			                break;
		                case 'Z':
			                LuaScriptFrame::GetCurrentInstance()->GetNunchukStatus(number).bt.hex |= WiimoteEmu::Nunchuk::BUTTON_Z;
			                break;
		                }
	                }
	                return 0;
				}

				int getAnalog(lua_State *L)
                {
	                int number = lua_tointeger(L, 1) - 1;
	                lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastNunchuckStatus(number).jx);
	                lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastNunchuckStatus(number).jy);
	                return 2;
                }

				int setAnalog(lua_State *L)
                {
	                if (Movie::IsPlayingInput())
		                return 0;
	                
	                if (lua_gettop(L) != 3)
	                {
		                return luaL_error(L, "Incorrect # of arguments passed to setAnalog. setAnalog expects 3 arguments/n");
	                }
	                int number = lua_tointeger(L, 1) - 1;
	                LuaScriptFrame::GetCurrentInstance()->GetNunchukStatus(number).jx = lua_tonumber(L, 2);
	                LuaScriptFrame::GetCurrentInstance()->GetNunchukStatus(number).jy = lua_tonumber(L, 3);

	                return 0;
                }

			} // namespace Nunchuk
            namespace Classic
            {
            void iPressButton(const char *button, int number)
            {
	            if (!strcmp(button, "A"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).bt.hex |=
		                WiimoteEmu::Classic::BUTTON_A;
	            }
	            if (!strcmp(button, "B"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).bt.hex |=
		                WiimoteEmu::Classic::BUTTON_B;
	            }
	            if (!strcmp(button, "-") || !strcmp(button, "MINUS"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).bt.hex |=
		                WiimoteEmu::Classic::BUTTON_MINUS;
	            }
	            if (!strcmp(button, "HOME"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).bt.hex |=
		                WiimoteEmu::Classic::BUTTON_MINUS;
	            }
	            if (!strcmp(button, "+") || !strcmp(button, "PLUS"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).bt.hex |=
		                WiimoteEmu::Classic::BUTTON_PLUS;
	            }
	            if (!strcmp(button, "X") || !strcmp(button, "ONE"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).bt.hex |=
		                WiimoteEmu::Classic::BUTTON_X;
	            }
	            if (!strcmp(button, "Y") || !strcmp(button, "TWO"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).bt.hex |=
		                WiimoteEmu::Classic::BUTTON_Y;
	            }
	            if (!strcmp(button, "L"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).bt.hex |=
		                WiimoteEmu::Classic::TRIGGER_L;
	            }
	            if (!strcmp(button, "R"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).bt.hex |=
		                WiimoteEmu::Classic::TRIGGER_R;
	            }
	            if (!strcmp(button, "ZL"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).bt.hex |=
		                WiimoteEmu::Classic::BUTTON_ZL;
	            }
	            if (!strcmp(button, "ZR"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).bt.hex |=
		                WiimoteEmu::Classic::BUTTON_ZR;
	            }
	            if (!strcmp(button, "D-Up"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).bt.hex |=
		                WiimoteEmu::Classic::PAD_UP;
	            }
	            if (!strcmp(button, "D-Down"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).bt.hex |=
		                WiimoteEmu::Classic::PAD_DOWN;
	            }
	            if (!strcmp(button, "D-Left"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).bt.hex |=
		                WiimoteEmu::Classic::PAD_LEFT;
	            }
	            if (!strcmp(button, "D-Right"))
	            {
		            LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).bt.hex |=
		                WiimoteEmu::Classic::PAD_RIGHT;
	            }
            }

				int getButtons(lua_State *L)            
				{
					int number = lua_tointeger(L, 1) - 1;
					lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastClassicStatus(number).bt.hex);
					return 1;
				}

				int setButtons(lua_State *L)
                {
	                if (Movie::IsPlayingInput())
		                return 0;
	                int number = lua_tointeger(L, 1) - 1; 
					const char *button = lua_tostring(L, 2);

					iPressButton(button, number);

	                return 0;
				}

				int getAnalogL(lua_State *L)
				{
	                int number = lua_tointeger(L, 1) - 1;
	                lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastClassicStatus(number).regular_data.lx);
	                lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastClassicStatus(number).regular_data.ly);
	                return 2;
				}

				int setAnalogL(lua_State *L)
				{
	                if (Movie::IsPlayingInput())
		                return 0;

	                if (lua_gettop(L) != 3)
	                {
		                return luaL_error(L, "Incorrect # of arguments passed to setAnalog. setAnalog expects 3 arguments/n");
	                }
	                int number = lua_tointeger(L, 1) - 1;
	                LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).regular_data.lx = lua_tonumber(L, 2);
	                LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).regular_data.ly = lua_tonumber(L, 3);
	                return 0;
				}

				int getAnalogR(lua_State *L)
                {
	                int number = lua_tointeger(L, 1) - 1;
	                int rx =
	                    (LuaScriptFrame::GetCurrentInstance()->GetLastClassicStatus(number).rx1 |
	                    (LuaScriptFrame::GetCurrentInstance()->GetLastClassicStatus(number).rx2 << 1) |
	                    (LuaScriptFrame::GetCurrentInstance()->GetLastClassicStatus(number).rx3 << 3));
	                lua_pushinteger(L, rx);
	                lua_pushinteger(L, LuaScriptFrame::GetCurrentInstance()->GetLastClassicStatus(number).ry);
	                return 2;
                }

				int setAnalogR(lua_State *L)
				{
	                if (Movie::IsPlayingInput())
		                return 0;

	                if (lua_gettop(L) != 3)
	                {
		                return luaL_error(
		                    L, "Incorrect # of arguments passed to setAnalog. setAnalog expects 3 arguments/n");
	                }
	                int number = lua_tointeger(L, 1) - 1;
	                int rx = lua_tointeger(L, 2);
	                LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).rx1 = rx & 0x1;
	                LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).rx2 = (rx >> 1) & 0x3;
	                LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).rx3 = (rx >> 3) & 0x3;
	                LuaScriptFrame::GetCurrentInstance()->GetClassicStatus(number).ry = lua_tointeger(L, 3);
	                return 0;
				}


			}
					   
		}

	}

}
	