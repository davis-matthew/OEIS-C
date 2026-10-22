/*
 * A337199  Binary weight of A337194(n) = 1+A000265(sigma(n)), where
 *          A000265(k) gives the odd part of k.
 * https://oeis.org/A337199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial weight of (1 + odd_part(sigma_of(n)))
 * Range:     u8 n <= 419, u16 n <= 8027, u32 n <= 8027, u64 n <= 8027,
 *            u128 n <= 8027, big any n
 * Helpers:   odd_part, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/odd_part.h"

seq_t a(idx_t n)
{
    /* sigma_of returns the sum of divisors of n (σ(n)) */
    seq_t s = sigma_of(n);

    /* odd_part returns the odd part of its argument, i.e. σ(n) with all factors of 2 removed */
    seq_t odd = odd_part(s);

    /* Compute 1 + odd part */
    seq_t value = odd + 1;

    /* Count the number of 1-bits in binary representation of value */
    seq_t cnt = 0;
    seq_t tmp = value;
    while (tmp)
    {
        cnt += tmp & 1;
        tmp >>= 1;
    }
    return cnt;
}
