project "Daedalus-Core"
	language "C++"
	location "build/daedalus-core"

	files {
		"daedalus/core.cpp",
		"daedalus/**/*.cpp",
		"include/AquIce/daedalus/*.hpp"
	}

	includedirs { "include/" }

	filter { "configurations:run" }
		kind "SharedLib"

	filter { "configurations:static-build" }
		kind "StaticLib"

	filter { "configurations:dynamic-build" }
		kind "SharedLib"