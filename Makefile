#
#   Copyright (C) 2014-2024 CASM Organization <https://casm-lang.org>
#   All rights reserved.
#
#   Developed by: Philipp Paulweber et al.
#   <https://github.com/casm-lang/casmc/graphs/contributors>
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

FORMAT  = src
FORMAT += src/*
FORMAT += src/*/*
FORMAT += etc
FORMAT += etc/*
FORMAT += etc/*/*

UPDATE_ROOT = ../../lib/stdhl

include .cmake/config.mk

# ENV_FLAGS = CASM=$(OBJ)/$(TARGET) CASM_ARG_PRE=--ast-exec-num
