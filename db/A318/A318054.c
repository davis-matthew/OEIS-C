/*
 * A318054  a(n) = n*(n + 1)*(n^2 + n + 22)/24.
 * https://oeis.org/A318054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2+n+22)/24
 * Range:     u8 n <= 7, u16 n <= 34, u32 n <= 566, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a simplified combinatorial reduction to avoid overflow.
   The function handles any seq_t type (8/16/32/64/128 bits or
   arbitrary precision).  No memoization is required because
   the computation is O(1). */

#include "../utils/gcd.h"  /* provides seq_t gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    seq_t N = n;
    seq_t f[3];
    f[0] = N;
    f[1] = N + 1;
    f[2] = N * N + N + 22;

    seq_t denom = 24;
    for (int i = 0; i < 3 && denom > 1; ++i) {
        seq_t g;
        while ((g = gcd(f[i], denom)) != 1) {
            f[i] /= g;
            denom /= g;
        }
    }
    /* The product is guaranteed divisible by the original denom.
       After cancelling common factors, denom must be 1. */
    seq_t result = f[0];
    result *= f[1];
    result *= f[2];
    return result;
}
