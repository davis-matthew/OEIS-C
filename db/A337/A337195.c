/*
 * A337195  The 2-adic valuation of 1+A000265(sigma(n)), where A000265 gives
 *          the odd part.
 * https://oeis.org/A337195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v2(1 + odd_part(sigma(n))) (A337195)
 * Range:     u8 n <= 8828, u16 n <= 9388, u32 n <= 9388, u64 n <= 9388,
 *            u128 n <= 9388, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);      /* sigma(n) */
    seq_t tmp = s;
    seq_t v2s = 0;
    while ((tmp & (seq_t)1) == (seq_t)0)
    {
        tmp >>= 1;
        v2s++;
    }
    seq_t odd = tmp;            /* odd part of sigma(n) */
    seq_t val = odd + 1;        /* 1 + odd part */
    seq_t v2 = 0;
    while ((val & (seq_t)1) == (seq_t)0)
    {
        val >>= 1;
        v2++;
    }
    return v2;
}
