// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/fml/thread_local.h"

namespace fml {

ThreadLocal::ThreadLocal() : ThreadLocal(nullptr) {}

ThreadLocal::ThreadLocal(ThreadLocalDestroyCallback destroy)
    : destroy_(destroy) {
  auto callback =
      reinterpret_cast<void (*)(void*)>(&ThreadLocal::ThreadLocalDestroy);
  FML_CHECK(pthread_key_create(&_key, callback) == 0);
}

ThreadLocal::~ThreadLocal() {
  // This will NOT call the destroy callbacks on thread local values still
  // active in other threads. Those must be cleared manually. The usage
  // of this class should be similar to the thread_local keyword but with
  // with a static storage specifier

  // Collect the container
  delete reinterpret_cast<Box*>(pthread_getspecific(_key));

  // Finally, collect the key
  FML_CHECK(pthread_key_delete(_key) == 0);
}

ThreadLocal::Box::Box(ThreadLocalDestroyCallback destroy, intptr_t value)
    : destroy_(destroy), value_(value) {}

ThreadLocal::Box::~Box() = default;

}  // namespace fml
