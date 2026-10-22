/*
 * A354135  Expansion of e.g.f. 1/(1 - log(1 + x)^5/120).
 * https://oeis.org/A354135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; a(n) = Σ_{k=1..n} binomial(n,k) * Stirling1(k,5) *
 *            a(n-k) where Stirling1(k,5) are signed Stirling numbers of the
 *            first kind
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Internal storage for the sequence values and Stirling1(k,5). */
static seq_t *a_cache = NULL;   /* a_cache[0..max_n] */
static idx_t  a_len   = 0;      /* number of filled entries (max_n+1) */
static idx_t  a_cap   = 0;      /* allocated capacity */

static seq_t *s5_cache = NULL;  /* s5_cache[k] = Stirling1(k,5) for k up to max_k */
static idx_t  s5_len   = 0;      /* number of filled entries (max_k+1) */
static idx_t  s5_cap   = 0;      /* allocated capacity */

/* Grow an array of seq_t elements to at least 'need'+1 capacity. */
static void grow_cache(seq_t **arr, idx_t *cap, idx_t need, size_t elem_size) {
    if (need < *cap) return;
    if (*cap == 0) *cap = 256;
    while (*cap <= need) *cap <<= 1;
    *arr = (seq_t *)realloc(*arr, *cap * elem_size);
}

/* Ensure that s5_cache[0..limit] holds Stirling1(k,5) for 0 ≤ k ≤ limit. */
static void ensure_s5_up_to(idx_t limit) {
    if (s5_cache != NULL && limit < s5_cap) {
        if (limit < s5_len) return;        /* already computed enough */
    }
    grow_cache(&s5_cache, &s5_cap, limit, sizeof(seq_t));

    /* Previous row of Stirling numbers (k = 0..5). */
    static seq_t prev[6] = {0};
    static int init_done = 0;
    if (!init_done) {
        prev[0] = 1;
        for (int i = 1; i <= 5; ++i) prev[i] = 0;
        init_done = 1;
        if (s5_len == 0) {                 /* ensure s5_cache[0] exists */
            s5_cache[0] = (seq_t)0;
            s5_len = 1;
        }
    }

    for (idx_t n = s5_len; n <= limit; ++n) {
        seq_t cur[6];
        cur[0] = (seq_t)0;
        for (int k = 1; k <= 5; ++k) {
            /* Recurrence: s1(n,k) = s1(n-1,k-1) - (n-1)*s1(n-1,k) */
            cur[k] = prev[k-1] - (seq_t)(n - 1) * prev[k];
        }
        s5_cache[n] = cur[5];            /* store Stirling1(n,5) */
        for (int k = 0; k <= 5; ++k) prev[k] = cur[k];
    }
    s5_len = limit + 1;
}

/* Ensure that a_cache[0..limit] holds the sequence values up to 'limit'. */
static void ensure_a_up_to(idx_t limit) {
    if (a_cache != NULL && limit < a_cap) {
        if (limit < a_len) return;
    }
    grow_cache(&a_cache, &a_cap, limit, sizeof(seq_t));

    if (a_len == 0) {                   /* base value */
        a_cache[0] = (seq_t)1;
        a_len = 1;
    }

    ensure_s5_up_to(limit);

    for (idx_t n = a_len; n <= limit; ++n) {
        if (n < 5) {                     /* a(1)..a(4) are 0 */
            a_cache[n] = (seq_t)0;
            continue;
        }
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;           /* binomial(n,0) */
        for (idx_t k = 1; k <= n; ++k) {
            /* Update binomial(n,k) from binomial(n,k-1) */
            binom = binom * (seq_t)(n - k + 1) / (seq_t)k;
            if (k >= 5) {
                seq_t term = binom * s5_cache[k];
                term       = term * a_cache[n - k];
                sum       += term;
            }
        }
        a_cache[n] = sum;
    }
    a_len = limit + 1;
}

/* Public function: return a(n). */
seq_t a(idx_t n) {
    if (n >= a_len) ensure_a_up_to(n);
    return a_cache[n];
}
