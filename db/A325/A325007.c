/*
 * A325007  Array read by descending antidiagonals: A(n,k) is the number of
 *          achiral colorings of the facets of a regular n-dimensional
 *          orthotope using up to k colors.
 * https://oeis.org/A325007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 143, u64 n <= 325,
 *            u128 n <= 325, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the entry A(m,k) of the array of achiral colorings
 * read by descending antidiagonals.  The pair (m,k) is obtained from n
 * by determining the antidiagonal d such that 1+2+…+d ≥ n, then  
 * m = position on that diagonal, k = d − m + 1.  
 * For that (m,k):  A(m,k) = binom( binom(k+1,2) + m-1 , m )
 *                - binom( binom(k,2)   , m )
 */
#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* a(0) is unused; return 0 for safety. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Find the diagonal d such that d*(d+1)/2 ≥ n. */
    idx_t d  = 1;
    idx_t cum = 1;                /* cumulative count up to diagonal d */
    while (cum < n) {
        ++d;
        cum += d;
    }

    /* Position within this diagonal (1 … d). */
    idx_t cum_before = (d * (d - 1)) / 2;   /* count up to diagonal d-1 */
    idx_t pos        = n - cum_before;      /* 1 ≤ pos ≤ d */

    /* Translate to array indices: m = pos, k = d - pos + 1. */
    idx_t m = pos;
    idx_t k = d - pos + 1;

    /* Compute the two binomial terms. */
    seq_t inner1 = binom(k + 1, 2);          /* C(k+1,2)   */
    seq_t inner2 = binom(k,     2);          /* C(k,2)     */
    seq_t top    = inner1 + (seq_t)(m - 1);  /* C(k+1,2)+m-1 */

    seq_t first  = binom((idx_t)top, m);     /* C(top, m)   */
    seq_t second = binom((idx_t)inner2, m);  /* C(inner2, m) */

    return first - second;
}
