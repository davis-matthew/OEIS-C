/*
 * A388067  Multiplicative sequence a(n) with a(p^e) = (1+floor(e/2)) *
 *          (2+floor(e/2)) / 2 for prime p and e >= 0.
 * https://oeis.org/A388067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} ((1+⌊e/2⌋)*(2+⌊e/2⌋))/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* OEIS offset 1: a(1)=1.  For n==0 we return 1 to avoid undefined
       behaviour, though no test case uses n==0. */
    if (n <= 1)
        return 1;

    seq_t result = 1;
    idx_t temp = n;
    idx_t e;

    /* Factor 2 separately (fast). */
    if ((temp & 1) == 0) {
        e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            e++;
        }
        if (e > 0) {
            seq_t t = e / 2;                       /* ⌊e/2⌋ */
            seq_t f = ((seq_t)(1 + t) * (seq_t)(2 + t)) / 2;
            result *= f;
        }
    }

    /* Factor odd primes up to sqrt(temp). */
    for (idx_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            e = 0;
            while (temp % i == 0) {
                temp /= i;
                e++;
            }
            seq_t t = e / 2;
            seq_t f = ((seq_t)(1 + t) * (seq_t)(2 + t)) / 2;
            result *= f;
        }
    }

    /* Remaining prime factor (if any) has exponent 1 => multiplier 1. */
    /* No action needed for temp > 1. */

    return result;
}
