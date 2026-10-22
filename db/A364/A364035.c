/*
 * A364035  Expansion of Sum_{k>0} k * x^k / (1 - 2*x^(2*k)).
 * https://oeis.org/A364035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d odd} 2^((d-1)/2) * (n/d)
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/* utilities that may be used */
#include "../utils/big_pow.h"     /* for big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* Extract the odd part of n (discard all powers of 2) */
    idx_t odd = n;
    while ((odd & 1ULL) == 0)          /* odd % 2 == 0 */
        odd >>= 1;                    /* odd /= 2 */

    seq_t sum = 0;

    /* Enumerate odd divisors of odd, using paired divisors for speed. */
    for (idx_t d = 1; d <= odd / d; ++d) {
        if (odd % d == 0) {
            /* case d itself */
            if ((d & 1ULL) == 1) {                 /* d odd */
                idx_t exp = (d - 1) >> 1;          /* (d-1)/2 */
                seq_t term = big_pow((seq_t)2, (seq_t)exp)
                             * ((seq_t)(n / d));
                sum += term;
            }
            /* check the paired divisor odd/d */
            idx_t d2 = odd / d;
            if (d2 != d && (d2 & 1ULL) == 1) {    /* d2 odd and not same */
                idx_t exp = (d2 - 1) >> 1;
                seq_t term = big_pow((seq_t)2, (seq_t)exp)
                             * ((seq_t)(n / d2));
                sum += term;
            }
        }
    }
    return sum;
}
