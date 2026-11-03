/*
 * A337199  Binary weight of A337194(n) = 1+A000265(sigma(n)), where
 *          A000265(k) gives the odd part of k.
 * https://oeis.org/A337199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial weight of (1 + odd_part(sigma_of(n)))
 * Range:     u8 n <= 419, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8027 (2026-11-01)
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
