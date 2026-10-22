/*
 * A372624  Expansion of e.g.f. exp(1 - exp(x)) * (exp(x) - 1)^2 / 2.
 * https://oeis.org/A372624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∑_{k=0..n} (-1)^k * Stirling2(n,k) * binomial(k,2)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 17, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;      /* a(0)...a(current) */
static idx_t  cache_len = 0;       /* number of entries *stored* */
static seq_t *S_rows = NULL;       /* current row of Stirling numbers S(n,k) */
static idx_t  S_size = 0;          /* allocated length of S_rows */

/* Ensure S_rows has room for at least new_size elements.
   New slots are initialized to zero. */
static void ensure_S_rows(idx_t new_size) {
    if (S_size >= new_size) return;
    seq_t *tmp = (seq_t *)realloc(S_rows, new_size * sizeof(seq_t));
    if (!tmp) abort();
    for (idx_t i = S_size; i < new_size; ++i) tmp[i] = 0;
    S_rows = tmp;
    S_size = new_size;
}

/* Ensure a_cache has room for at least new_size elements.
   New slots are initialized to zero.  The actual cache_len remains
   unchanged – it is updated after computing new values. */
static void ensure_a_cache(idx_t new_size) {
    if (cache_len >= new_size) return;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    for (idx_t i = cache_len; i < new_size; ++i) tmp[i] = 0;
    a_cache = tmp;
    /* cache_len stays the same here */
}

/* Compute all a(m) for m ≤ n, storing them in a_cache. */
static void compute_upto(idx_t n) {
    if (n < cache_len) return;             /* already have what is needed */

    /* Allocate necessary space. */
    ensure_a_cache(n + 1);
    ensure_S_rows(n + 1);

    /* Initialise the first row if this is the first call. */
    if (cache_len == 0) {
        a_cache[0] = 0;
        S_rows[0]  = 1;   /* S(0,0)=1 */
        cache_len = 1;
    }

    /* Compute rows one by one. */
    for (idx_t cur = cache_len; cur <= n; ++cur) {
        if (cur == 0) continue;   /* base already handled */

        /* Update Stirling row for n = cur */
        for (idx_t k = cur; k >= 1; --k) {
            S_rows[k] = (seq_t)k * S_rows[k] + S_rows[k - 1];
        }
        S_rows[0] = 0;            /* S(n,0)=0 for next iteration */

        /* Compute a(cur) using the current row. */
        seq_t sum = 0;
        for (idx_t k = 2; k <= cur; ++k) {
            seq_t term = S_rows[k];
            term *= ((seq_t)k * (k - 1)) / 2;   /* binomial(k,2) */
            if (k & 1) sum -= term;             /* (-1)^k */
            else      sum += term;
        }
        a_cache[cur] = sum;
    }

    /* All values up to n are now stored. */
    cache_len = n + 1;
}

/* Public function: returns a(n). */
seq_t a(idx_t n) {
    compute_upto(n);
    return a_cache[n];
}
