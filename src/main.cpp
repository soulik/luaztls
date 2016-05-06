/*
	LuaZTLS - Lua binding for ZTLS library

	Copyright 2016 Mário Kašuba
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are
	met:

	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "common.h"
#include "main.h"

namespace LuaZTLS {


#define getZTLSobject(n) *(static_cast<void**>(stack->to<void*>((n))))
#define pushUData(v) {void ** s = static_cast<void**>(stack->newUserData(sizeof(void*))); *s = (v); stack->newTable(); stack->setMetatable();}

	int lua_ztls_clientNew(lutok2::State & state){
		Stack * stack = state.stack;
		void * context = nullptr;

		if (stack->is<LUA_TUSERDATA>(1) && stack->is<LUA_TSTRING>(2)){
			const std::string data_endpoint = stack->to<const std::string>(2);
			if (stack->is<LUA_TSTRING>(3)){
				const std::string control_endpoint = stack->to<const std::string>(3);
				context = ztls_client_new_with_ctx(getZTLSobject(1), data_endpoint.c_str(), control_endpoint.c_str());
			}
			else{
				context = ztls_client_new_with_ctx(getZTLSobject(1), data_endpoint.c_str());
			}
		}else
		if (stack->is<LUA_TSTRING>(1)){
			const std::string data_endpoint = stack->to<const std::string>(1);
			if (stack->is<LUA_TSTRING>(2)){
				const std::string control_endpoint = stack->to<const std::string>(2);
				context = ztls_client_new(data_endpoint.c_str(), control_endpoint.c_str());
			}
			else{
				context = ztls_client_new(data_endpoint.c_str());
			}
		}


		if (!context){
			stack->push<bool>(false);
			stack->push<const std::string &>("Couldn't create ztls object");
			return 2;
		}
		else{
			pushUData(context);
			return 1;
		}
	}

	int lua_ztls_clientDestroy(lutok2::State & state){
		Stack * stack = state.stack;
		if (state.stack->is<LUA_TUSERDATA>(1)){
			bool result = ztls_client_destroy(getZTLSobject(1));
			state.stack->push<bool>(result);
			return 1;
		}
		return 0;
	}

	int lua_ztls_clientCAChain(lutok2::State & state){
		Stack * stack = state.stack;
		if (state.stack->is<LUA_TUSERDATA>(1) && stack->is<LUA_TSTRING>(2)){
			const std::string hostname = stack->toLString(2);

			bool result = ztls_client_CA_chain(getZTLSobject(1), hostname.c_str(), hostname.length());
			state.stack->push<bool>(result);
			return 1;
		}
		return 0;
	}

	int lua_ztls_clientConnect(lutok2::State & state){
		Stack * stack = state.stack;
		if (state.stack->is<LUA_TUSERDATA>(1) && stack->is<LUA_TSTRING>(2) && stack->is<LUA_TNUMBER>(3)){
			const std::string hostname = stack->to<const std::string>(2);
			uint16_t port = stack->to<int>(3);
			bool result = ztls_client_connect(getZTLSobject(1), hostname.c_str(), port);
			state.stack->push<bool>(result);
			return 1;
		}
		return 0;
	}

	int lua_ztls_clientClose(lutok2::State & state){
		Stack * stack = state.stack;
		if (state.stack->is<LUA_TUSERDATA>(1)){
			bool result = ztls_client_close(getZTLSobject(1));
			state.stack->push<bool>(result);
			return 1;
		}
		return 0;
	}

};

extern "C" LIBLUAZTLS_DLL_EXPORTED int luaopen_luaztls(lua_State * L){
	State * state = new State(L);
	Stack * stack = state->stack;
	Module luaztls_module;

	stack->newTable();
	
	luaztls_module["clientNew"] = LuaZTLS::lua_ztls_clientNew;
	luaztls_module["clientDestroy"] = LuaZTLS::lua_ztls_clientDestroy;
	luaztls_module["clientConnect"] = LuaZTLS::lua_ztls_clientConnect;
	luaztls_module["clientClose"] = LuaZTLS::lua_ztls_clientClose;
	luaztls_module["clientCAChain"] = LuaZTLS::lua_ztls_clientCAChain;

	state->registerLib(luaztls_module);
	return 1;
}