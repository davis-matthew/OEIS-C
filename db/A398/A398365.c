/*
 * A398365  a(n) = Sum_{k=0..floor(n/8)} binomial(2*k,k) *
 *          binomial(n-6*k,2*k).
 * https://oeis.org/A398365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/8)} binomial(2*k,k) *
 *            binomial(n-6*k,2*k)
 * Range:     u8 n <= 18, u16 n <= 31, u32 n <= 57, u64 n <= 109,
 *            u128 n <= 212, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 8;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t c1 = binomial(2 * k, k);
        seq_t c2 = binomial(n - 6 * k, 2 * k);
        sum += c1 * c2;
    }
    return sum;
}
