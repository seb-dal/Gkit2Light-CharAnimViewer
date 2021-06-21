if _PREMAKE_VERSION >="5.0" then
	location ("build/")
end


no_project = true
dofile "./premake4.lua"

-- ShallowWater
gfx_masterCharAnim_dir = path.getabsolute(".")

master_CharAnim_files = {	gfx_masterCharAnim_dir .. "/src/ShallowWater/*.cpp", 
							gfx_masterCharAnim_dir .. "/src/ShallowWater/*.h"
	}
	
project("ShallowWater")
    language "C++"
    kind "ConsoleApp"
    targetdir ( gfx_masterCharAnim_dir .. "/bin" )
	includedirs { gfx_masterCharAnim_dir .. "/src/ShallowWater/" }
    files ( gkit_files )
    files ( master_CharAnim_files )
	location ("build/")
