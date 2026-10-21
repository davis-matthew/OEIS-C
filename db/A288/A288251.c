/*
 * A288251  k-th term of n in the 3x+1 problem, k >= 1. Square array A(n, k)
 *          read by antidiagonals downwards.
 * https://oeis.org/A288251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look, tabl
 * Formula:   a(n) = C^{[k]}(i) where (i,k) are obtained from n by reading *
 *            the square array A(n,k) on antidiagonals (OEIS A288251). * C is
 *            the Collatz map: 3*x+1 if x odd, x/2 if x even
 * Range:     u8 n <= 696, u16 n <= 9985, u32 n <= 9985, u64 n <= 9985,
 *            u128 n <= 9985, big any n
 * Helpers:   collatz_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* for size_t (unused but harmless) */
#include "../utils/collatz_k.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n<1; return 0 for safety */

    /* Find the diagonal d.  Number of elements up to and including
     * diagonal d is d*(d-1)/2 (with d >= 2). */
    idx_t d = 2;
    while (d * (d - 1) / 2 < n)
        ++d;

    /* Elements before diagonal d */
    idx_t prev = (d - 1) * (d - 2) / 2;
    idx_t r     = n - prev;          /* position within the diagonal (1-based) */
    idx_t i     = r;                 /* row index (original starting number) */
    idx_t k     = d - r;             /* number of Collatz iterations */

    seq_t value = (seq_t)i;
    return collatz_k(value, k);
}
