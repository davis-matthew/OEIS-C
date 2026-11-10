/*
 * A366379  Numbers k such that A057889(k) != k (mod 3), where A057889 is the
 *          bijective bit-reverse.
 * https://oeis.org/A366379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) is the n-th k >= 1 with A057889(k) mod 3 != k mod
 *            3, where A057889(k) = (binary reversal of the odd part of k) *
 *            2^(trailing zeros of k)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A057889_rev(uint64_t k)
{
    unsigned tz = 0;
    while ((k & 1ULL) == 0ULL) {
        k >>= 1;
        ++tz;
    }
    uint64_t r = 0;
    while (k) {
        r = (r << 1) | (k & 1ULL);
        k >>= 1;
    }
    return r << tz;
}

seq_t a(idx_t n)
{
    uint64_t k = 0;
    idx_t count = 0;
    while (count < n) {
        ++k;
        if (A057889_rev(k) % 3 != k % 3)
            ++count;
    }
    return (seq_t)k;
}
