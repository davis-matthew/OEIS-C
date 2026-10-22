/*
 * A329655  Square array read by antidiagonals: T(n,k) is the number of
 *          relations between set A with n elements and set B with k elements
 *          that are both right unique and left unique.
 * https://oeis.org/A329655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i,j) where T(n,k) = Sum_{t=1..k} binomial(n,t) *
 *            binomial(k,t) * t! * The index n is read in antidiagonal order:
 *            a(1)=T(1,1), a(2)=T(1,2), a(3)=T(2,1), etc
 * Range:     u8 n <= 31, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d(d+1)/2 >= n */
    idx_t d = 0;
    while ((d * (d + 1) / 2) < n)
        ++d;

    /* Previous cumulative count */
    idx_t cum_prev = d * (d - 1) / 2;
    idx_t offset = n - cum_prev - 1;       /* zero‑based position within diagonal */

    /* Corresponding i and j */
    idx_t i = offset + 1;
    idx_t j = d - offset;

    /* Compute T(i,j) = sum_{t=1..j} binomial(i,t) * binomial(j,t) * t! */
    seq_t sum = 0;
    for (idx_t t = 1; t <= j; ++t) {
        seq_t c1 = binomial(i, t);
        seq_t c2 = binomial(j, t);
        seq_t f  = factorial(t);
        sum += c1 * c2 * f;
    }
    return sum;
}
