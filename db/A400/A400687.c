/*
 * A400687  a(n) = Sum_{k=1..n} gcd(k!, k*(k+1)/2)^2.
 * https://oeis.org/A400687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = Sum_{k=1..n} gcd(k!, k*(k+1)/2)^2 * with gcd(k!, T_k) =
 *            T_k except when k+1 is an odd prime, * then gcd = k/2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* Provides bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    /* The sequence starts at offset 1.  For safety we return 0 for n==0. */
    if (n == 0)
        return (seq_t)0;

    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        /* T_k = k*(k+1)/2  (all in seq_t arithmetic) */
        seq_t t = (seq_t)k * ((seq_t)k + (seq_t)1) / (seq_t)2;

        seq_t g;
        /* Check if k+1 is an odd prime */
        if (((k + 1) & 1ULL) && is_prime(k + 1))
            g = (seq_t)k / (seq_t)2;   /* gcd = (p-1)/2 = k/2 */
        else
            g = t;                     /* gcd = T_k */

        sum += g * g;   /* accumulate the squared gcd */
    }

    return sum;
}
