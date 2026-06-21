#define BOOST_TEST_MODULE SHA1Tests
#include <boost/test/included/unit_test.hpp>
#include "sha1.hpp"
#include <string>

BOOST_AUTO_TEST_CASE(testEmptyString) {
  petrenko::SHA1 sha;
  std::string hash = sha("");

  BOOST_CHECK_EQUAL(hash, "da39a3ee5e6b4b0d3255bfef95601890afd80709");
}

BOOST_AUTO_TEST_CASE(testHelloWorld) {
  petrenko::SHA1 sha;
  std::string hash = sha("Hello, World!");

  BOOST_CHECK_EQUAL(hash, "0a0a9f2a6772942557ab5355d76af442f8f65e01");
}

BOOST_AUTO_TEST_CASE(testTheQuickBrownFox) {
  petrenko::SHA1 sha;
  std::string hash = sha("The quick brown fox jumps over the lazy dog");

  BOOST_CHECK_EQUAL(hash, "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12");
}

BOOST_AUTO_TEST_CASE(testAbc) {
  petrenko::SHA1 sha;
  std::string hash = sha("abc");

  BOOST_CHECK_EQUAL(hash, "a9993e364706816aba3e25717850c26c9cd0d89d");
}

BOOST_AUTO_TEST_CASE(testOneMillionAs) {
  petrenko::SHA1 sha;
  std::string input(1000000, 'a');
  std::string hash = sha(input);

  BOOST_CHECK_EQUAL(hash, "34aa973cd4c4daa4f61eeb2bdbad27316534016f");
}

BOOST_AUTO_TEST_CASE(testHashBytes) {
  petrenko::SHA1 sha;
  const uint8_t bytes[] = {0x61, 0x62, 0x63};
  std::string hash = sha.hashBytes(bytes, 3);

  BOOST_CHECK_EQUAL(hash, "a9993e364706816aba3e25717850c26c9cd0d89d");
}

BOOST_AUTO_TEST_CASE(testByteHashFunction) {
  const uint8_t bytes[] = {0x61, 0x62, 0x63};
  size_t hash = petrenko::byteHash(bytes, 3);

  BOOST_CHECK_NE(hash, 0);
}
