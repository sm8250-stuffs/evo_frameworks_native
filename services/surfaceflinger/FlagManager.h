/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cstdint>
#include <mutex>
#include <optional>
#include <string>

namespace android {
// Manages flags for SurfaceFlinger, including default values, system properties, and Mendel
// experiment configuration values. Can be called from any thread.
class FlagManager {
private:
    // Effectively making the constructor private, while allowing std::make_unique to work
    struct ConstructorTag {};

public:
    static const FlagManager& getInstance();
    static FlagManager& getMutableInstance();

    FlagManager(ConstructorTag);
    virtual ~FlagManager();

    void markBootCompleted();
    void dump(std::string& result) const;

    bool test_flag() const;
    bool use_adpf_cpu_hint() const;
    bool use_skia_tracing() const;

protected:
    // overridden for unit tests
    virtual std::optional<bool> getBoolProperty(const char*) const;
    virtual bool getServerConfigurableFlag(const char*) const;

private:
    friend class TestableFlagManager;

    FlagManager(const FlagManager&) = delete;

    std::atomic_bool mBootCompleted;

    static std::unique_ptr<FlagManager> mInstance;
    static std::once_flag mOnce;
};
} // namespace android
