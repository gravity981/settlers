set(sources
		src/tmp.cpp
		src/FieldGenerator.cpp
		src/Tile.cpp
)

set(exe_sources
		src/main.cpp
		${sources}
)

set(headers
		include/settlers/tmp.hpp
		include/settlers/FieldGenerator.h
		include/settlers/Tile.h
)

set(test_sources
		src/tmp_Tests.cpp
		src/FieldGenerator_Tests.cpp
)
