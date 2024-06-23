add_subdirectory(platform)

target_sources(${APP} PRIVATE
                startup.cpp
                main.cpp
                                )