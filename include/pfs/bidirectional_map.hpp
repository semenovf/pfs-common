////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Vladislav Trifochkin
//
// This file is part of [pfs-common](https://github.com/semenovf/pfs-common) library.
//
// Changelog:
//      2021.02.13 Initial version
//
// TODO
//      1. Implement (see boost::bimap - https://www.boost.org/doc/libs/1_75_0/libs/bimap/doc/html/index.html)
////////////////////////////////////////////////////////////////////////////////
//#pragma once // in next implementations need to get rid of using "include" files of library Boost for 
#ifndef BOOST_BIMAP_BIMAP_HPP
#define BOOST_BIMAP_BIMAP_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/config.hpp>
#include <boost/bimap/detail/user_interface_config.hpp>
#include <boost/mpl/aux_/na.hpp>

#ifndef BOOST_BIMAP_DISABLE_SERIALIZATION
    #include <boost/serialization/nvp.hpp>
#endif // BOOST_BIMAP_DISABLE_SERIALIZATION

// Boost.Bimap
#include <boost/bimap/detail/bimap_core.hpp>
#include <boost/bimap/detail/map_view_base.hpp>
#include <boost/bimap/detail/modifier_adaptor.hpp>
#include <boost/bimap/relation/support/data_extractor.hpp>
#include <boost/bimap/relation/support/member_with_tag.hpp>

#include <boost/bimap/support/map_type_by.hpp>
#include <boost/bimap/support/map_by.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>

namespace pfs {

template< class KeyTypeA, class KeyTypeB>
class bidirectional_map
{

public:
	bidirectional_map& operator=(const bidirectional_map & bMap)
	{
		core = bMap.core;
		return *this;
	}

private:
	BOOST_DEDUCED_TYPENAME base_::core_type core;
};

} // namespace pfs
