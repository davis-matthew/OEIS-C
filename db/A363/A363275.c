/*
 * A363275  Expansion of 1 / Sum_{k>=0} x^(k*(3*k - 2)).
 * https://oeis.org/A363275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = - Σ_{k≥1 : k(3k-2)≤n} a(n - k(3k-2)), a(0)=1 OEIS
 *            A363275: Expansion of 1 / Σ_{k≥0} x^(k*(3*k - 2))
 * Range:     i8 n <= 33, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*--- cached data -----------------------------------------------------*/
static seq_t *a_cache = NULL;    /* a(0) .. a(n) */
static size_t cache_len = 0;     /* number of elements stored */

/*--- pre‑computed exponents i = k(3k-2) --------------------------------*/
static idx_t *exps = NULL;   /* list of exponents for k≥1 */
static size_t exps_len = 0;   /* how many stored */
static size_t exps_cap = 0;   /* allocated capacity */
static idx_t  next_k   = 1;    /* next k to generate */

/*
 * Make sure the array exps contains all exponents ≤ n.
 * Only appends new entries; uses exponential reallocation.
 */
static void
ensure_exps_up_to(idx_t n)
{
    while (1) {
        idx_t e = next_k * (3*next_k - 2);
        if (e > n) break;
        if (exps_len == exps_cap) {               /* grow the array */
            size_t new_cap = (exps_cap ? exps_cap * 2 : 8);
            idx_t *tmp = (idx_t *)realloc(exps, new_cap * sizeof(idx_t));
            if (!tmp) exit(EXIT_FAILURE);
            exps = tmp;
            exps_cap = new_cap;
        }
        exps[exps_len++] = e;
        ++next_k;
    }
}

/*
 * Extend the cache a_cache[0..n] using the recurrence.
 * The cache is grown only as needed.
 */
static void
ensure_a_up_to(idx_t n)
{
    size_t new_len = (size_t)n + 1;
    if (new_len <= cache_len) return;            /* nothing to do */

    seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    a_cache = tmp;

    /* initial value a(0)=1 */
    if (cache_len == 0) {
        a_cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* compute new terms, starting from the first missing index */
    for (size_t m = cache_len; m < new_len; ++m) {
        ensure_exps_up_to((idx_t)m);
        seq_t sum = 0;
        /* exps are sorted increasingly; stop when exceeding m */
        for (size_t ei = 0; ei < exps_len && exps[ei] <= (idx_t)m; ++ei) {
            sum += a_cache[m - exps[ei]];
        }
        a_cache[m] = -sum;
    }

    cache_len = new_len;
}

/* --- public function -------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_a_up_to(n);
    return a_cache[(size_t)n];
}
