/*
 * A251176  G.f. A(x) satisfies A(x) = 1 + x/(1-9*x^3)^(1/3) * A(
 *          x/sqrt(1-9*x^3)^(1/3) ).
 * https://oeis.org/A251176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{⌊(n-1)/3⌋} a(n-3k-1) * C(n/3-1, k) * 9^k with
 *            a(0)=a(1)=a(2)=a(3)=1 The binomial C(α , k) uses the general
 *            (possibly fractional) definition. This recurrence follows
 *            directly from A(x) = 1 + G(x)*A(G(x)) where G(x) =
 *            x/(1-9x^3)^(1/3)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache of previously computed terms */
static seq_t *seq = NULL;
static idx_t computed_max = 0;       /* highest index computed (inclusive) */

/* ensure the cache has room for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (seq == NULL) {
        /* initialise at least for indices 0..3 */
        size_t init_cap = (n < 4) ? 4 : n + 1;
        seq = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (seq == NULL) abort();
        /* base values a(0) .. a(3) = 1 */
        for (idx_t i = 0; i < 4; ++i) seq[i] = (seq_t)1;
        computed_max = 3;
    } else if (computed_max < n) {
        size_t new_cap = (size_t)(n + 1);
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (seq == NULL) abort();
        /* compute all missing terms */
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            if (i < 4) {
                seq[i] = (seq_t)1;   /* safety guard */
                continue;
            }
            idx_t maxk = (i - 1) / 3;
            seq_t term_val = (seq_t)1;          /* C(α,0)*9^0 = 1 */
            seq_t sum = seq[i - 1];             /* k = 0 term */

            for (idx_t k = 1; k <= maxk; ++k) {
                /* factor = i - 3 - 3*(k-1)  (≥1 because k ≤ maxk) */
                idx_t factor = i - 3 - 3 * (k - 1);
                /* term_val = term_val * factor * 3 / k  */
                term_val = term_val * (seq_t)factor * (seq_t)3 / (seq_t)k;

                idx_t idx = i - 3 * k - 1;
                sum += seq[idx] * term_val;
            }
            seq[i] = sum;
        }
        computed_max = n;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq[n];
}
