/*
 * A392255  a(n) = Sum_{k=0..floor(n/2)} binomial(k+2,2) *
 *          binomial(k,3*(n-2*k)).
 * https://oeis.org/A392255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(k+2,2) * binomial(k,3*(n-2*k))
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 56, u64 n <= 121,
 *            u128 n <= 254, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* lower bound for k: ceil(3*n/7) */
    idx_t lo = (3 * n + 6) / 7;
    /* upper bound for k: floor(n/2) */
    idx_t hi = n / 2;

    seq_t sum = 0;
    if (lo > hi)
        return sum;

    for (idx_t k = lo; k <= hi; ++k) {
        /* t = 3*(n - 2*k) */
        idx_t t = 3 * (n - 2 * k);
        seq_t term1 = binomial(k + 2, 2);
        seq_t term2 = binomial(k, t);
        sum += term1 * term2;
    }

    return sum;
}
