#include "catch.hpp"

#include <array>
#include <oroch/varint.h>

using varint32 = oroch::varint_codec<uint32_t>;
using varint64 = oroch::varint_codec<uint64_t>;

TEST_CASE("varint codec for single value", "[varint]") {

	std::array<uint8_t, 10> bytes;

	auto it = bytes.begin();
	REQUIRE(varint32::encode(it, bytes.end(), 1));
	REQUIRE(it == bytes.begin() + 1);
	REQUIRE(bytes[0] == 1);

	it = bytes.begin();
	REQUIRE(varint32::encode(it, bytes.end(), 2));
	REQUIRE(it == bytes.begin() + 1);
	REQUIRE(bytes[0] == 2);

	it = bytes.begin();
	REQUIRE(varint32::encode(it, bytes.end(), 127));
	REQUIRE(it == bytes.begin() + 1);
	REQUIRE(bytes[0] == 127);

	it = bytes.begin();
	REQUIRE(varint32::encode(it, bytes.end(), 128));
	REQUIRE(it == bytes.begin() + 2);
	REQUIRE(bytes[0] == 128);
	REQUIRE(bytes[1] == 1);

	for (uint32_t i = 1; i != 0; i += i) {
		it = bytes.begin();
		REQUIRE(varint32::encode(it, bytes.end(), i));

		uint32_t j;
		auto it_end = it;
		it = bytes.begin();
		REQUIRE(varint32::decode(j, it, it_end));
		REQUIRE(i == j);
	}

	for (uint64_t i = 1; i != 0; i += i) {
		it = bytes.begin();
		REQUIRE(varint64::encode(it, bytes.end(), i));

		uint64_t j;
		auto it_end = it;
		it = bytes.begin();
		REQUIRE(varint64::decode(j, it, it_end));
		REQUIRE(i == j);
	}
}

TEST_CASE("varint codec for array", "[varint]") {
	std::array<uint8_t, 15> bytes;
	std::array<uint32_t, 5> integers {{
		0, 0x80, 0x4000, 0x200000, 0x1000000
	}};

	auto b_it = bytes.begin();
	auto i_it = integers.begin();
	REQUIRE(varint32::encode(b_it, bytes.end(), i_it, integers.end()));

	std::array<uint32_t, 5> integers2;
	b_it = bytes.begin();
	i_it = integers2.begin();
	REQUIRE(varint32::decode(i_it, integers2.end(), b_it, bytes.end()));

	for (size_t i = 0; i < integers.size(); i++) {
		REQUIRE(integers[i] == integers2[i]);
	}
}