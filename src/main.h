namespace LuaZMQ {
	extern "C" LIBLUAZTLS_DLL_EXPORTED int luaopen_luaztls(lua_State *);

	int lua_ztls_clientNew(State &);
	int lua_ztls_clientDestroy(State &);
	int lua_ztls_clientConnect(State &);
	int lua_ztls_clientClose(State &);
	int lua_ztls_clientCAChain(State &);
};
