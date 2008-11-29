#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#if !defined(LUA_VERSION_NUM) || (LUA_VERSION_NUM < 501)
#include <compat-5.1.h>
#endif

#ifdef _WIN32
    #define LH_EXPORT __declspec(dllexport)
#else
    #define LH_EXPORT
#endif

int MD5File(lua_State *L);
int MD5String(lua_State *L);
int CRC32Int(lua_State *L);
int CRC32String(lua_State *L);
int SHA1File(lua_State *L);
int SHA1String(lua_State *L);

static const luaL_Reg lash_md5[] = {
    {"file2hex", MD5File},
    {"string2hex", MD5String},
    {NULL, NULL}
};

static const luaL_Reg lash_crc32[] = {
    {"string2num", CRC32Int},
    {"string2hex", CRC32String},
    {NULL, NULL}
};

static const luaL_Reg lash_sha1[] = {
    {"file2hex", SHA1File},
    {"string2hex", SHA1String},
    {NULL, NULL}
};

LH_EXPORT int luaopen_lash(lua_State *L) {
    luaL_register(L, "lash.MD5", lash_md5);
    luaL_register(L, "lash.CRC32", lash_crc32);
    luaL_register(L, "lash.SHA1", lash_sha1);

    return 1;
}
