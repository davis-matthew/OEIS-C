/*
 * A342395  a(n) = Sum_{k=1..n} k^(n/gcd(k,n)).
 * https://oeis.org/A342395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} k^( n / gcd(k,n) )
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Euclidean algorithm for unsigned 64-bit integers */
static idx_t _gcd_u64(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Compute A342395 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t g   = _gcd_u64(k, n);
        idx_t exp = n / g;
        seq_t term = big_pow((seq_t)k, (seq_t)exp);
        sum += term;
    }
    return sum;
}
