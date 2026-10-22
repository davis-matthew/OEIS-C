/*
 * A399199  Table of T(n, k) read by antidiagonals: Exact inverse of Pascal's
 *          triangle (A007318) such that T(binomial(N, k), k) = N, for n >= 0,
 *          k >= 1.
 * https://oeis.org/A399199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, new
 * Formula:   not stated
 * Range:     u8 n <= 1654, u16 n <= 6276, u32 n <= 9966, u64 n <= 9966,
 *            u128 n <= 9966, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n)  –  OEIS A399199  
    The sequence is obtained by reading the array T(n,k) by antidiagonals.  
    T(n,k) is defined as  
        T(n,k) = max { N ≥ k-1 | binomial(N,k) ≤ n } .  
    For a given index n (starting with n = 0) we first determine the
    diagonal d such that d(d+1)/2 > n.  Then we compute the position
    pos = n – (d-1)d/2 on that diagonal, giving
        row   n' = pos          (0 ≤ n' < d)
        col   k' = d - pos (k' ≥ 1).
    Finally a(n) = T(n',k').  All operations are performed on integer
    types only.
*/

#include <stdint.h>   /* for intptr_t, etc. – types seq_t and idx_t are supplied by the build */

/* Helper: compute binomial coefficient C(N,k) for unsigned indices.
   For N < k the result is 0.  The calculation is done multiplicatively
   and safely for small N and k. */
static seq_t binom_val(idx_t N, idx_t k)
{
    if (N < k) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (N - k + i);
        res = res / i;
    }
    return res;
}

/* Public function: a(n) for OEIS A399199. */
seq_t a(idx_t n)
{
    /* Find the diagonal d (smallest d≥1 with d(d+1)/2 > n). */
    idx_t d = 1;
    idx_t tri = 1;                      /* triangular number T(d) = d(d+1)/2 */
    while (tri <= n) {
        ++d;
        tri = d * (d + 1) / 2;
    }
    idx_t prev_tri = (d - 1) * d / 2;    /* triangular number of previous diagonal */

    /* Position on diagonal d (0‑based). */
    idx_t pos   = n - prev_tri;
    idx_t n_idx = pos;                   /* row index in T (0‑based) */
    idx_t k_idx = d - pos;               /* column index in T (1‑based) */

    /* Compute T(n_idx, k_idx): max N such that C(N, k_idx) ≤ n_idx. */
    idx_t high = k_idx - 1;               /* minimum possible N */
    while (binom_val(high + 1, k_idx) <= n_idx) {
        ++high;
    }

    return (seq_t)high;
}
