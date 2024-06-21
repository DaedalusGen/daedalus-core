workspace "Daedalus"
	configurations { "run", "static-build", "dynamic-build" }
	startproject "Daedalus-Core"

	include "include-core.lua"

project "Daedalus-Core"
	filter { "configurations:run" }
		kind "ConsoleApp"