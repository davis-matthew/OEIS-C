/*
 * A383480  Number of lattice paths from (0,0) to (n,n) using steps
 *          (1,0),(4,0),(0,1).
 * https://oeis.org/A383480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(n+k,k) *
 *            binomial(2*n-3*k,n-4*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static seq_t compute_term(idx_t n)
{
    idx_t maxk = n / 4;
    /* first term k = 0 : binomial(n+0,0)=1, binomial(2*n,n) */
    seq_t sum = binomial(2 * n, n);
    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t t1 = binomial(n + k, k);
        seq_t t2 = binomial(2 * n - 3 * k, n - 4 * k);
        sum += t1 * t2;
    }
    return sum;
}

seq_t a(idx_t n)
{
    return compute_term(n);
}
