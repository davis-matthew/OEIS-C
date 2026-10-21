/*
 * A247495  Generalized Motzkin numbers: Square array read by descending
 *          antidiagonals, T(n, k) = k!*[x^k](exp(n*x)* BesselI_{1}(2*x)/x),
 *          n>=0, k>=0.
 * https://oeis.org/A247495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 79, u32 n <= 174, u64 n <= 440,
 *            u128 n <= 1137, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A247495 – Generalized Motzkin numbers
 *  a(n) is the entry read by ascending antidiagonals of the array
 *  T(n,k) where
 *
 *      T(n,k) = Σ_{j=0..⌊k/2⌋} n^{k-2j} * C(k,2j) * C(2j,j) / (j+1)
 *
 *  The mapping from the 1‑dimensional index n to coordinates (i,j) is
 *
 *      let s = floor( (sqrt(8*n+1)-1)/2 )
 *      let off = n - s*(s+1)/2
 *      then i = off                 (row)
 *           j = s - off              (column)
 *
 *  ==============================================================================
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"   // binomial(idx_t, idx_t)

seq_t a(idx_t n)
{
    /* find diagonal s such that s(s+1)/2 <= n < (s+1)(s+2)/2 */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;

    idx_t offset = n - s * (s + 1) / 2;
    idx_t i = offset;           /* row index */
    idx_t j = s - offset;       /* column index */

    seq_t sum = 0;
    for (idx_t t = 0; t <= j / 2; ++t)
    {
        /* exponent for n */
        idx_t exp = j - 2 * t;

        /* n^exp */
        seq_t npow = 1;
        for (idx_t e = 0; e < exp; ++e)
            npow *= (seq_t)i;

        /* binomials */
        seq_t b1 = binomial(j, 2 * t);
        seq_t b2 = binomial(2 * t, t);

        /* term = npow * b1 * b2 / (t+1) */
        seq_t term = npow * b1;
        term = term * b2;
        term /= (t + 1);

        sum += term;
    }

    return sum;
}
