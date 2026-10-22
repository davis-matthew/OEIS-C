/*
 * A352840  a(1) = 0; a(2) = 1; thereafter a(n) = a(n-1)/2 if a(n-1) is even,
 *          otherwise a(n) = a(n-1) + a((n-1)-a(n-1)).
 * https://oeis.org/A352840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(1)=0, a(2)=1; for n>2: if a(n-1) even, a(n)=a(n-1)/2; else
 *            a(n)=a(n-1)+a((n-1)-a(n-1))
 * Range:     u8 n <= 332, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache     = NULL;  /* array of computed terms, index 0 unused */
static size_t a_cache_cap = 0;     /* total capacity allocated (number of elements) */
static idx_t   a_cache_last = 0;   /* largest n for which a_cache[n] is valid */

/* Ensure the cache contains values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n <= a_cache_last) return;           /* nothing to do */

    /* grow the cache to hold indices 0..n */
    size_t needed_cap = (size_t)n + 1;        /* extra index 0 */
    if (a_cache_cap < needed_cap) {
        seq_t *tmp = (seq_t *)realloc(a_cache, needed_cap * sizeof(seq_t));
        if (!tmp) abort();                    /* out of memory */
        a_cache      = tmp;
        a_cache_cap  = needed_cap;
    }

    /* compute new terms */
    for (idx_t i = a_cache_last + 1; i <= n; ++i) {
        if (i == 1) {
            a_cache[i] = 0;
        } else if (i == 2) {
            a_cache[i] = 1;
        } else {
            seq_t prev = a_cache[i - 1];
            if ((prev & 1) == 0) {            /* even */
                a_cache[i] = prev >> 1;
            } else {                          /* odd */
                /* idx = (i-1) - prev; safe because prev <= i-2 for n>=3 */
                idx_t idx = (i - 1) - (idx_t)prev;
                a_cache[i] = prev + a_cache[idx];
            }
        }
    }
    a_cache_last = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1, return 0 safely */
    ensure_cache(n);
    return a_cache[n];
}
