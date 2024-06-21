project "Daedalus-Core"
	language "C++"
	location "build/daedalus-core"
	entrypoint "main"
	
	files {
		"main.cpp",
		"daedalus/**/*.cpp",
		"include/AquIce/daedalus/*.hpp"
	}

	includedirs { "include/" }

	filter { "configurations:run" }
		kind "ConsoleApp"

	filter { "configurations:static-build" }
		kind "StaticLib"
		removefiles { "main.cpp" }

	filter { "configurations:dynamic-build" }
		kind "SharedLib"
		removefiles { "main.cpp" }