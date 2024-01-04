cmake_minimum_required( VERSION 3.27 )
project( one_billion_row_challenge )


find_package(Boost REQUIRED container)
find_package(daw-header-libraries CONFIG REQUIRED)
find_package(FastFloat CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable( one_billion_row_challenge src/one_billion_row_challenge.cpp )
target_link_libraries( one_billion_row_challenge PRIVATE
                       Boost::boost
                       daw::daw-header-libraries
                       FastFloat::fast_float
                       fmt::fmt
)
set_property( TARGET one_billion_row_challenge PROPERTY CXX_STANDARD 23 )
set_property( TARGET one_billion_row_challenge PROPERTY CXX_STANDARD_REQUIRED ON )
