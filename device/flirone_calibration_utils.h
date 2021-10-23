//
// Copyright 2020 ICSL. All Rights Reserved.
//

#ifndef DEVICE_FLIRONE_CALIBRATION_UTILS_H_
#define DEVICE_FLIRONE_CALIBRATION_UTILS_H_

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>

#include <glog/logging.h>

namespace device {
namespace flir {
class FlirOneCalibProfile {
  // https://rdrr.io/cran/Thermimage/man/raw2temp.html
  // https://exiftool.org/forum/index.php/topic,4898.60.html
  // Default value used from FLIR #00, serial T07K8R0010C
 public:
  double R1 = 18509.6;
  double R2 = 0.0125;
  double B = 1435;
  double F = 1;
  double O = -2163;
  double ReflectedApparentTemp = 22;
  double Emissivity = 0.9;
};

class FlirOneCalibUtil {
  // https://rdrr.io/cran/Thermimage/man/raw2temp.html
  // https://exiftool.org/forum/index.php/topic,4898.60.html
  // RAW_refl=R1/(R2*(e^(B/T_refl)-F))-O
  // RAW_obj=(S-(1-Emissivity)*RAW_refl)/Emissivity
  // T_obj= B / ln(R1/(R2*(RAW_obj+O))+F)
 public:
  static double ConvertRawToDoubleK(
      const FlirOneCalibProfile& profile, int raw);
  static int16_t ConvertRawToInt16K(
      const FlirOneCalibProfile& profile,int raw);
  static double ConvertRawToDoubleC(
      const FlirOneCalibProfile& profile,int raw);
  static int16_t ConvertRawToInt16C(
      const FlirOneCalibProfile& profile,int raw);
};
}  // namespace flir
}  // namespace device
#endif  // DEVICE_FLIRONE_CALIBRATION_UTILS_H_