/*
 * A325816  Number of ways to form the sum sigma(n) = x+y, so that n-x and n-y
 *          are not coprime, with x and y in range 0..sigma(n).
 * https://oeis.org/A325816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..sigma(n)} [gcd(n-i, n-(sigma(n)-i)) ≠ 1]
 * Range:     u8 n <= 119, u16 n <= 9070, u32 n <= 9070, u64 n <= 9070,
 *            u128 n <= 9070, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Vote accurately counts all pairs (x,y) with x+y = sigma(n) such that
   n-x and n-y are not coprime; 0 and 0 are treated as not coprime. */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include <stdint.h>          /* for int64_t */

seq_t a(idx_t n)
{
    /* Compute sigma(n) – the sum of all positive divisors of n. */
    seq_t sigma = sigma_of(n);
    idx_t sig = (idx_t)sigma;          /* sigma is small enough for idx_t */

    seq_t cnt = 0;

    for (idx_t i = 0; i <= sig; ++i)
    {
        /* Differences: d1 = n - i, d2 = n - (sigma - i) = n - sigma + i */
        int64_t d1 = (int64_t)n - (int64_t)i;
        int64_t d2 = (int64_t)n - (int64_t)sig + (int64_t)i;

        /* Use absolute values because gcd is defined for non‑negatives. */
        seq_t a_val = (d1 < 0) ? (seq_t)(-d1) : (seq_t)d1;
        seq_t b_val = (d2 < 0) ? (seq_t)(-d2) : (seq_t)d2;

        /* Count when gcd != 1.  gcd(0,0)=0 hence counts as not coprime. */
        if (gcd(a_val, b_val) != (seq_t)1)
            ++cnt;
    }

    return cnt;
}
