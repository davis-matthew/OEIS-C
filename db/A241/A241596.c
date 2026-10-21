/*
 * A241596  Partitions listed by alternately incrementing each part and
 *          appending a 1.
 * https://oeis.org/A241596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   a(n) is obtained from the binary expansion of n. Starting from
 *            the partition [1], process the bits of n from least‑significant
 *            to most‑significant, excluding the leading 1. For each 0‐bit
 *            increment every part by 1; for each 1‐bit append a new part of
 *            size 1. The value is the concatenation of the...
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 511, u64 n <= 511,
 *            u128 n <= 511, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_log2_uint64.h"

/* number of decimal digits of a positive integer x (x≤ULLONG_MAX) */
static inline int num_digits(uint64_t x)
{
    int d = 1;
    while (x >= 10) {
        x /= 10;
        ++d;
    }
    return d;
}

seq_t a(idx_t n)
{
    /* maximum possible number of parts is floor_log2(n)+1 (≤64 for 64‑bit n). */
    uint64_t parts[65] = {0};
    size_t parts_len = 1;   /* start with [1] */
    parts[0] = 1U;

    /* number of bits below the highest set bit */
    idx_t bits = floor_log2_uint64(n);

    for (idx_t pos = 0; pos < bits; ++pos) {
        if ((n >> pos) & 1ULL) {
            /* bit is 1 → append a part of size 1 */
            parts[parts_len++] = 1ULL;
        } else {
            /* bit is 0 → increment every part by 1 */
            for (size_t j = 0; j < parts_len; ++j)
                parts[j] += 1ULL;
        }
    }

    /* concatenate decimal representations of the parts */
    seq_t result = 0;
    for (size_t j = 0; j < parts_len; ++j) {
        uint64_t part = parts[j];
        int d = num_digits(part);
        seq_t factor = 1;
        for (int k = 0; k < d; ++k)
            factor *= 10;
        result = result * factor + (seq_t)part;
    }

    return result;
}
