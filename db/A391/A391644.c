/*
 * A391644  Number of integer compositions of n that are the first sums of a
 *          unique composition.
 * https://oeis.org/A391644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Use the linear recurrence a(n) = 4*a(n-2) + a(n-3) - 4*a(n-4) -
 *            3*a(n-5) - a(n-6) + a(n-7) + 2*a(n-8) + 2*a(n-9) for n ≥ 10 with
 *            initial values a(1)…a(9) = 0,1,0,1,2,3,4,8,10
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static memoisation cache */
static seq_t *cache = NULL;          /* indexed from 0 (unused) up to n */
static size_t cache_len = 0;         /* actual allocated length */

/* initial terms (index 0 unused, 1‑based) */
static const seq_t init_terms[10] = {
    0,  /* unused */
    0,  /* a(1) */
    1,  /* a(2) */
    0,  /* a(3) */
    1,  /* a(4) */
    2,  /* a(5) */
    3,  /* a(6) */
    4,  /* a(7) */
    8,  /* a(8) */
    10  /* a(9) */
};

/* make cache large enough for indices up to need (inclusive) */
static void ensure_cache(size_t need)
{
    if (cache_len > need)
        return;
    seq_t *new_cache = (seq_t *)realloc(cache, (need + 1) * sizeof(seq_t));
    if (new_cache == NULL) {
        /* allocation failed – abort (should not happen in tests) */
        abort();
    }
    /* initialise new slots to 0 */
    for (size_t i = cache_len; i <= need; ++i)
        new_cache[i] = (seq_t)0;
    cache = new_cache;
    cache_len = need + 1;
}

/* initialise base terms up to 9 */
static void init_base_terms(void)
{
    if (cache_len <= 9)
        ensure_cache(9);
    for (size_t i = 1; i <= 9; ++i)
        cache[i] = init_terms[i];
    cache[0] = (seq_t)0;  /* safety */
}

/* compute terms from current_cache+1 up to n using the recurrence */
static void compute_to(idx_t n)
{
    if (n < (idx_t)10)
        return;  /* base terms already present */

    size_t start = cache_len;  /* first index not yet computed */
    if (start < (size_t)10)
        start = 10;  /* begin at 10 if cache is smaller */

    ensure_cache((size_t)n);

    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t term = (seq_t)4 * cache[i - 2]
                   + cache[i - 3]
                   - (seq_t)4 * cache[i - 4]
                   - (seq_t)3 * cache[i - 5]
                   - cache[i - 6]
                   + cache[i - 7]
                   + (seq_t)2 * cache[i - 8]
                   + (seq_t)2 * cache[i - 9];
        cache[i] = term;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined for n=0, return 0 for safety */

    init_base_terms();          /* guarantees cache up to 9 */
    compute_to(n);
    return cache[(size_t)n];
}
