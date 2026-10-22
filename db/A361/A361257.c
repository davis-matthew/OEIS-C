/*
 * A361257  a(n) = Sum_{j=0..n} n^wt(j), where wt = A000120.
 * https://oeis.org/A361257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{j=0..n} n^wt(j), wt(j) = A000120(j)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 60, u64 n <= 254,
 *            u128 n <= 3070, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine maximum exponent needed: floor(log2(n)) + 1.
       For n = 0 this yields 0, which is fine. */
    unsigned maxpow = 0;
    for (idx_t t = n; t; t >>= 1)
        ++maxpow;

    /* Pre‑compute powers n^k for k = 0..maxpow (maxpow ≤ 64). */
    seq_t pow_n[65];
    pow_n[0] = (seq_t)1;
    for (unsigned k = 1; k <= maxpow; ++k)
        pow_n[k] = pow_n[k - 1] * (seq_t)n;

    /* Sum the values n^popcount(j) for j = 0..n. */
    seq_t sum = 0;
    for (idx_t j = 0; j <= n; ++j) {
        unsigned pc = __builtin_popcountll((unsigned long long)j);
        sum += pow_n[pc];
    }
    return sum;
}
