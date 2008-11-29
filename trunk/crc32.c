/*
 * Copyright (c) 2008 Neil Richardson (nrich@iinet.net.au)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 */

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#if !defined(LUA_VERSION_NUM) || (LUA_VERSION_NUM < 501)
#include <compat-5.1.h>
#endif

static unsigned int CRC32[] = {
    0,79764919,159529838,222504665,319059676,
    398814059,445009330,507990021,638119352,
    583659535,797628118,726387553,890018660,
    835552979,1015980042,944750013,1276238704,
    1221641927,1167319070,1095957929,1595256236,
    1540665371,1452775106,1381403509,1780037320,
    1859660671,1671105958,1733955601,2031960084,
    2111593891,1889500026,1952343757,2552477408,
    2632100695,2443283854,2506133561,2334638140,
    2414271883,2191915858,2254759653,3190512472,
    3135915759,3081330742,3009969537,2905550212,
    2850959411,2762807018,2691435357,3560074640,
    3505614887,3719321342,3648080713,3342211916,
    3287746299,3467911202,3396681109,4063920168,
    4143685023,4223187782,4286162673,3779000052,
    3858754371,3904687514,3967668269,881225847,
    809987520,1023691545,969234094,662832811,
    591600412,771767749,717299826,311336399,
    374308984,453813921,533576470,25881363,
    88864420,134795389,214552010,2023205639,
    2086057648,1897238633,1976864222,1804852699,
    1867694188,1645340341,1724971778,1587496639,
    1516133128,1461550545,1406951526,1302016099,
    1230646740,1142491917,1087903418,2896545431,
    2825181984,2770861561,2716262478,3215044683,
    3143675388,3055782693,3001194130,2326604591,
    2389456536,2200899649,2280525302,2578013683,
    2640855108,2418763421,2498394922,3769900519,
    3832873040,3912640137,3992402750,4088425275,
    4151408268,4197601365,4277358050,3334271071,
    3263032808,3476998961,3422541446,3585640067,
    3514407732,3694837229,3640369242,1762451694,
    1842216281,1619975040,1682949687,2047383090,
    2127137669,1938468188,2001449195,1325665622,
    1271206113,1183200824,1111960463,1543535498,
    1489069629,1434599652,1363369299,622672798,
    568075817,748617968,677256519,907627842,
    853037301,1067152940,995781531,51762726,
    131386257,177728840,240578815,269590778,
    349224269,429104020,491947555,4046411278,
    4126034873,4172115296,4234965207,3794477266,
    3874110821,3953728444,4016571915,3609705398,
    3555108353,3735388376,3664026991,3290680682,
    3236090077,3449943556,3378572211,3174993278,
    3120533705,3032266256,2961025959,2923101090,
    2868635157,2813903052,2742672763,2604032198,
    2683796849,2461293480,2524268063,2284983834,
    2364738477,2175806836,2238787779,1569362073,
    1498123566,1409854455,1355396672,1317987909,
    1246755826,1192025387,1137557660,2072149281,
    2135122070,1912620623,1992383480,1753615357,
    1816598090,1627664531,1707420964,295390185,
    358241886,404320391,483945776,43990325,
    106832002,186451547,266083308,932423249,
    861060070,1041341759,986742920,613929101,
    542559546,756411363,701822548,3316196985,
    3244833742,3425377559,3370778784,3601682597,
    3530312978,3744426955,3689838204,3819031489,
    3881883254,3928223919,4007849240,4037393693,
    4100235434,4180117107,4259748804,2310601993,
    2373574846,2151335527,2231098320,2596047829,
    2659030626,2470359227,2550115596,2947551409,
    2876312838,2788305887,2733848168,3165939309,
    3094707162,3040238851,2985771188,
};

#include <errno.h>
#include <string.h>

#define READ_SIZE       2048
#define ERR_STRING_LEN  512

#define CRC32_INIT  4294967295

/*
 * num = lash.CRC32.string2num(str)
 *
 *  calculates the CRC32 hash of str
 *  and returns it as an integer 
 */
int CRC32Int(lua_State *L) {
    size_t length;
    unsigned char *data = (unsigned char *)luaL_checklstring(L, 1, &length);
    int i;
    unsigned int crc = CRC32_INIT;

    for (i = 0; i < length; i++) {
	crc = (crc << 8) ^ CRC32[(crc >> 24) ^ data[i]];
    }

    lua_pushinteger(L, crc);
    return 1;
}

/*
 * hexstr = lash.CRC32.string2hex(str)
 *
 *  calculates the CRC32 hash of str
 *  and returns it as a lowecase hex string
 */
int CRC32String(lua_State *L) {
    size_t length;
    unsigned char *data = (unsigned char *)luaL_checklstring(L, 1, &length);
    int i;
    unsigned int crc = CRC32_INIT;
    char hashstring[9];

    for (i = 0; i < length; i++) {
	crc = (crc << 8) ^ CRC32[(crc >> 24) ^ data[i]];
    }

    sprintf(hashstring, "%08x", crc);

    lua_pushstring(L, hashstring);
    return 1;
}

/*
 * num,err = lash.CRC32.file2num(filename)
 *
 *  calculates the CRC32 hash of the contents of filename
 *  and returns it as an integer
 *  on error return nil,errorstr
 */
int CRC32FileInt(lua_State *L) {
    unsigned int crc = CRC32_INIT;
    int i;
    const char *filename = luaL_checkstring(L, 1);

    FILE *inFile = fopen(filename, "rb");
    int bytes;

    unsigned char data[READ_SIZE];

    if (inFile == NULL) {
        char err[ERR_STRING_LEN];
        strerror_r(errno, err, ERR_STRING_LEN);

        lua_pushnil(L);
        lua_pushfstring(L, "Cannot open file '%s' for input: %s", filename, err);
        return 2;
    }

    while ((bytes = fread (data, 1, READ_SIZE, inFile)) != 0) {
	for (i = 0; i < bytes; i++) {
	    crc = (crc << 8) ^ CRC32[(crc >> 24) ^ data[i]];
	}
    }

    fclose(inFile);

    lua_pushinteger(L, crc);
    return 1;
}

/*
 * hexstr,err = lash.CRC32.file2num(filename)
 *
 *  calculates the CRC32 hash of the contents of filename
 *  and returns it as a lowercase hexstring
 *  on error return nil,errorstr
 */
int CRC32FileString(lua_State *L) {
    unsigned int crc = CRC32_INIT;
    int i;
    const char *filename = luaL_checkstring(L, 1);
    char hashstring[9];

    FILE *inFile = fopen(filename, "rb");
    int bytes;

    unsigned char data[READ_SIZE];

    if (inFile == NULL) {
        char err[ERR_STRING_LEN];
        strerror_r(errno, err, ERR_STRING_LEN);

        lua_pushnil(L);
        lua_pushfstring(L, "Cannot open file '%s' for input: %s", filename, err);
        return 2;
    }

    while ((bytes = fread (data, 1, READ_SIZE, inFile)) != 0) {
	for (i = 0; i < bytes; i++) {
	    crc = (crc << 8) ^ CRC32[(crc >> 24) ^ data[i]];
	}
    }

    fclose(inFile);

    sprintf(hashstring, "%08x", crc);

    lua_pushstring(L, hashstring);
    return 1;
}
