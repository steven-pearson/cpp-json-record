// LCOV_EXCL_START
#include <gtest/gtest.h>
// LCOV_EXCL_STOP
#include "../src/include/Variant.h"

TEST(VariantTestSuite, boolTests)
{
  taskly::Variant subject;

  EXPECT_EQ(subject.type(), taskly::VARTYPE::vtEmpty) << "Initial variant type incorrect";

  subject = true;
  
  EXPECT_EQ(subject.type(), taskly::VARTYPE::vtBool) << "Type not set to bool";
  EXPECT_EQ(subject, true) << "The add is not correct using double inputs";
}

TEST(VariantTestSuite, doubleTests)
{
  taskly::Variant subject;

  subject = 1.34;
  
  EXPECT_EQ(subject.type(), taskly::VARTYPE::vtDouble) << "Type not set to double";
  EXPECT_EQ(subject, 1.34) << "The add is not correct using double inputs";
  EXPECT_NE(subject, 1.4) << "The add is not correct using double inputs";
}

TEST(VariantTestSuite, InitialiseTest)
{
  taskly::Variant subject;
  const taskly::Variant empty;

  EXPECT_EQ(subject.type(), taskly::VARTYPE::vtEmpty) << "Initial variant type incorrect";
  EXPECT_EQ(empty.type(), taskly::VARTYPE::vtEmpty) << "Empty variant type incorrect";

  EXPECT_EQ(subject, empty) << "Empty variant compare failed";

  subject = true;
  
  EXPECT_NE(subject.type(), taskly::VARTYPE::vtEmpty) << "Type has not been set";
  EXPECT_EQ(subject, true) << "The add is not correct using double inputs";

  subject.clear();
  EXPECT_EQ(subject.type(), taskly::VARTYPE::vtEmpty) << "Reset variant type incorrect";
  EXPECT_NE(subject, true) << "Should no longer match value";
  EXPECT_EQ(subject, empty) << "Reset subject not equaling empty variant";

}
