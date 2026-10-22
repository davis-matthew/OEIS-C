/*
 * A318338  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2 or
 *          6 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 56, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Empirical recurrence from OEIS A318338:
   a(1)=4, a(2)=10, a(3)=20, a(4)=36, a(5)=76, a(6)=160
   For n>6: a(n) = 2*a(n-1)+2*a(n-2)-3*a(n-3)-2*a(n-4)+2*a(n-5)
   Numbers are stored in seq_t, indexed by idx_t (1‑based).
*/
#include <stdlib.h>

/* static cache for computed values (1‑based index) */
static seq_t *cache = NULL;   /* array of seq_t, cache[0] unused */
static size_t cache_cap = 0;  /* last allocated index */

/* base values for n = 1..6 */
static const seq_t base_vals[7] = {0, 4, 10, 20, 36, 76, 160};

seq_t a(idx_t n)
{
    /* handle trivial small n */
    if (n <= 6)
        return base_vals[(size_t)n];

    /* first allocation or enlarging the cache */
    if (cache == NULL) {
        /* allocate enough space for at least 7 elements (index 0..6) */
        cache_cap = 7;
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) abort();                /* allocation failure */
        cache[0] = 0;                       /* unused slot */
        /* copy base values into cache */
        for (size_t i = 1; i <= 6; ++i)
            cache[i] = base_vals[i];
        /* zero the rest */
        for (size_t i = 7; i <= cache_cap; ++i)
            cache[i] = 0;
    }

    /* enlarge cache if necessary */
    size_t N = (size_t)n;
    if (N > cache_cap) {
        size_t new_cap = N;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();                 /* reallocation failure */
        cache = tmp;
        /* initialize newly allocated entries to 0 */
        for (size_t i = cache_cap + 1; i <= new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }

    /* compute missing values up to N using the recurrence */
    for (size_t i = 7; i <= N; ++i) {
        if (cache[i] == 0) { /* value not yet computed */
            cache[i] = 2 * cache[i-1]
                       + 2 * cache[i-2]
                       - 3 * cache[i-3]
                       - 2 * cache[i-4]
                       + 2 * cache[i-5];
        }
    }

    return cache[N];
}
