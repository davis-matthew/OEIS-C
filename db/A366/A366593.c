/*
 * A366593  G.f. A(x) satisfies A(x) = 1 + x^2*(1+x)^3*A(x)^4.
 * https://oeis.org/A366593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*k,n-2*k) *
 *            binomial(4*k,k)/(3*k+1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed directly, no caching. */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n >> 1;          /* floor(n/2) */
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(3 * k, n - 2 * k);
        seq_t t2 = binomial(4 * k, k);
        seq_t term = t1 * t2;
        term = term / (seq_t)(3 * k + 1);  /* exact division by small integer */
        sum += term;
    }
    return sum;
}
