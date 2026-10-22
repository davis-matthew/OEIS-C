/*
 * A357919  a(n) = Sum_{k=0..floor(n/3)} Stirling1(n - 2*k,k).
 * https://oeis.org/A357919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 10, i32 n <= 15, i64 n <= 23, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A357919:  a(n) = Sum_{k=0..⌊n/3⌋} Stirling1(n-2k,k)  (Stirling numbers of the first kind) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array to hold all a[0..n] */
    seq_t *a_vals = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!a_vals) return (seq_t)0;          /* out of memory: return zero as safe fallback */

    /* a(0) = 1, others initially zero */
    for (size_t i = 0; i <= (size_t)n; ++i) a_vals[i] = (seq_t)0;
    a_vals[0] = (seq_t)1;

    /* Size of Stirling‑number buffers: floor(n/3)+1 */
    size_t k_cap = (size_t)(n / 3) + 1;
    seq_t *prev_s = (seq_t*)malloc(k_cap * sizeof(seq_t));
    seq_t *cur_s  = (seq_t*)malloc(k_cap * sizeof(seq_t));
    if (!prev_s || !cur_s) {
        free(a_vals); free(prev_s); free(cur_s);
        return (seq_t)0;
    }

    /* Row m = 0 : s(0,0)=1 */
    prev_s[0] = (seq_t)1;
    size_t prev_k_max = 0;          /* maximum k index stored in prev_s for row 0 */

    /* Iterate over rows m = 1 .. n */
    for (idx_t m = 1; m <= n; ++m) {
        /* Determine maximum k for this row: k ≤ min(m, (n - m)/2) */
        idx_t t = (n - m) / 2;
        size_t cur_k_max = (m < t ? m : t);   /* floor values automatically */
        size_t cur_k_size = cur_k_max + 1;

        /* Compute cur_s[0..cur_k_max] from Stirling recurrence */
        cur_s[0] = (seq_t)0;                /* s(m,0)=0 for m>0 */
        for (size_t k = 1; k <= cur_k_max; ++k) {
            seq_t a_val = (k-1 <= prev_k_max) ? prev_s[k-1] : (seq_t)0;
            seq_t b_val = (k <= prev_k_max)     ? prev_s[k]   : (seq_t)0;
            cur_s[k] = a_val - (seq_t)(m-1) * b_val;
        }

        /* Add contributions to a[] : a[n] += s(m,k) where n = m + 2k */
        for (size_t k = 0; k <= cur_k_max; ++k) {
            idx_t idx = m + (idx_t)(2 * (idx_t)k);
            /* idx is always ≤ n by construction */
            a_vals[(size_t)idx] += cur_s[k];
        }

        /* Prepare for next iteration */
        seq_t *tmp = prev_s;
        prev_s = cur_s;
        cur_s  = tmp;
        prev_k_max = cur_k_max;
    }

    seq_t result = a_vals[(size_t)n];
    free(a_vals); free(prev_s); free(cur_s);
    return result;
}
