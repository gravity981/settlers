set(sources
		src/tmp.cpp
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
		include/settlers/tmp.hpp
		include/settlers/WorldGenerator.h
		include/settlers/Tile.h
		include/settlers/Corner.h
		include/settlers/Edge.h
)

set(test_sources
		src/tmp_Tests.cpp
		src/WorldGenerator_Tests.cpp
)
