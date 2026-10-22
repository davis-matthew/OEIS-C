/*
 * A329723  Coefficients of expansion of (1-2x^3)/(1-x-x^2) in powers of x.
 * https://oeis.org/A329723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329723: coefficients of (1-2x^3)/(1-x-x^2)
   Formula: a(0)=1, a(1)=1, a(2)=2, a(3)=1,
            a(n)=a(n-1)+a(n-2)  for n>=4 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* memoised values a(0..k) */
static idx_t  cache_cap = 0;      /* allocated capacity */
static idx_t  last_computed = 3;  /* highest index currently stored */

static void grow_cache(idx_t n)
{
    if (cache_cap >= n + 1) return;          /* already large enough */

    idx_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
    if (n + 1 > new_cap) new_cap = n + 1;     /* ensure space up to n */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* handle the first four values directly */
    if (n < 4) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)1;
        }
    }

    /* initialise cache on first call */
    if (!cache) {
        grow_cache(n);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
        last_computed = 3;
    } else {
        /* enlarge if needed */
        if (n + 1 > cache_cap) grow_cache(n);
    }

    /* compute missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2];
    }
    last_computed = (n > last_computed) ? n : last_computed;

    return cache[n];
}
