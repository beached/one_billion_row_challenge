// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/one_billion_row_challenge
//

#include <daw/daw_memory_mapped_file.h>
#include <daw/daw_string_view.h>

#include <boost/container/flat_map.hpp>
#include <cstddef>
#include <cstdlib>
#include <fast_float/fast_float.h>
#include <fmt/format.h>
#include <iterator>
#include <limits>

using sv_t = daw::basic_string_view<char, daw::string_view_bounds_type::size>;

int main( int argc, char **argv ) {
	if( argc < 2 ) {
		fmt::println( stderr, "Expected filename on commandline" );
		exit( EXIT_FAILURE );
	}
	auto const fdata_map = daw::filesystem::memory_mapped_file_t( argv[1] );
	if( not fdata_map ) {
		fmt::println( stderr, "Invalid input file {}", argv[1] );
		exit( EXIT_FAILURE );
	}
	struct aggregates_t {
		float min = std::numeric_limits<float>::max( );
		float max = std::numeric_limits<float>::lowest( );
		float mean = 0;
		float count = 0;

		aggregates_t( ) = default;

		constexpr void update( float value ) {
			if( value < min ) {
				min = value;
			}
			if( value > max ) {
				max = value;
			}
			count = count + 1;
			mean = mean + ( value - mean ) / count;
			// mean = mean * ( ( count - 1 ) / count ) + value / count;
		}
	};
	auto aggregates = boost::container::flat_map<sv_t, aggregates_t>{ };
	auto fdata = sv_t( fdata_map );
	aggregates.reserve( 1024 );
	while( not fdata.empty( ) ) {
		auto name = fdata.pop_front_until( ';' );
		auto value = fdata.pop_front_until( '\n' );
		float temp;
		(void)fast_float::from_chars( value.data( ), value.data_end( ), temp );
		aggregates[name].update( temp );
	}
	fmt::println( "Found {} cities", aggregates.size( ) );
	for( auto const &kv : aggregates ) {
		fmt::println( "{}: min = {}, max = {}, median = {}, count = {}",
		              static_cast<std::string>( kv.first ), kv.second.min,
		              kv.second.max, kv.second.mean, kv.second.count );
	}
}
