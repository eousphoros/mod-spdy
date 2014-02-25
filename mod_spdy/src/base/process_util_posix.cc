// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file is a minimal stub version of
// src/third_party/chromium/src/base/process_util_posix.cc.  We use this
// instead of the original file (which contains many more functions than the
// one below, none of which we need) to avoid pulling in lots of other
// dependencies that we otherwise wouldn't need.

#include <unistd.h>

#include "base/process_util.h"

namespace base {

ProcessId GetCurrentProcId() {
  return getpid();
}

}  // namespace base
