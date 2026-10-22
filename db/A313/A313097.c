/*
 * A313097  Coordination sequence Gal.5.195.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = - a(n-1) - a(n-3) - a(n-4) + 2*a(n-5) + 2*a(n-6) +
 *            2*a(n-8) + 2*a(n-9) - a(n-10) - a(n-11) - a(n-13) - a(n-14) (n >
 *            14) Initial values: a(0)=1, a(1)=4, a(2)=9, a(3)=14, a(4)=19,
 *            a(5)=23, a(6)=28, a(7)=31, a(8)=39, a(9)=39, a(10)=49, a(11)=48,
 *            a(12)=58, a(13)=59, a(14)=65
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


static seq_t *cache   = NULL;   /* cached sequence values */
static idx_t  cache_cap = 0;    /* allocated size of cache   */
static idx_t  computed  = 0;    /* highest index with a value */

/* Ensure cache has space for indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= n) {
        idx_t new_cap = n + 1;
        if (cache_cap && new_cap < cache_cap * 2)
            new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                     /* allocation failure            */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Initialise cache with first 15 terms (indices 0..14) */
static void init_base(void)
{
    if (!cache_cap || computed < 14) {
        ensure_cache(14);
        cache[0]  = 1;  cache[1]  = 4;  cache[2]  = 9;
        cache[3]  = 14; cache[4]  = 19; cache[5]  = 23;
        cache[6]  = 28; cache[7]  = 31; cache[8]  = 39;
        cache[9]  = 39; cache[10] = 49; cache[11] = 48;
        cache[12] = 58; cache[13] = 59; cache[14] = 65;
        computed = 14;
    }
}

seq_t a(idx_t n)
{
    init_base();
    ensure_cache(n);

    if (n <= computed)
        return cache[n];

    /* Compute all missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* positive contributions: 2*a[i-5] + 2*a[i-6] + 2*a[i-8] + 2*a[i-9] */
        seq_t pos = cache[i - 5];
        pos += cache[i - 5];
        pos += cache[i - 6];
        pos += cache[i - 6];
        pos += cache[i - 8];
        pos += cache[i - 8];
        pos += cache[i - 9];
        pos += cache[i - 9];

        /* negative contributions: a[i-1] + a[i-3] + a[i-4] + a[i-10]
           + a[i-11] + a[i-13] + a[i-14] */
        seq_t neg = cache[i - 1];
        neg += cache[i - 3];
        neg += cache[i - 4];
        neg += cache[i - 10];
        neg += cache[i - 11];
        neg += cache[i - 13];
        neg += cache[i - 14];

        cache[i] = pos - neg;               /* guaranteed non‑negative   */
    }

    computed = n;
    return cache[n];
}
