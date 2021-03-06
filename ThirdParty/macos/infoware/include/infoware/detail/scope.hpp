// infoware - C++ System information Library
//
// Written in 2016-2020 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>


#pragma once


#include <infoware/linkage.hpp>

#include <functional>


namespace iware {
	namespace detail {
		struct quickscope_wrapper {
			std::function<void()> func;

			quickscope_wrapper(const quickscope_wrapper&) = delete;
			INFOWARE_API_LINKAGE_INTERNAL ~quickscope_wrapper();
		};
	}  // namespace detail
}  // namespace iware
