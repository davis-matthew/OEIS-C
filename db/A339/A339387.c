/*
 * A339387  a(n) = Sum_{k=1..n} (lcm(n,k)/gcd(n,k) mod k).
 * https://oeis.org/A339387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (lcm(n,k)/gcd(n,k) mod k)
 * Range:     u8 n <= 41, u16 n <= 569, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A339387. */
#include <stdint.h>
#include "../utils/gcd.h"     /* provides seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t g   = gcd((seq_t)n, (seq_t)k);
        seq_t p   = (seq_t)n / g;
        seq_t q   = (seq_t)k / g;
        seq_t term = (p * q) % (seq_t)k;   /* (n/g)*(k/g) mod k */
        sum += term;
    }
    return sum;
}
