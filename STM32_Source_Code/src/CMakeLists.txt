add_subdirectory(platform)
add_subdirectory(controllers)

target_sources(${APP} PRIVATE
                startup.cpp
                main.cpp
                )