/* bit_length_u32: the number of bits of n (0 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BIT_LENGTH_U32_H
#define OEISC_UTILS_BIT_LENGTH_U32_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int bit_length_u32(unsigned x) { int b = 0; while (x) { b++; x >>= 1; } return b; }

#endif
