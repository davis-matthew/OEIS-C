/*
 * A350479  G.f. A(x) satisfies: A(x) = A(x^3 - x^5)/x^2.
 * https://oeis.org/A350479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1 Recurrence derived from A(x)=A(x^3-x^5)/x^2: a(n) =
 *            Σ_{m} a(m) * binom(2m-1, n-3m+2) * (-1)^{n-3m+2} where the sum
 *            is over m such that (n+4)/5 ≤ m ≤ (n+2)/3 (i.e. those m for
 *            which 3m-2 ≤ n ≤ 5m-3). The function computes terms by iterating
 *            over m in increasing order and accumulating cont...
 * Range:     i8 n <= 13, i16 n <= 32, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* static data and helpers */
static seq_t *seq = NULL;          /* sequence array, 1-based */
static idx_t seq_cap = 0;          /* allocated capacity (number of elements) */
static idx_t cached_n = 0;         /* highest n that has been fully computed */

/* ensure seq has room for indices up to 'need' (inclusive) */
static void ensure_seq(idx_t need)
{
    if (need < seq_cap) return;
    idx_t cap = seq_cap ? seq_cap : 16;
    while (cap <= need) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    if (cap > seq_cap) {
        for (idx_t i = seq_cap; i < cap; ++i) seq[i] = (seq_t)0;
    }
    seq_cap = cap;
}

/* incrementally compute terms up to N (inclusive) */
static void compute_upto(idx_t N)
{
    if (N <= cached_n) return;
    ensure_seq(N);
    idx_t old = cached_n;           /* previous maximum computed */

    /* initial base value */
    if (old == 0 && N >= 1) seq[1] = (seq_t)1;

    idx_t max_n_new = (N + 2) / 3;
    /* Iterate over all contributor indices */
    for (idx_t n = 1; n <= max_n_new; ++n) {
        idx_t start = 3 * n - 2;
        if (start > N) continue;

        /* first valid k gives m = start; k ranges up to 2n‑1 or until m > N */
        idx_t k_max = 2 * n - 1;
        if (start + k_max > N) k_max = N - start;
        if (k_max < 0) continue;

        seq_t bin = (seq_t)1;   /* binom(2n-1, 0) */

        for (idx_t k = 0; k <= k_max; ++k) {
            idx_t m = start + k;

            /* avoid adding to indices already computed in previous call */
            if (m > old) {
                if (m != n) {   /* skip the trivial self‑contribution when n=1 */
                    seq_t term = seq[n];
                    term *= bin;
                    if (k & 1) term = -term;
                    seq[m] += term;
                }
            }

            /* update binomial coefficient for next k */
            if (k < k_max) {
                seq_t num = bin * (seq_t)(2 * n - 1 - k);
                seq_t den = (seq_t)(k + 1);
                bin = num / den;      /* exact division */
            }
        }
    }

    cached_n = N;
}

/* --------------------------------------------------------------- */
/* user function required by the test harness                    */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (cached_n < n) compute_upto(n);
    return seq[n];
}
