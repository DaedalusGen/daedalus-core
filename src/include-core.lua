project "Daedalus-Core"
	language "C++"
	location "build/daedalus-core"

	files {
		"daedalus-core/core.cpp",
		"daedalus-core/**/*.cpp",
		"include/daedalus/core/core.hpp",
		"include/daedalus/core/**/*.hpp"
	}

	includedirs { "include/" }

	filter { "action:gmake" }
        buildoptions { "-Wall", "-Werror", "-Wpedantic" }

	filter { "platforms:run" }
		kind "SharedLib"

	filter { "platforms:static-build" }
		kind "StaticLib"

	filter { "platforms:dynamic-build" }
		kind "SharedLib"

	filter { "configurations:debug" }
	    defines { "DEBUG" }
