//
//  Copyright (C) 2014-2018 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                Florian Hahn
//                <https://github.com/casm-lang/casmc>
//
//  This file is part of casmc.
//
//  casmc is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  casmc is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with casmc. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _CASMC_VERSION_H_
#define _CASMC_VERSION_H_

/**
   @brief    TODO

   TODO

   variables are automatically set by CMake
*/

namespace casmc
{
    constexpr const char* REVTAG = "@GIT_REVTAG@";
    constexpr const char* COMMIT = "@GIT_COMMIT@";
    constexpr const char* BRANCH = "@GIT_BRANCH@";
    constexpr const char* LICENSE = "@GIT_LICENSE@";
    constexpr const char* NOTICE = "@GIT_NOTICE@";
}

#endif  // _CASMC_VERSION_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
