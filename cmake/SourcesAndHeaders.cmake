set(sources
		src/settlers/World.cpp
		src/settlers/Tile.cpp
		src/settlers/Corner.cpp
		src/settlers/Edge.cpp
		src/settlers/Road.cpp
		src/settlers/Settlement.cpp
		src/settlers/Territory.cpp
		src/settlers/DiceRoller.cpp
		src/settlers/Sector.cpp
		src/settlers/Harbour.cpp
		src/settlers/TriggerEffectCollection.cpp
		src/settlers/TradingCenter.cpp
		src/settlers/Resource.cpp
		src/settlers/JsonUtil.cpp
		src/settlers/JsonUtil.h
		src/settlers/Game.cpp
)

set(exe_sources
		src/consoleApp/main.cpp
)
#${sources}
set(headers
		include/settlers/World.h
		include/settlers/Tile.h
		include/settlers/Corner.h
		include/settlers/Edge.h
		include/settlers/Road.h
		include/settlers/Settlement.h
		include/settlers/Territory.h
		include/settlers/Sector.h
		include/settlers/Harbour.h
		include/settlers/DiceRoller.h
		include/settlers/TriggerEffectCollection.h
		include/settlers/ITileObject.h
		include/settlers/ICornerObject.h
		include/settlers/IEdgeObject.h
		include/settlers/ISectorObject.h
		include/settlers/TradingCenter.h
		include/settlers/Resource.h
		include/settlers/Game.h
		include/settlers/IWorld.h
		include/settlers/IGameObserver.h
)

set(test_sources
		src/mock/MockWorld.h
		src/mock/MockGameObserver.h
)

set(test_exe_sources
		src/World_Tests.cpp
		src/Corner_Tests.cpp
		src/DiceRoller_Tests.cpp
		src/Tile_Tests.cpp
		src/TradingCenter_Tests.cpp
		src/Resource_Tests.cpp
		src/Game_Tests.cpp
		)
