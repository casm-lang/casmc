#
#   Copyright (c) 2014-2017 CASM Organization
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
#                 Emmanuel Pescosta
#                 Florian Hahn
#                 https://github.com/casm-lang/casmc
#
#   This file is part of casmc.
#
#   casmc is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   casmc is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with casmc. If not, see <http://www.gnu.org/licenses/>.
#

TARGET = casmc

include .config.mk

ENV_FLAGS = CASM=$(OBJ)/$(TARGET) CASM_ARG_PRE=--ast-exec-num


version: $(OBJ)/version.h
$(OBJ)/version.h: $(OBJ)
	@echo "const char VERSION[] =" > $@
	@echo "\""`git describe --always --tags --dirty`"\"" >> $@
	@echo ";" >> $@

license: $(OBJ)/license.h
$(OBJ)/license.h: $(OBJ) LICENSE.txt
	@echo "const char LICENSE[] =" > $@
	@head -n `grep -ne "------" LICENSE.txt | grep -Eo "[0-9]*"` LICENSE.txt | \
		sed "/-----/d" | \
		sed "/This file is part of/d" | \
		sed "s/^/    /" | \
		sed "s/^/\"/g" | \
		sed "s/$$/\\\n\"/g" >> $@
	@echo ";" >> $@
