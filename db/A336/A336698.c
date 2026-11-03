/*
 * A336698  a(n) = A000265(1+A000265(sigma(n))), where A000265(k) gives the
 *          odd part of k.
 * https://oeis.org/A336698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd_part(1 + odd_part(sigma(n)))
 * Range:     u8 n <= 336, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 113 terms through n = 9990 (2026-11-01)
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/odd_part.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);          /* sum of divisors of n */
    seq_t odd_sigma = odd_part(sigma);  /* odd part of sigma(n)   */
    seq_t sum = odd_sigma + (seq_t)1;   /* 1 + odd_part(sigma(n))*/
    return odd_part(sum);               /* odd part of the sum   */
}
