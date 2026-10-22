/*
 * A351568  Sum of the divisors of the largest unitary divisor of n that is a
 *          square.
 * https://oeis.org/A351568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = σ( max unitary square divisor of n ) where σ(x) is the
 *            sum of all divisors of x
 * Range:     u8 n <= 48, u16 n <= 9250, u32 n <= 9250, u64 n <= 9250,
 *            u128 n <= 9250, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the helper routines that are provided in the utilities. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Return a(n) for n >= 1 (a(1) = 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* not used for the offset 1 */

    /* Factorise n.  The factorisation routine returns prime factors in
       the array 'pr' and their corresponding exponents in 'ex' and sets
       '*cnt' to the number of distinct prime factors. */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = 1;                         /* σ(1) = 1 */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = ex[i];
        /* We keep only primes with an even exponent to form a square. */
        if ((e & 1) == 0) {
            seq_t p  = (seq_t)pr[i];           /* base prime */
            seq_t exp = (seq_t)(e + 1);        /* exponent for p^(e+1) */

            /* Compute p^(e+1). */
            seq_t pow = big_pow(p, exp);

            /* σ(p^e) = (p^(e+1)-1)/(p-1). */
            seq_t num = pow - 1;
            seq_t den = p - 1;
            seq_t term = num / den;             /* exact division */

            /* Accumulate the product of σ(p^e) over all even‑exponent primes. */
            result = result * term;
        }
    }
    return result;
}
