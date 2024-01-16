zephyr_library()

zephyr_library_sources_ifdef(CONFIG_PMW3320 src/pmw3320.c)
zephyr_include_directories(../app/include)
