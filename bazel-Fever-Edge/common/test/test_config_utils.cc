//
// Copyright 2020 ICSL. All Rights Reserved.
//

#include <stdlib.h> 

#include "gtest/gtest.h"
#include "common/config_utils.h"

namespace common {
namespace config {

TEST(config_utils, constructor_1) {
  char env[] = "TEST_ENV";
  char set_env[] = "THIS IS TEST FOR ENV";
  char overwrite[] = "THIS IS TEST FOR OVERWRITE";
  char random_env[] = "adsfasdfkjadsfljasdkjlfjasdlf";
  setenv(env, set_env, 0);
  std::string get_env = GetEnvVariable(env, overwrite);
  EXPECT_EQ(get_env.compare(set_env), 0);
  get_env = GetEnvVariable(random_env, overwrite);
  EXPECT_EQ(get_env.compare(overwrite), 0);
}
}  // namespace config
}  // namespace common
