#pragma once
/*
 * This file is part of Lydia.
 *
 * Lydia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Lydia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Lydia.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <cstdint>
#include <memory>
#include <set>
#include <vector>

namespace whitemech {
namespace lydia {

class Basic;
class Symbol;
class LDLfFormula;

typedef uint64_t hash_t;
typedef std::vector<std::shared_ptr<Basic>> vec_basic;
typedef std::set<std::shared_ptr<LDLfFormula>> set_boolean;
typedef std::vector<std::shared_ptr<LDLfFormula>> vec_boolean;
} // namespace lydia
}; // namespace whitemech
