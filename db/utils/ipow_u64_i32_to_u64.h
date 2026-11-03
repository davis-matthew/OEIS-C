/* ipow_u64_i32_to_u64: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_U64_I32_TO_U64_H
#define OEISC_UTILS_IPOW_U64_I32_TO_U64_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static unsigned long long ipow_u64_i32_to_u64(unsigned long long base, int exp) {
    unsigned long long result = 1;
    for (int i = 0; i < exp; ++i) result *= base;
    return result;
}

#endif
