set(sources
		src/tmp.cpp
		src/FieldGenerator.cpp
)

set(exe_sources
		src/main.cpp
		${sources}
)

set(headers
		include/settlers/tmp.hpp
		include/settlers/FieldGenerator.h
)

set(test_sources
		src/tmp_Tests.cpp
		src/FieldGenerator_Tests.cpp
)
