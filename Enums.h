#pragma once
#include <cstdint>

enum class Permission : uint8_t
{
	liberalEntry,
	protectedEntry,
	restrictedEntry
};


union SourcePair
{
	uint8_t V4SP[6];
	uint8_t V6SP[18];

	uint8_t IPv4[4];
	uint8_t IPv6[16];
};