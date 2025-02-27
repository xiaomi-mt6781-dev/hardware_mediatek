/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/vibrator/BnVibrator.h>
#include <map>

namespace aidl {
namespace android {
namespace hardware {
namespace vibrator {

const std::string kVibratorPropPrefix = "ro.vendor.vibrator.hal.";
const std::string kVibratorPropDuration = ".duration";

const std::string kVibratorState = "/sys/class/leds/vibrator/state";
const std::string kVibratorDuration = "/sys/class/leds/vibrator/duration";
const std::string kVibratorActivate = "/sys/class/leds/vibrator/activate";

#ifdef VIBRATOR_SUPPORTS_EFFECTS
const std::string kVibratorStrength = "/sys/class/leds/vibrator/vmax";
const std::string kVibratorStrengthMax = "/sys/class/leds/vibrator/vmax_max";
#endif

static std::map<EffectStrength, float> vibStrengths = {
        {EffectStrength::LIGHT, 0.25}, {EffectStrength::MEDIUM, 0.5}, {EffectStrength::STRONG, 1}};

class Vibrator : public BnVibrator {
  public:
#ifdef VIBRATOR_SUPPORTS_EFFECTS
    Vibrator();
#endif
    ndk::ScopedAStatus getCapabilities(int32_t* _aidl_return) override;
    ndk::ScopedAStatus off() override;
    ndk::ScopedAStatus on(int32_t timeoutMs,
                          const std::shared_ptr<IVibratorCallback>& callback) override;
    ndk::ScopedAStatus perform(Effect effect, EffectStrength strength,
                               const std::shared_ptr<IVibratorCallback>& callback,
                               int32_t* _aidl_return) override;
    ndk::ScopedAStatus getSupportedEffects(std::vector<Effect>* _aidl_return) override;
    ndk::ScopedAStatus setAmplitude(float amplitude) override;
    ndk::ScopedAStatus setExternalControl(bool enabled) override;
    ndk::ScopedAStatus getCompositionDelayMax(int32_t* maxDelayMs);
    ndk::ScopedAStatus getCompositionSizeMax(int32_t* maxSize);
    ndk::ScopedAStatus getSupportedPrimitives(std::vector<CompositePrimitive>* supported) override;
    ndk::ScopedAStatus getPrimitiveDuration(CompositePrimitive primitive,
                                            int32_t* durationMs) override;
    ndk::ScopedAStatus compose(const std::vector<CompositeEffect>& composite,
                               const std::shared_ptr<IVibratorCallback>& callback) override;
    ndk::ScopedAStatus getSupportedAlwaysOnEffects(std::vector<Effect>* _aidl_return) override;
    ndk::ScopedAStatus alwaysOnEnable(int32_t id, Effect effect, EffectStrength strength) override;
    ndk::ScopedAStatus alwaysOnDisable(int32_t id) override;
    ndk::ScopedAStatus getResonantFrequency(float* resonantFreqHz) override;
    ndk::ScopedAStatus getQFactor(float* qFactor) override;
    ndk::ScopedAStatus getFrequencyResolution(float* freqResolutionHz) override;
    ndk::ScopedAStatus getFrequencyMinimum(float* freqMinimumHz) override;
    ndk::ScopedAStatus getBandwidthAmplitudeMap(std::vector<float>* _aidl_return) override;
    ndk::ScopedAStatus getPwlePrimitiveDurationMax(int32_t* durationMs) override;
    ndk::ScopedAStatus getPwleCompositionSizeMax(int32_t* maxSize) override;
    ndk::ScopedAStatus getSupportedBraking(std::vector<Braking>* supported) override;
    ndk::ScopedAStatus composePwle(const std::vector<PrimitivePwle>& composite,
                                   const std::shared_ptr<IVibratorCallback>& callback) override;

  private:
    static ndk::ScopedAStatus setNode(const std::string path, const int32_t value);
    static int getIntProperty(const std::string& key, const int fallback);
    std::map<Effect, int32_t> vibEffects = {
            {Effect::CLICK, getIntProperty("click" + kVibratorPropDuration, 50)},
            {Effect::TICK, getIntProperty("tick" + kVibratorPropDuration, 32)},
            {Effect::TEXTURE_TICK, getIntProperty("texture_tick" + kVibratorPropDuration, 25)},
    };
#ifdef VIBRATOR_SUPPORTS_EFFECTS
    static bool exists(const std::string path);
    static int getNode(const std::string path, const int fallback);
    bool mVibratorStrengthSupported;
    int mVibratorStrengthMax;
#endif
    ndk::ScopedAStatus activate(const int32_t timeoutMs);
};

}  // namespace vibrator
}  // namespace hardware
}  // namespace android
}  // namespace aidl
