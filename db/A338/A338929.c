/*
 * A338929  a(n) is the smallest prime number p larger than A072668(n) such
 *          that p is equal to 1 (mod A072668(n)).
 * https://oeis.org/A338929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p = k*m+1 (k>=1), where m = A072668(n) =
 *            (n-th composite) - 1; found by search
 * Range:     u8 n <= 19, u16 n <= 1427, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9962 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t A338929_nth_composite_minus1(uint64_t n)
{
    uint64_t count = 0;
    uint64_t c = 3;
    while (count < n) {
        ++c;
        if (!is_prime_u64(c))
            ++count;
    }
    return c - 1;
}

seq_t a(idx_t n)
{
    uint64_t m = A338929_nth_composite_minus1((uint64_t)n);
    uint64_t p = m + 1;
    for (;;) {
        if (is_prime_u64(p))
            return (seq_t)p;
        p += m;
    }
}
