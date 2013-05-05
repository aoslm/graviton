/**
 * @file
 *
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
 *
 * This file is part of GraVitoN.
 *
 * Graviton is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Graviton is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Graviton.  If not, see http://www.gnu.org/licenses/.
 *
 * @brief GraVitoN::Lua
 *
*/

#ifndef _GVN_LIBLUA_HEAD_
#define _GVN_LIBLUA_HEAD_

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//

#ifdef OPT_LUA_52
    #include <external/liblua/lua.hpp>
    #include <external/liblua/llimits.h>
#else
    #include <external/lua.hpp>
    #include <external/lua5.1/llimits.h>

    #define LUA_OK 0
#endif

#include <graviton.hpp>
#include <core/logger.hpp>
using namespace std;

namespace GraVitoN
{
namespace Core
{
namespace Lua
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
typedef struct LuaObj
{
    lua_State *lua_state;

    LuaObj()
    {
        lua_state = _null_;
    }
} & R_LuaObj;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo free: Double Free Exception
/*
bool free(R_LuaObj obj)
{
	// Double Free Exception
	if( obj.lua_state )
	{
		lua_close ( obj.lua_state );
		return true;
	}
	else
	{
		return false;
    }
}
*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool initialize(R_LuaObj obj)
{
    if ( obj.lua_state == _null_ )
	{
        obj.lua_state = luaL_newstate();

        luaL_openlibs( obj.lua_state );

        // Lua 5.1
        // luaopen_base(obj.lua_state);
        // luaopen_debug(obj.lua_state);
        // luaopen_io(obj.lua_state);
        // luaopen_math(obj.lua_state);
        // luaopen_package(obj.lua_state);
        // luaopen_string(obj.lua_state);
        // luaopen_table(obj.lua_state);

        // Lua 5.2
        // luaopen_base ( obj.lua_state );
        // luaopen_coroutine ( obj.lua_state );
        // luaopen_table ( obj.lua_state );
        // luaopen_io ( obj.lua_state );
        // luaopen_os(obj.lua_state);
        // luaopen_string ( obj.lua_state );
        // luaopen_bit32 ( obj.lua_state );
        // luaopen_math ( obj.lua_state );
        // // luaopen_debug ( obj.lua_state );
        // luaopen_package ( obj.lua_state );
	}
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool runScript(R_LuaObj obj, const int prev_err = LUA_OK)
{
	if ( prev_err != LUA_OK )
	{
		lua_pop(obj.lua_state, 1);
		Logger::logIt("------------------ Lua Error Code: ");
		Logger::logIt(prev_err);
		Logger::logItLn(" ------------------");
		return false;
	}

	int err = lua_pcall (obj.lua_state, 0, LUA_MULTRET, 0 );
	lua_pop(obj.lua_state, 1);

	if( err != LUA_OK )
	{
		Logger::logIt("------------------ Lua Error Code: ");
		Logger::logIt(err);
		Logger::logItLn(" ------------------");
		return false;
	}
	else
	{
		Logger::logItLn("------------------ Lua Executed Successfuly ------------------");
	}

	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int loadModuleFile (R_LuaObj obj, const string &_file )
{
    Logger::logVariable("File",_file);
    return luaL_loadfile (obj.lua_state, _file.c_str() );
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int loadModuleString (R_LuaObj obj, const string &_script )
{
    return luaL_loadstring ( obj.lua_state, _script.c_str() );
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int runScriptFile (R_LuaObj obj, const string &_file )
{
    return runScript(obj, loadModuleFile(obj, _file));
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int runScriptString (R_LuaObj obj, const string &_script )
{
    return runScript(obj, loadModuleString(obj, _script));
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
void preloadModule (R_LuaObj obj, const string &module_name, lua_CFunction function_addr)
{
    lua_getfield(obj.lua_state, LUA_GLOBALSINDEX, "package");
    lua_getfield(obj.lua_state, -1, "preload");
    lua_pushcfunction(obj.lua_state, function_addr);
    lua_setfield(obj.lua_state, -2, module_name.c_str());
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool regiserFunction (R_LuaObj obj, const string& func_name, const lua_CFunction func_addr )
{
    lua_register (obj.lua_state, func_name.c_str(), func_addr );
    return true;
}


} // Lua
} // Core
} // GraVitoN

#endif // _GVN_LUA_HEAD_
