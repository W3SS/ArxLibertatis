/*
===========================================================================
ARX FATALIS GPL Source Code
Copyright (C) 1999-2010 Arkane Studios SA, a ZeniMax Media company.

This file is part of the Arx Fatalis GPL Source Code ('Arx Fatalis Source Code'). 

Arx Fatalis Source Code is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Arx Fatalis Source Code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Arx Fatalis Source Code.  If not, see 
<http://www.gnu.org/licenses/>.

In addition, the Arx Fatalis Source Code is also subject to certain additional terms. You should have received a copy of these 
additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Arx 
Fatalis Source Code. If not, please request a copy in writing from Arkane Studios at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing Arkane Studios, c/o 
ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/

#include "io/Filesystem.h"

#include "Configure.h"

#ifdef HAVE_BOOST_FILESYSTEM_V3

#define try
#define catch(a) if(false)
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#undef try
#undef catch

#include "io/FilePath.h"

using std::string;

namespace fs {

namespace fs_boost = boost::filesystem;

using boost::system::error_code;

bool exists(const path & p) {
	error_code ec;
	return fs_boost::exists(p.string(), ec) && !ec;
}

bool is_directory(const path & p) {
	error_code ec;
	return fs_boost::is_directory(p.string(), ec) && !ec;
}

bool is_regular_file(const path & p) {
	error_code ec;
	return fs_boost::is_regular_file(p.string(), ec) && !ec;
}

std::time_t last_write_time(const path & p) {
	error_code ec;
	std::time_t time = fs_boost::last_write_time(p.string(), ec);
	return ec ? 0 : time;
}

u64 file_size(const path & p) {
	error_code ec;
	uintmax_t size = fs_boost::file_size(p.string(), ec);
	return ec ? (u64)-1 : (u64)size;
}

bool remove(const path & p) {
	error_code ec;
	fs_boost::remove(p.string(), ec);
	return !ec;
}

bool remove_all(const path & p) {
	error_code ec;
	fs_boost::remove_all(p.string(), ec);
	return !ec;
}

bool create_directory(const path & p) {
	error_code ec;
	fs_boost::create_directory(p.string(), ec);
	return !ec;
}

bool create_directories(const path & p) {
	error_code ec;
	fs_boost::create_directories(p.string(), ec);
	return !ec;
}

bool copy_file(const path & from_p, const path & to_p, bool overwrite) {
	error_code ec;
	fs_boost::copy_option::enum_type o;
	if(overwrite) {
		o = fs_boost::copy_option::overwrite_if_exists;
	} else {
		o = fs_boost::copy_option::fail_if_exists;
	}
	fs_boost::copy_file(from_p.string(), to_p.string(), o, ec);
	return !ec;
}

bool rename(const path & old_p, const path & new_p) {
	error_code ec;
	fs_boost::rename(old_p.string(), new_p.string(), ec);
	return !ec;
}

directory_iterator::directory_iterator(const fs::path & p) {
	error_code ec;
	handle = new fs_boost::directory_iterator(p.empty() ? "./" : p.string(), ec);
	if(ec) {
		delete reinterpret_cast<fs_boost::directory_iterator *>(handle);
		handle = new fs_boost::directory_iterator();
	}
};

directory_iterator::~directory_iterator() {
	delete reinterpret_cast<fs_boost::directory_iterator *>(handle);
}

directory_iterator & directory_iterator::operator++() {
	error_code ec;
	(*reinterpret_cast<fs_boost::directory_iterator *>(handle)).increment(ec);
	if(ec) {
		delete reinterpret_cast<fs_boost::directory_iterator *>(handle);
		handle = new fs_boost::directory_iterator();
	}
	return *this;
}

bool directory_iterator::end() {
	return (*reinterpret_cast<fs_boost::directory_iterator *>(handle) == fs_boost::directory_iterator());
}

string directory_iterator::name() {
	return (*reinterpret_cast<fs_boost::directory_iterator *>(handle))->path().filename().string();
}

bool directory_iterator::is_directory() {
	error_code ec;
	return fs_boost::is_directory((*reinterpret_cast<fs_boost::directory_iterator *>(handle))->status(ec)) && !ec;
}

bool directory_iterator::is_regular_file() {
	error_code ec;
	return fs_boost::is_regular_file((*reinterpret_cast<fs_boost::directory_iterator *>(handle))->status(ec)) && !ec;
}

} // namespace fs

#endif // HAVE_BOOST_FILESYSTEM_V3
