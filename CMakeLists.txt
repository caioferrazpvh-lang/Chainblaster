cmake_minimum_required(VERSION 3.10)
project(chain C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(ALLEGRO REQUIRED
    allegro-5
    allegro_ttf-5
    allegro_font-5
    allegro_primitives-5
    allegro_image-5
    allegro_main-5
    allegro_audio-5
    allegro_acodec-5
)

add_executable(chain
    chain.c
    heros.c
    enemies.c
    tiro.c
    mechanics.c
    game.c
    asteroid.c
)

target_include_directories(chain PRIVATE ${ALLEGRO_INCLUDE_DIRS})
target_link_directories(chain PRIVATE ${ALLEGRO_LIBRARY_DIRS})
target_link_libraries(chain PRIVATE ${ALLEGRO_LIBRARIES})
target_compile_options(chain PRIVATE ${ALLEGRO_CFLAGS_OTHER})
