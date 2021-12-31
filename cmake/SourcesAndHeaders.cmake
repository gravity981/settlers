set(sources
		src/WorldGenerator.cpp
		src/Tile.cpp
		src/Corner.cpp
		src/Edge.cpp
)

set(exe_sources
		src/main.cpp
		${sources}
)

set(headers
		include/settlers/WorldGenerator.h
		include/settlers/Tile.h
		include/settlers/Corner.h
		include/settlers/Edge.h
)

set(test_sources
		src/WorldGenerator_Tests.cpp
		src/Corner_Tests.cpp
)
