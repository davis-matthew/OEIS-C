/*
 * A281086  Denominator of sum of reciprocals of numbers less than n that do
 *          not divide n.
 * https://oeis.org/A281086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Σ_{k=1}^{n-1, k∤n} 1/k
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* safety for out‑of‑range calls */
    seq_t num   = 0;                /* current numerator   */
    seq_t denom = 1;                /* current denominator */

    for (idx_t k = 1; k < n; ++k)   /* all k < n */
    {
        if (n % k == 0) continue;   /* skip divisors of n   */

        seq_t ks = (seq_t)k;        /* cast to seq_t       */
        /* add 1/k : num/denom + 1/ks */
        seq_t new_num = num * ks + denom;   /* (num*ks + denom) */
        seq_t new_den = denom * ks;         /* denom * ks        */
        seq_t g       = gcd(new_num, new_den);
        num   = new_num / g;
        denom = new_den / g;
    }
    return denom;
}
