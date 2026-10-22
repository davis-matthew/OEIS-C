/*
 * A365084  G.f. satisfies A(x) = 1 + x*A(x) / (1 + x)^5.
 * https://oeis.org/A365084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4*a(n-1) -10*a(n-2) -10*a(n-3) -5*a(n-4) - a(n-5) with
 *            initial values a(0)=1, a(1)=1, a(2)=-4, a(3)=6, a(4)=6, a(5)=-49
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 26, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for sequence values */
static seq_t *seq_cache      = NULL; /* array of computed values   */
static size_t seq_cache_cap  = 0;    /* allocated capacity         */
static size_t seq_cache_len  = 0;    /* number of values stored    */

/* Ensure the cache holds at least n+1 elements */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= seq_cache_cap)
        return;

    size_t new_cap = need * 2;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Abort on allocation failure */
        abort();
    }
    seq_cache      = new_cache;
    seq_cache_cap  = new_cap;
}

/* Compute all values up to n using the recurrence */
static void compute_to(idx_t n)
{
    /* Initialize base cases if cache is empty */
    if (seq_cache_len == 0) {
        ensure_cache(5);
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)1;
        seq_cache[2] = (seq_t)-4;
        seq_cache[3] = (seq_t)6;
        seq_cache[4] = (seq_t)6;
        seq_cache[5] = (seq_t)-49;
        seq_cache_len = 6; /* indices 0..5 stored */
    }

    if (seq_cache_len > (size_t)n)
        return; /* already computed */

    ensure_cache(n);

    for (size_t i = seq_cache_len; i <= (size_t)n; ++i) {
        /* For i < 6, values already set; recurrence applies from i = 6 */
        if (i < 6)
            continue;
        seq_t val = -4 * seq_cache[i - 1];
        val += -10 * seq_cache[i - 2];
        val += -10 * seq_cache[i - 3];
        val += -5  * seq_cache[i - 4];
        val += -seq_cache[i - 5];
        seq_cache[i] = val;
    }

    seq_cache_len = (size_t)n + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    compute_to(n);
    return seq_cache[(size_t)n];
}
