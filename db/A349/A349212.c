/*
 * A349212  a(n) = Sum_{d|n} n^(d mod 2).
 * https://oeis.org/A349212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} n^(d mod 2) (for each divisor d, add n if d is
 *            odd, otherwise add 1) Equivalent: a(n) = d_odd * (n + v2(n))
 *            where d_odd is the number of divisors of the odd part of n and
 *            v2(n) is the exponent of 2 in n
 * Range:     u8 n <= 44, u16 n <= 3464, u32 n <= 9892, u64 n <= 9892,
 *            u128 n <= 9892, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* Extract n, exponent of 2, and odd part */
    idx_t orig_n = n;          /* keep original n for final expression */
    idx_t k = 0;               /* exponent of 2 */
    idx_t tmp = n;
    while ((tmp & 1) == 0) {   /* faster check for evenness */
        k++;
        tmp >>= 1;
    }
    idx_t odd = tmp;           /* odd part of n */

    /* Compute number of divisors of the odd part, d_odd */
    seq_t d_odd = 1;
    if (odd > 1) {
        idx_t p = 3;
        /* trial division over odd primes */
        while (p * p <= odd) {
            if (odd % p == 0) {
                idx_t exp = 0;
                while (odd % p == 0) {
                    odd /= p;
                    exp++;
                }
                /* multiply current divisor count by (exp+1) */
                d_odd *= (seq_t)(exp + 1);
            }
            p += 2;
        }
        if (odd > 1) {
            /* remaining prime factor with exponent 1 */
            d_odd *= 2;
        }
    }

    /* Final result: d_odd * (n + k) */
    seq_t result = d_odd * ((seq_t)orig_n + (seq_t)k);
    return result;
}
