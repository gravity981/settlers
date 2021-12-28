if(${PROJECT_NAME}_ENABLE_CONAN)
  #
  # Setup Conan requires and options here:
  #

  set(${PROJECT_NAME}_CONAN_REQUIRES "")
  set(${PROJECT_NAME}_CONAN_OPTIONS "")

  #
  # If `conan.cmake` (from https://github.com/conan-io/cmake-conan) does not exist, download it.
  #
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(
      STATUS
      "Downloading conan.cmake from https://github.com/conan-io/cmake-conan..."
    )
    file(
      DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/release/0.17/conan.cmake"
      "${CMAKE_BINARY_DIR}/conan.cmake"
    )
    message(STATUS "Cmake-Conan downloaded succesfully.")
  endif()

  include(${CMAKE_BINARY_DIR}/conan.cmake)

  #conan_add_remote(
  #  NAME bincrafters
  #  URL
  #  https://api.bintray.com/conan/bincrafters/public-conan
  #)

  #conan_cmake_run(
  #  REQUIRES
  #  ${${PROJECT_NAME}_CONAN_REQUIRES}
  #  OPTIONS
  #  ${${PROJECT_NAME}_CONAN_OPTIONS}
  #  BASIC_SETUP
  #  CMAKE_TARGETS # Individual targets to link to
  #  BUILD
  #  missing
  #)
  conan_cmake_configure(
          REQUIRES gtest/1.10.0 nlohmann_json/3.10.4 spdlog/1.9.2
          GENERATORS cmake_paths cmake_find_package
          BUILD_REQUIRES cmake/3.15.7
          IMPORTS "bin, *.dll -> ./bin"
          IMPORTS "lib, *.dylib* -> ./bin"
          OPTIONS
            gtest:shared=True gtest:build_gmock=True gtest:hide_symbols=False
            spdlog:shared=True spdlog:header_only=True spdlog:no_exceptions=True
  )

  conan_cmake_autodetect(settings BUILD_TYPE Release)

  conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings})

  include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

  verbose_message("Conan is setup and all requires have been installed.")
endif()
