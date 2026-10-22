/*
 * A336698  a(n) = A000265(1+A000265(sigma(n))), where A000265(k) gives the
 *          odd part of k.
 * https://oeis.org/A336698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd_part(1 + odd_part(sigma(n)))
 * Range:     u8 n <= 336, u16 n <= 9990, u32 n <= 9990, u64 n <= 9990,
 *            u128 n <= 9990, big any n
 * Helpers:   odd_part, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/odd_part.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);          /* sum of divisors of n */
    seq_t odd_sigma = odd_part(sigma);  /* odd part of sigma(n)   */
    seq_t sum = odd_sigma + (seq_t)1;   /* 1 + odd_part(sigma(n))*/
    return odd_part(sum);               /* odd part of the sum   */
}
