/******************************************************************************
 *                       ____    _    _____                                   *
 *                      / ___|  / \  |  ___|    C++                           *
 *                     | |     / _ \ | |_       Actor                         *
 *                     | |___ / ___ \|  _|      Framework                     *
 *                      \____/_/   \_|_|                                      *
 *                                                                            *
 * Copyright (C) 2011 - 2016                                                  *
 * Dominik Charousset <dominik.charousset (at) haw-hamburg.de>                *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License or  *
 * (at your option) under the terms and conditions of the Boost Software      *
 * License 1.0. See accompanying files LICENSE and LICENSE_ALTERNATIVE.       *
 *                                                                            *
 * If you did not receive a copy of the license files, see                    *
 * http://opensource.org/licenses/BSD-3-Clause and                            *
 * http://www.boost.org/LICENSE_1_0.txt.                                      *
 ******************************************************************************/

#ifndef CAF_NONE_HPP
#define CAF_NONE_HPP

#include <string>

#include "caf/detail/comparable.hpp"

namespace caf {

/// Represents "nothing", e.g., for clearing an `optional` by assigning `none`.
struct none_t : detail::comparable<none_t> {
  constexpr none_t() {
    // nop
  }
  constexpr explicit operator bool() const {
    return false;
  }

  static constexpr int compare(none_t) {
    return 0;
  }
};

static constexpr none_t none = none_t{};

/// @relates none_t
inline std::string to_string(const none_t&) {
  return "none";
}

} // namespace caf

#endif // CAF_NONE_HPP
