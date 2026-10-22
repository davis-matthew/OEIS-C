/*
 * A378608  Number of maximal chains in the poset of all compositions with
 *          weight <= n, ordered by A covers B iff A_i >= B_{i+k} for all i in
 *          B and some k >= 0.
 * https://oeis.org/A378608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A378608
 * a(n) = coefficient of t^n in Σ_{k≥0} L_k(t),
 * where L_0(t)=1 and for k≥1   
 * L_k(t) = (2*t*L_{k-1}(t) – t^k) / (1 – k*t).
 * For coefficients: l_k[n] = k*l_k[n-1] + 2*l_{k-1}[n-1] – (n==k?1:0),
 * with l_k[0] = 0 for k≥1 and l_0[0] = 1. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate two temporary arrays of size n+1 for coefficient tables. */
    size_t sz = (size_t)n + 1;                /* n is never huge, fits in size_t */
    seq_t *prev = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *curr = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!prev || !curr) return 0;             /* out‑of‑memory – unlikely */

    /* L_0(t) = 1 → prev[0] = 1, prev[i>0] = 0. */
    prev[0] = 1;
    for (idx_t i = 1; i <= n; ++i) prev[i] = 0;

    /* a(0) comes from L_0(t). */
    seq_t total = prev[n];

    for (idx_t k = 1; k <= n; ++k) {
        curr[0] = 0;                          /* no constant term for k≥1 */

        for (idx_t i = 1; i <= n; ++i) {
            seq_t term = (seq_t)k * curr[i-1] + ((seq_t)2 * prev[i-1]);
            if (i == k) term -= (seq_t)1;    /* subtract t^k term */
            curr[i] = term;
        }

        total += curr[n];                     /* contribution of L_k */

        /* Reuse arrays: swap prev and curr for the next k. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    free(prev);
    free(curr);
    return total;
}
