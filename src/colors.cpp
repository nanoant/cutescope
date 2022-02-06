//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#include "colors.h"

namespace colors {

const char* defChannel[] = { "#ff3333", "#00ccff", "silver" };
const int nbChannel = sizeof(defChannel) / sizeof(*defChannel);
const char** channel = defChannel;

}  // namespace colors
