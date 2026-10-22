/*
 * A354232  Expansion of e.g.f. exp(log(1 + x)^5).
 * https://oeis.org/A354232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 120 * Sum_{k=1..n} binomial(n-1,k-1) * Stirling1(k,5) *
 *            a(n-k); a(0) = 1
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 18, i128 n <= 1,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Memoization cache for a(n) */
static seq_t *a_cache   = NULL;
static idx_t a_cache_cap = 0;
static idx_t a_last = 0;   /* last index computed */

/* Cache for s(k,5) (Stirling numbers of the 1st kind where m = 5) */
static seq_t *s5_cache = NULL;
static idx_t s5_cap   = 0;
static idx_t s5_last  = 0;            /* last k computed */
static seq_t s_prev[6];              /* s(n‑1,0..5) for current n‑1 */
static seq_t s_curr[6];              /* temporary storage */

/* Ensure the a() cache can hold indices up to n (inclusive) */
static void ensure_a(idx_t n)
{
    if (a_cache == NULL) {            /* first time allocation */
        a_cache_cap = (n > 0) ? n + 1 : 1;
        a_cache = (seq_t*)malloc(a_cache_cap * sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = 1;
        a_last = 0;
    } else if (a_cache_cap <= n) {    /* grow capacity */
        idx_t new_cap = a_cache_cap ? a_cache_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_cache_cap = new_cap;
    }
}

/* Ensure s5_cache holds S(k,5) for all k ≤ n */
static void ensure_s5(idx_t n)
{
    if (s5_last >= n) return;

    if (s5_cache == NULL) {           /* first time allocation */
        s5_cap = 1;
        s5_cache = (seq_t*)malloc(s5_cap * sizeof(seq_t));
        if (!s5_cache) abort();
        s5_cache[0] = 0;              /* s(0,5) = 0 */
        s_prev[0] = 1;                /* s(0,0) = 1 */
        for (idx_t i = 1; i <= 5; ++i) s_prev[i] = 0;
    } else if (s5_cap <= n) {         /* grow capacity */
        idx_t new_cap = s5_cap << 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(s5_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        s5_cache = tmp;
        s5_cap = new_cap;
    }

    for (idx_t k = s5_last + 1; k <= n; ++k) {
        s_curr[0] = 0;                /* s(k,0) = 0 for k > 0 */
        for (idx_t j = 1; j <= 5; ++j) {
            s_curr[j] = s_prev[j - 1] - (seq_t)(k - 1) * s_prev[j];
        }
        s5_cache[k] = s_curr[5];
        for (idx_t j = 0; j <= 5; ++j) s_prev[j] = s_curr[j];
    }
    s5_last = n;
}

/* Sequence a(n) */
seq_t a(idx_t n)
{
    ensure_a(n);
    ensure_s5(n);                      /* need S(k,5) up to n */

    /* compute any missing values up to n */
    for (idx_t i = a_last + 1; i <= n; ++i) {
        seq_t sum   = 0;
        seq_t binom = 1;               /* binom(i-1,0) */

        for (idx_t k = 1; k <= i; ++k) {
            seq_t s5k = s5_cache[k];
            if (s5k) {
                seq_t term = binom * s5k;
                term = term * a_cache[i - k];
                sum += term;
            }
            if (k < i) {
                /* update to binom(i-1,k) */
                binom = binom * (seq_t)(i - k) / (seq_t)k;
            }
        }
        a_cache[i] = 120 * sum;
    }
    a_last = n;
    return a_cache[n];
}
