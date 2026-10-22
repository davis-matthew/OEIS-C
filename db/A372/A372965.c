/*
 * A372965  a(n) = Sum_{k = 1..n} ( n/gcd(k, n) )^4.
 * https://oeis.org/A372965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{k = 1..n} (n/gcd(k, n))^4
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 88, u64 n <= 7150,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t res = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t g = gcd((seq_t)k, N);    /* gcd of k and n */
        seq_t t = N / g;               /* n/g */
        seq_t t2 = t * t;              /* (n/g)^2 */
        seq_t t4 = t2 * t2;            /* (n/g)^4 */
        res += t4;                     /* accumulate */
    }
    return res;
}
