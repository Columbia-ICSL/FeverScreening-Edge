//
// Copyright 2020 ICSL. All Rights Reserved.
//

#include <cmath>

#include "device/flirone_calibration_utils.h"

#define K_TO_C(x) (x - 273.15)

namespace device {
namespace flir {

FlirOneCalibUtil calib_util;

double FlirOneCalibUtil::ConvertRawToDoubleK(
    const FlirOneCalibProfile& profile, int raw) {
  // RAW_refl=R1/(R2*(e^(B/T_refl)-F))-O
  // RAW_obj=(S-(1-Emissivity)*RAW_refl)/Emissivity
  // T_obj= B / ln(R1/(R2*(RAW_obj+O))+F)
  raw = raw << 2;
  double raw_refl = 1.0 * profile.R1 
      / (profile.R2 * (exp(profile.B / profile.ReflectedApparentTemp)
      - profile.F)) - profile.O;
  double raw_obj = (1.0 * raw - (1 - profile.Emissivity)
      * raw_refl) / profile.Emissivity;
  return profile.B / log(profile.R1 
      / (profile.R2 * (raw_obj + profile.O)) + profile.F);
}

int16_t FlirOneCalibUtil::ConvertRawToInt16K(
    const FlirOneCalibProfile& profile, int raw) {
  double temp = FlirOneCalibUtil::ConvertRawToDoubleK(profile, raw);
  return static_cast<int16_t>(round(temp * 100));
}

double FlirOneCalibUtil::ConvertRawToDoubleC(
    const FlirOneCalibProfile& profile, int raw) {
  return K_TO_C(FlirOneCalibUtil::ConvertRawToDoubleK(profile, raw));
}

int16_t FlirOneCalibUtil::ConvertRawToInt16C(
    const FlirOneCalibProfile& profile, int raw) {
  double temp = FlirOneCalibUtil::ConvertRawToDoubleK(profile, raw);
  return static_cast<int16_t>(round(K_TO_C(temp) * 100));
}
}  // namespace flir
}  // namespace device
