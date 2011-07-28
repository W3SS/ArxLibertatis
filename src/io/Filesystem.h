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

#ifndef ARX_IO_FILESYSTEM_H
#define ARX_IO_FILESYSTEM_H

#include <cstddef>
#include <ctime>

#include "platform/Platform.h"

namespace fs {

class path;

/**
 * Check if a file (directory or regular file) exists.
 * @return true if the file exists, false if it doesn't exist or there was an error
 */
bool exists(const path & p);

/**
 * Check if a path points to a directory.
 * @return true if the p exists and is a directory, false otherwise
 */
bool is_directory(const path & p);

/**
 * Check if a path points to a regular file.
 * @return true if the p exists and is a regular file, false otherwise.
 */
bool is_regular_file(const path & p);

/**
 * Get the last write time of a file.
 * @return the last write time or 0 if there was an error (file doesn't exist, ...).
 */
std::time_t last_write_time(const path & p);

/**
 * Get the size of a file.
 * @return the filesize or (u64)-1 if there was an error (file doesn't exist, ...).
 */
u64 file_size(const path & p);

/**
 * Remove a file or empty directory.
 * @return true if the file was removed or didn't exist.
 */
bool remove(const path & p);

/**
 * Recursively remove a file or directory.
 * @return true if the file was removed or didn't exist.
 */
bool remove_all(const path & p);

/**
 * Create a directory.
 * p.parent() must exist and be a directory.
 * @return true if the directory was created or false if there was an error.
 */
bool create_directory(const path & p);

/**
 * Create a directory.
 * All ancestors of p must either be a directory or not exist.
 * @return true if the directory was created or false if there was an error.
 */
bool create_directories(const path & p);

/**
 * Copy a regular file.
 * from_p must exist and be a regular file.
 * to_p.parent() must exist and be a directory.
 * TODO does this overwrite to_p
 * @return true if the file was copied or false if there was an error.
 */
bool copy_file(const path & from_p, const path & to_p, bool overwrite = false);

/**
 * Move a regular file or directory.
 * old_p must exist.
 * new_p.parent() must exist and be a directory.
 * TODO does this overwrite to_p
 * @return true if the file was copied or false if there was an error.
 */
bool rename(const path & old_p, const path & new_p);

/**
 * Read a file into memory.
 * @param p The file to load.
 * @param size Will receive the size of the loaded file.
 * @return a new[]-allocated buffer containing the file data or NULL on error.
 */
char * read_file(const path & p, size_t & size);

class directory_iterator {
	
	directory_iterator operator++(int); //!< prevent postfix ++
	
	//! prevent assignment
	directory_iterator & operator=(const directory_iterator &);
	directory_iterator(const directory_iterator &);
	
	void * handle;
#if !defined(BOOST_FILESYSTEM_VERSION) || BOOST_FILESYSTEM_VERSION < 3
	void * buf;
#endif
	
public:
	
	directory_iterator(const fs::path & p);
	
	~directory_iterator();
	
	directory_iterator & operator++();
	
	bool end();
	
	std::string name();
	
	bool is_directory();
	
	bool is_regular_file();
	
};

}

#endif // ARX_IO_FILESYSTEM_H
