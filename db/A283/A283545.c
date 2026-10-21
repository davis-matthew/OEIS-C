/*
 * A283545  Number of 4 X n 0..1 arrays with no 1 equal to more than one of
 *          its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 19*a(n-2) + 62*a(n-3) + 24*a(n-4) + 82*a(n-5)
 *            - 75*a(n-6) + 50*a(n-7) - 145*a(n-8) (n≥9) Initial values:
 *            a(1)=16, a(2)=98, a(3)=573, a(4)=4089, a(5)=25532, a(6)=167920,
 *            a(7)=1094959, a(8)=7101809
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed values, 1‑based indexing in terms of n */
static size_t cap   = 0;      /* allocated size of cache array */
static size_t len   = 0;      /* number of terms stored in cache (len == cache[0] … cache[len-1]) */

/* ensure the cache has room for at least 'need' terms (1‑based) */
static void grow_cache(size_t need)
{
    if (cache == NULL) {
        cap = need > 8 ? need : 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();   /* allocation failed – terminate */
        /* initialise with the first 8 terms */
        cache[0] = (seq_t)16;
        cache[1] = (seq_t)98;
        cache[2] = (seq_t)573;
        cache[3] = (seq_t)4089;
        cache[4] = (seq_t)25532;
        cache[5] = (seq_t)167920;
        cache[6] = (seq_t)1094959;
        cache[7] = (seq_t)7101809;
        len = 8;
    }
    if (need > cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failed – terminate */
        cache = tmp;
        cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* undefined for n < 1, but return 0 for safety */
    grow_cache((size_t)n);        /* guarantees cache holds n terms */

    if ((size_t)n <= len)        /* already computed */
        return cache[n-1];

    /* compute terms from len+1 up to n */
    for (size_t i = len + 1; i <= (size_t)n; ++i) {
        size_t idx = i - 1; /* zero‑based index in cache */

        /* positive contributions */
        seq_t pos = 0;
        pos += cache[idx-1] * (seq_t)2;
        pos += cache[idx-2] * (seq_t)19;
        pos += cache[idx-3] * (seq_t)62;
        pos += cache[idx-4] * (seq_t)24;
        pos += cache[idx-5] * (seq_t)82;
        pos += cache[idx-7] * (seq_t)50;

        /* negative contributions */
        seq_t neg = 0;
        neg += cache[idx-6] * (seq_t)75;
        neg += cache[idx-8] * (seq_t)145;

        cache[idx] = pos - neg;   /* pos must be >= neg for a(n) ≥ 0 */
    }

    len = (size_t)n;             /* cache now holds all terms up to n */
    return cache[n-1];
}
