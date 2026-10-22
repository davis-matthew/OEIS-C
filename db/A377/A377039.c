/*
 * A377039  Antidiagonal-sums of the array A377038(n,k) = n-th term of k-th
 *          differences of squarefree numbers (A005117).
 * https://oeis.org/A377039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{m=0}^{n} (-1)^{n-m} * binom(n+1, m) * s_m where s_m is
 *            the m‑th squarefree number (A005117) starting with s_0 = 1. To
 *            compute a(n) we first generate all squarefree numbers up to
 *            index n and then evaluate the above sum using incremental
 *            binomial coefficients
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 28, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   grow_seq_array, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_squarefree.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* ---------- lazily generate squarefree numbers up to index n ---------- */
    static seq_t *sqfree = NULL;   /* array of squarefree numbers s[0..] */
    static size_t sqfree_len   = 0; /* number of entries currently stored   */
    static size_t sqfree_cap   = 0; /* allocated capacity                    */
    static idx_t   next_candidate = 1; /* next natural number to test     */

    if (sqfree_len <= n) {
        /* ensure enough capacity for the new entries */
        if (sqfree_cap < (size_t)(n + 1))
            grow_seq_array(&sqfree, &sqfree_cap, n + 1);

        /* fill until we have indices 0..n */
        while (sqfree_len <= n) {
            if (is_squarefree((seq_t)next_candidate))
                sqfree[sqfree_len++] = (seq_t)next_candidate;
            ++next_candidate;
        }
    }

    /* ---------- compute a(n) via the binomial‑sum formula ---------- */
    seq_t sum   = 0;
    seq_t binom = 1;                 /* binom(n+1,0) */
    idx_t i, m;

    for (m = 0; m <= n; ++m) {
        /* sign = (-1)^{n-m} */
        seq_t sign = (((n - m) & 1U) ? (seq_t)-1 : (seq_t)1);

        seq_t term = sqfree[m] * binom;
        if (sign < 0)
            term = -term;
        sum += term;

        /* update binom to binom(n+1, m+1) for next iteration */
        if (m < n) {
            seq_t mult = (seq_t)(n + 1 - m);
            seq_t div  = (seq_t)(m + 1);
            binom = (binom * mult) / div;   /* exact division */
        }
    }

    return sum;
}
