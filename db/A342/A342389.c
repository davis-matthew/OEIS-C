/*
 * A342389  a(n) = Sum_{k=1..n} k^gcd(k,n).
 * https://oeis.org/A342389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} k^gcd(k,n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety for non‑offset calls */

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t d = gcd_idx(k, n);
        seq_t term = big_pow((seq_t)k, (seq_t)d);
        sum += term;
    }

    return sum;
}
