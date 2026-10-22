/*
 * A393287  a(n) is the least k >= 1 such that n + k^2 is the digit reversal
 *          of a square.
 * https://oeis.org/A393287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 498, u64 n <= 498, u128 n <= 498,
 *            big any n
 * Helpers:   reverse64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A393287: a(n) is the least k ≥ 1 such that n + k^2 is the digit
   reversal of a perfect square, with no trailing zeros in n + k^2. */

#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/reverse64.h"

/* Integer square root of an unsigned 64‑bit integer: floor(sqrt(x)). */
static unsigned long long
isqrt64(unsigned long long x)
{
    unsigned long long low  = 0, high = (unsigned long long)1 << 32, mid, sq;
    while (low < high) {
        mid = (low + high) >> 1;
        sq  = mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* OEIS A393287 sequence term. */
seq_t a(idx_t n)
{
    unsigned long long n_val = (unsigned long long)n;
    unsigned long long k = 1;

    while (true) {
        /* Compute k^2, using 128‑bit to avoid overflow on 64‑bit seq_t. */
        unsigned __int128 prod128 = (unsigned __int128)k * k;
        if (prod128 > ULLONG_MAX) {
            /* k is too large for 64‑bit arithmetic; not needed for n ≤ 10 000. */
            return (seq_t)k;
        }
        unsigned long long kk = (unsigned long long)prod128;

        unsigned long long val = n_val + kk;

        /* Reject values ending in zero. */
        if ((val % 10) == 0) {
            ++k;
            continue;
        }

        unsigned long long rev = reverse64(val);
        unsigned long long s   = isqrt64(rev);
        if (s * s == rev)        /* rev is a perfect square */
            return (seq_t)k;     /* smallest k found */

        ++k;
    }
}
