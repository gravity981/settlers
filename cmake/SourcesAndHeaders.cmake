set(sources
		src/WorldGenerator.cpp
		src/Tile.cpp
		src/Corner.cpp
		src/Edge.cpp
		src/Road.cpp
		src/Settlement.cpp
		src/Territory.cpp
		src/DiceRoller.cpp
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
		include/settlers/Road.h
		include/settlers/Settlement.h
		include/settlers/Territory.h
		include/settlers/DiceRoller.h
		include/settlers/ITileObject.h
		include/settlers/ICornerObject.h
)

set(test_sources
		src/WorldGenerator_Tests.cpp
		src/Corner_Tests.cpp
		src/DiceRoller_Tests.cpp
)
