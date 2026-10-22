/*
 * A390612  a(n) = Sum_{k=0..n} (k+1) * 2^k * binomial(k,n-k)^2.
 * https://oeis.org/A390612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} (k+1) * 2^k * binomial(k, n-k)^2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the provided utility binomial() and pow2() functions. */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* k runs from ceil(n/2) to n because binomial(k, n-k) is zero otherwise */
    idx_t start = (n + 1) >> 1;          /* ceil(n/2) for unsigned n */
    for (idx_t k = start; k <= n; ++k) {
        idx_t j = n - k;                /* j = n-k, satisfies 0 <= j <= k */
        seq_t bin = binomial(k, j);
        bin = bin * bin;                /* square the binomial */
        seq_t term = pow2(k);           /* 2^k */
        term = term * bin;              /* 2^k * bin^2 */
        term = term * ((seq_t)(k) + 1); /* *(k+1) */
        sum += term;
    }
    return sum;
}
