/*
 * A354137  Expansion of e.g.f. exp(log(1 + x)^5/120).
 * https://oeis.org/A354137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; for n≥1, a(n) = Σ_{k=1..n} binomial(n‑1,k‑1) *
 *            Stirling1(k,5) * a(n‑k)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>               /* size_t */
#include "../utils/seq_realloc.h" /* seq_realloc */

static seq_t *a_cache   = NULL; /* cached a(n) values */
static size_t a_cache_len = 0;  /* number of entries currently stored */

static seq_t *s5_cache   = NULL; /* cached Stirling1(k,5) values */
static size_t s5_cache_len = 0;

static seq_t prev_stirling[6] = {0}; /* previous row for s(m,0..5) */
static seq_t curr_stirling[6] = {0};
static idx_t stir_last = 0;          /* last m for which Stirling numbers were computed */

/* Ensure all tables are computed up to and including n. */
static void ensure_upto(idx_t n)
{
    if (n < a_cache_len)
        return;                     /* everything already available */

    /* Grow a_cache */
    a_cache   = seq_realloc(a_cache,   a_cache_len, (size_t)n + 1);
    /* Grow s5_cache (same size) */
    s5_cache = seq_realloc(s5_cache, s5_cache_len, (size_t)n + 1);

    /* Zero the newly allocated entries */
    size_t old_len = a_cache_len;
    for (size_t i = old_len; i <= (size_t)n; ++i) {
        a_cache[i] = 0;
        s5_cache[i] = 0;
    }

    /* ----------  Compute Stirling1(k,5) for all k ≤ n  ---------- */
    /* Initialise the recurrence if this is the first call. */
    if (stir_last == 0) {
        prev_stirling[0] = (seq_t)1;
        for (int i = 1; i <= 5; ++i)
            prev_stirling[i] = (seq_t)0;
    }
    for (idx_t m = stir_last + 1; m <= n; ++m) {
        /* s(m,k) = s(m-1,k-1) - (m-1)*s(m-1,k)  for k≥1 */
        for (int k = 5; k >= 1; --k)
            curr_stirling[k] = prev_stirling[k-1] - (seq_t)(m-1) * prev_stirling[k];
        curr_stirling[0] = (seq_t)0;            /* s(m,0) = 0 for m>0 */
        s5_cache[m] = curr_stirling[5];         /* only the k=5 column is needed */

        /* copy curr → prev for next iteration */
        for (int k = 0; k <= 5; ++k)
            prev_stirling[k] = curr_stirling[k];
    }
    stir_last = n;

    /* ----------  Compute a(n) for all n ≤ current request  ---------- */
    if (a_cache_len == 0)               /* first computation, a(0)=1 */
        a_cache[0] = (seq_t)1;

    for (idx_t i = a_cache_len; i <= n; ++i) {
        if (i == 0) {                   /* already set, but keep explicit */
            a_cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;          /* binomial(i-1,0) */

        for (idx_t k = 1; k <= i; ++k) {
            if (k >= 5) {               /* Stirling1(k,5) is zero for k<5 */
                seq_t term = binom * s5_cache[k] * a_cache[i - k];
                sum = sum + term;
            }
            if (k < i) {                /* update binomial for next k */
                seq_t num = (seq_t)(i - k);
                seq_t den = (seq_t)k;
                binom = binom * num / den;
            }
        }
        a_cache[i] = sum;
    }

    a_cache_len   = (size_t)n + 1;
    s5_cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_upto(n);
    return a_cache[n];
}
