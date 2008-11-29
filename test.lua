#!/usr/bin/lua

require('lash')

-- do hashes on either the 
-- first command line argument
-- or this file itself
local val = arg[1] or arg[0]

-- MD5 functions
print(lash.MD5.file2hex(val))
print(lash.MD5.string2hex(val))

-- CRC32 functions
print(lash.CRC32.string2hex(val))
print(lash.CRC32.string2num(val))

-- SHA1 functions
print(lash.SHA1.string2hex(val))
print(lash.SHA1.file2hex(val))

