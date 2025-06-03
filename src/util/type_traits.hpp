// SPDX-License-Identifier: BSD-3-Clause
#pragma once

#include <type_traits>

template<typename T>
struct false_type : std::false_type
{
};
