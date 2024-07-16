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

	filter { "configurations:run" }
		kind "SharedLib"

	filter { "configurations:static-build" }
		kind "StaticLib"

	filter { "configurations:dynamic-build" }
		kind "SharedLib"