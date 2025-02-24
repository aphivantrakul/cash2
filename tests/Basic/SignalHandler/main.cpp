// Copyright (c) 2018-2021 The Cash2 developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <Common/SignalHandler.h>
#include <gtest/gtest.h>
#include <iostream>

using namespace Tools;

// constructor
TEST(SignalHandler, 1)
{
  SignalHandler();
}

// install()
TEST(SignalHandler, 2)
{
  SignalHandler signalHandler;

  std::function<void(void)> myFunction = [&](){ std::cout << "Hello World" << std::endl; };
  ASSERT_TRUE(signalHandler.install(myFunction));
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}